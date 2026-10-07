# RK3576 Side BSD Native Pipeline 성능 검증 기록

작성일: 2026-10-06  
대상 저장소: `WOOSEOK99/AI_Camera`  
기준 코드: `e01c06f98883c3c9ff9d3d4f9c09437528cb0bf6` (`Pipeline side capture with native inference`)

## 1. 목적

RK3576 보드에서 Side BSD의 좌/우 640x640 YOLO11 INT8 추론을 Native RKNN dual-context로 실행하고,
실제 `SideCamera.infer()`, 좌/우 optical flow, 3채널 영상 입력을 포함한 전체 파이프라인이
좌/우 각각 30 FPS 목표를 만족할 수 있는지 병목을 분리해서 확인한다.

이번 기록은 **Lab recorded-video + 실제 RK3576 보드 성능 확인**이다.
실제 V4L2 카메라 3채널 입력 성능은 별도 검증이 필요하며, 이 문서를 최종 Vehicle/Camera Validation PASS로 해석하지 않는다.

## 2. 시험 환경

### 보드

- SoC: RK3576
- OS: Debian 12
- Python: 3.11.2
- RKNN Runtime: 2.3.2
- RKNPU Driver: 0.9.8
- NPU governor: `userspace`
- NPU: 950 MHz / max 950 MHz
- DMC: 2112 MHz / max 2112 MHz
- CPU0~3: `performance`, 2016 MHz
- CPU4~7: `performance`, 2208 MHz
- 측정 중 thermal zone: 약 44~48°C

### CPU / OpenCV 배치

- main thread affinity: `0,1,2,3,6,7`
- LEFT RKNN worker: CPU4
- RIGHT RKNN worker: CPU5
- OpenCV threads: 2
- LEFT NPU: core0
- RIGHT NPU: core1

### 모델 / 라이브러리

제품 테스트 모델:

`/userdata/ai_camera/deploy/models/rk3576/yolo11n_rk3576_i8_test.rknn`

Native detector:

`/userdata/ai_camera/deploy/native/rk3576/librknn_detector.so`

입력 크기:

- YOLO input 640x640 유지
- 512x512 축소는 사용하지 않음

## 3. 코드 상태

이번 성능 검증에 사용한 주요 코드 변경은 다음과 같다.

- Native RKNN detector backend
- 좌/우 독립 RKNN context
- LEFT/RIGHT 전용 worker + CPU affinity
- 좌/우 Native inference 병렬화
- N inference 동안 N+1 capture/optical-flow를 준비하는 pipeline
- optical-flow 측정과 global ego state publish 분리
- frame index / timestamp를 capture cycle에 고정
- capture failure 적용을 prior inference 종료 뒤로 지연

현재 pipeline commit:

`e01c06f98883c3c9ff9d3d4f9c09437528cb0bf6`

RGA 전처리 실험은 parity/performance 문제로 제품 코드에서 되돌렸다.

RGA revert commit:

`0424d5edfa01b8b20d99fe5a756c82ebfabf6fe1`

## 4. PC 회귀 테스트

PC는 Python 3.11.9 전용 `.venv`를 생성해 아래 최소 패키지로 시험했다.

- numpy 1.26.4
- opencv-python 4.11.0.86
- scipy 1.17.1
- supervision 0.29.1

실행 대상:

- `tests.side_bsd.test_c02_fault_isolation_production`
- `tests.side_bsd.test_p1_production_paths`
- `tests.side_bsd.test_ego_motion`

결과:

```text
Ran 28 tests in 1.533s
OK
```

참고:

- `pyserial` 미설치로 HardwareIO는 Mock 모드
- ByteTrack deprecation warning은 시험 실패가 아님

## 5. Native / SideCamera 단독 성능

### 5.1 순수 Native dual RKNN

LEFT CPU4/core0, RIGHT CPU5/core1:

```text
DUAL_NATIVE_PAIR avg=21.17ms p95=21.74ms pair_fps=47.23
LEFT avg=21.05ms p95=21.61ms
RIGHT avg=20.49ms p95=20.68ms
```

판정:

- Native detector / NPU 자체는 정상
- 30 FPS budget 33.33 ms보다 충분히 빠름

### 5.2 실제 SideCamera.infer() dual

tracker / ROI / 거리 / 경보 정책까지 포함:

```text
SIDECAMERA_DUAL avg=28.41ms p95=28.49ms pair_fps=35.20
LEFT avg=24.23ms p95=24.30ms
RIGHT avg=27.82ms p95=28.24ms
```

Native detector 이후 Python SideCamera 후처리 비용은 존재하지만,
단독 실행 기준으로는 여전히 30 FPS budget 안에 들어온다.

## 6. Pipeline 병목 분리

### 6.1 3채널 decode + 좌/우 optical flow 단독

```text
PREP_ONLY avg=23.89ms p95=24.71ms
```

### 6.2 3채널 decode+flow를 side inference와 overlap

```text
CORE_OVERLAP avg=38.57ms p95=40.12ms fps=25.93
OVERLAP_PREP avg=34.19ms p95=36.49ms
OVERLAP_LEFT avg=35.82ms p95=39.17ms
OVERLAP_RIGHT avg=38.19ms p95=39.76ms
```

결론:

- 단순 overlap만으로는 충분하지 않음
- decode/optical-flow와 inference가 동시에 CPU/메모리 자원을 사용하면서 양쪽 모두 느려짐

### 6.3 decode만 overlap + flow 후처리

```text
DECODE_OVERLAP_CYCLE avg=43.14ms p95=44.60ms fps=23.18
DECODE_OVERLAP avg=21.98ms p95=22.19ms
POST_FLOW avg=8.09ms p95=8.65ms
LEFT avg=31.45ms p95=35.10ms
RIGHT avg=34.69ms p95=36.28ms
```

판정:

- 더 느려짐
- 이 구조는 채택하지 않음

### 6.4 REAR decode를 CPU6 별도 worker로 분리

```text
ASYNC_REAR_CYCLE avg=37.55ms p95=42.64ms fps=26.63
SIDE_PREP avg=30.75ms p95=40.22ms
REAR_DECODE avg=16.12ms p95=17.44ms
LEFT avg=33.79ms p95=37.37ms
RIGHT avg=36.30ms p95=39.19ms
```

판정:

- REAR decode를 별도 Python thread에 두는 것만으로 해결되지 않음
- CPU thread 배치보다 software video decode와 메모리/CPU 경합 영향이 큼

## 7. REAR 입력 영향 확인

### 7.1 REAR decode 포함 — 원본 1080p H.264 MKV

Full app에서 대략:

- LOOP: 약 19~22.7 FPS
- LOOP p95: 약 45.9~54.2 ms
- Side pipeline: 대략 33~39 ms

MULTI → SINGLE로 바꿔도 약 21~25 FPS 수준이라 rendering은 주 병목이 아니었다.

### 7.2 REAR decode 완전 제외

Side-only overlap microbenchmark:

```text
SIDE_ONLY_OVERLAP avg=32.45ms p95=33.47ms fps=30.82
SIDE_ONLY_PREP avg=22.12ms p95=24.51ms
LEFT avg=29.55ms p95=32.52ms
RIGHT avg=32.14ms p95=33.18ms
```

Full app 임시 monkey-patch로 전진 중 REAR decode만 제외:

```text
window 1: LOOP p95=33.1ms, fps=32.3, PERF_RESULT=PASS
window 2: LOOP p95=32.4ms, fps=32.7, PERF_RESULT=PASS
window 3: LOOP p95=34.0ms, fps=31.2
```

세 번째 window는 RIGHT pipeline p95=33.5ms로 기준을 약간 초과해 FAIL이었지만,
전체 처리율은 31.2 FPS였다.

핵심 결론:

**현재 좌/우 Native SideBSD 구조 자체는 약 30 FPS를 낼 수 있으며,
recorded-video 시험에서 REAR H.264 software decode가 30 FPS 달성을 막는 가장 큰 추가 부하로 확인됐다.**

## 8. 720p 후방 테스트 영상 재시험

PC에서 후방 테스트 영상을 다음 조건으로 변환해 보드에 업로드했다.

- 파일: `rearview.mp4`
- H.264
- 1280x720
- 30 FPS CFR
- 테스트 파일 크기 약 14 MB

보드:

`/userdata/ai_camera/video/in/rearview.mp4`

설정 파일은 변경하지 않고 runtime override로만 시험했다.

결과:

```text
window 1: LOOP p95=40.0ms, fps=27.1
window 2: LOOP p95=36.7ms, fps=28.9
window 3: LOOP p95=39.3ms, fps=26.3
window 4: LOOP p95=40.6ms, fps=26.2
```

1080p 원본보다 개선됐지만 30 FPS에는 미달했다.

따라서 해상도 축소만으로는 H.264 software decode 부담을 제거할 수 없었다.

## 9. RK3576 하드웨어 디코더 확인

보드에 다음이 존재한다.

- `/usr/bin/mpi_dec_test`
- `gst-launch-1.0`
- `gst-inspect-1.0`
- GStreamer `rockchipmpp:mppvideodec`

원본 rearview.mkv 정보:

- Matroska
- H.264 High Profile
- 1920x1080
- 30/1 FPS
- duration 36.333 s

MPP decode smoke test:

```bash
gst-launch-1.0 -q filesrc location=/userdata/ai_camera/video/in/rearview.mkv \
  ! matroskademux ! h264parse ! mppvideodec ! fakesink sync=false
```

결과:

```text
real 0m1.823s
user 0m0.347s
sys  0m0.280s
```

MPP hardware decode 자체는 매우 빠르게 동작한다.

다만 보드 Python venv의 OpenCV build는:

```text
GStreamer: NO
```

이므로 `cv2.VideoCapture(..., CAP_GSTREAMER)`로 바로 연결할 수 없다.

## 10. 현재 판단

현재는 **테스트 영상 기반 성능 분석 단계**이므로 REAR recorded-video decode를 위해
production pipeline을 추가로 복잡하게 변경하지 않는다.

실제품에서는 후방 카메라가 실제 V4L2/ISP 입력으로 들어오므로,
recorded H.264 파일을 OpenCV software decode하는 현재 시험 부하는 제품 입력과 동일하지 않을 수 있다.

따라서 다음 원칙으로 진행한다.

1. 640x640 YOLO input 유지
2. Native dual-context + side pipeline 구조 유지
3. recorded REAR H.264 decode 때문에 production code를 성급하게 변경하지 않음
4. 실제 좌/우/후방 카메라 연결 후 3채널 실입력으로 다시 성능 측정
5. 실제 카메라가 압축 스트림을 제공해 decode 병목이 재현될 때만 MPP/V4L2 zero-copy 경로를 설계

## 11. 별도 확인 사항

성능과 별개로 다음 경고/미완료가 남아 있다.

- LEFT/RIGHT 거리 calibration RMSE: 1.1764m > 0.75m
- REAR calibration RMSE: 0.3915m PASS
- TEST 모드에서 `TEST_ROI_DIRECT_ALERT_MODE` 활성
- HardwareIO는 pyserial 미설치로 Mock
- 실제 카메라 입력 Validation 미수행
- 실제 차량 thermal/power soak test 미수행

## 12. 최종 요약

**RK3576에서 YOLO11 640x640 INT8 Native dual-context는 정상이며,
실제 SideCamera 후처리까지 포함한 좌/우 경로는 30 FPS 목표에 근접/도달한다.
현재 recorded-video full app의 30 FPS 미달은 후방 H.264 파일을 OpenCV software decode하는 시험 부하가 핵심 원인으로 분리됐다.
최종 성능 판정은 실제 3채널 카메라 입력으로 다시 수행한다.**
