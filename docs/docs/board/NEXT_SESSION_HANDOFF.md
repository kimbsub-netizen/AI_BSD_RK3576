# AI_Camera RK3576 — 다음 작업 인수인계

> 이 파일은 새 채팅 시작용 **rolling handoff**다. 새 날짜별 인수인계 파일을 계속 만들지 말고 필요할 때 이 파일을 갱신한다.
>
> 기준 시점: 2026-10-06
>
> GitHub: `WOOSEOK99/AI_Camera`
>
> 현재 기준 commit: `2ede75a55041c24570b27551cf5106c85b8f587c`

## 1. 오늘 완료한 핵심

RK3576용 현재 기준 모델을 **YOLOv8n 640×640 INT8**로 정리했다.

초기 개발은 YOLO11로 시작했지만 성능 문제로 YOLOv8n으로 전환한 흐름이 맞으며, 남아 있던 YOLO11 기본 설정/문서 혼선을 정리했다.

현재 기준 모델:

```text
deploy/models/rk3576/yolov8n_rk3576_i8_test.rknn
SHA-256:
1B855C0ED7DD4CE143CCD711958BEB7459FC94804F5326CBFD262ECAF73075A7
```

현재 RK3576 INT8 선택값:

```text
rk3576_i8_test
rk3576_yolov8_i8_test
```

둘 다 위 YOLOv8n 모델을 가리킨다.

기존 YOLO11 모델은 과거 시험 재현용으로만 유지:

```text
rk3576_yolo11_i8_test
rk3576_yolo11_fp
```

Factory Release 기본 모델도 YOLOv8n으로 변경했고, manifest에 모델 경로와 SHA-256을 기록하도록 수정했다.

## 2. Native .so + YOLOv8 실보드 재검증 결과

AI_Camera Manager에 다음 버튼을 추가했다.

```text
Build / Release
→ Verify YOLOv8 + Native on Board
```

2026-10-06 개발 보드 실제 실행 결과:

```text
MODEL_PATH=/userdata/ai_camera/deploy/models/rk3576/yolov8n_rk3576_i8_test.rknn

MODEL_SHA256=
1b855c0ed7dd4ce143ccd711958beb7459fc94804f5326cbfd262ecaf73075a7

NATIVE_PATH=
/userdata/ai_camera/deploy/native/rk3576/librknn_detector.so

NATIVE_SHA256=
4414b7d6066330e58bfa81ca4be1b72ba1c8b482c0db12f62edc2f90633ef529

TEST_FRAME=
/userdata/rknn_yolov8_demo_rk3576/model/left_side_frame.jpg

DETECTIONS=5

BOARD_YOLOV8_NATIVE_VERIFY: PASS
```

따라서 현재 개발 보드에서는 다음이 확인 완료다.

- GitHub YOLOv8 모델과 보드 모델 SHA-256 일치
- Native `.so` AArch64 ELF 정상
- YOLOv8 모델 Native 로드 정상
- RKNN/NPU 실제 추론 정상
- 테스트 프레임에서 detection 5개 반환

즉 **현재 YOLOv8n + 현재 Native `.so` 조합은 더 이상 미확인 상태가 아니다.**

## 3. Native .so의 역할

`librknn_detector.so`는 전체 BSD 프로그램이 아니다.

역할:

```text
Python letterbox/preprocess
→ Native .so
→ RKNN C API
→ RK3576 NPU
→ YOLO output decode / NMS
→ [x1,y1,x2,y2,score,class]
→ Python
→ tracking / distance / TTC / risk / warning
```

따라서 ROI, 거리보정, TTC, 경보 로직, UART, 카메라 연결 같은 Python 측 변경 때문에 `.so`를 다시 만들 필요는 없다.

재빌드가 필요한 경우는 주로 모델 tensor contract 변경, 입력 크기 변경, RKNN SDK/runtime ABI 변경, 다른 SoC 사용, Native C++ 수정이다.

현재 640×640 / 80-class / 9-output DFL 구조의 YOLOv8n 조합에서는 재빌드 필요 없음.

## 4. 아직 완료되지 않은 것

오늘 PASS는 **실카메라 최종 검증이 아니다.**

현재 실제 LEFT / RIGHT / REAR 카메라는 개발 보드에 물리적으로 연결되어 있지 않다.

따라서 아직 남아 있다.

- 실제 3채널 카메라 입력 연결/장치 확인
- 실제 카메라 기준 검출 정확도, 오검출/미검출
- 실제 설치 위치 기준 ROI/거리 calibration
- 거리/TTC 검증
- 실제 차량 경보 LED/buzzer/UART 검증
- 실제 카메라 입력에서 최종 FPS/CPU/NPU/VPU 확인
- 신규 보드 Factory 최신 설치 PASS
- Factory A→B 업데이트 PASS
- 재부팅 후 runtime profile 지속 확인
- 설치 실패/복구 경로 확인
- Golden Image/BSP 기준 확정

`yolov8n_rk3576_i8_test.rknn`의 `_test`는 그대로 유지한다.
실카메라/최종 데이터셋 합격 전에는 양산 최종 모델로 rename하지 않는다.

## 5. 내일 새 채팅에서 시작할 위치

먼저 PC에서 최신 코드 동기화:

```powershell
cd D:\AI\AI_project
git pull --ff-only
git status
```

새 채팅 첫 메시지는 다음처럼 시작하면 된다.

```text
docs/board/NEXT_SESSION_HANDOFF.md 읽고 이어서 하자.
어제 YOLOv8n + Native .so 실보드 검증은 PASS까지 끝냈다.
오늘은 남은 작업을 우선순위대로 진행하자.
```

### 우선순위

1. **실카메라가 준비됐다면:** 카메라 연결/영상 입력 확인부터 시작.
2. **실카메라가 아직 없다면:** 신규 보드가 있는 경우 Factory 최신 설치/A→B update/reboot 검증.
3. **둘 다 아직 없다면:** Factory requirements의 `scipy` 직접 pin 여부와 UART용 `pyserial` 도입 조건 정리, Manager/Factory Release 로컬 빌드 회귀검사.

## 6. 중요한 현재 상태

### AI_Camera Manager

개발자용 Windows GUI.

현재 기능:

- Hardware Diagnostic
- Environment Audit
- Dual File Manager
- Dual Terminals
- PC Setup / WSL
- Native `.so` build/stage
- Factory Release build
- **YOLOv8 + Native on Board 검증**

YOLOv8 + Native 검증 버튼은 실제 Windows Manager + 개발 보드에서 PASS 확인됨.

### Factory Installer

공장용 별도 도구.

현재 최신 Factory Release 빌더의 기본 모델은 YOLOv8n.

단, 최신 코드 기준으로 다음은 아직 신규 보드에서 실제 검증 전:

- 신규 설치
- A→B 업데이트
- reboot persistence
- 실패/복구

### WSL

일반 실행/Factory 설치에는 필요 없다.

**Native `.so`를 새로 컴파일할 때만 필요**하다.

## 7. 기준 문서

보드 절차의 canonical 문서:

```text
docs/board/README.md
```

새로운 보드 운영 절차/TODO는 이 문서에 누적한다.

이 인수인계 파일은 새 채팅 시작용 요약이며, 상세 기록의 기준은 `docs/board/README.md`다.

## 8. 오늘 결론

오늘 목표였던 다음 항목은 완료:

```text
YOLO11 → YOLOv8 기준 정리
YOLOv8 모델 GitHub 등록
앱 기본 RK3576 INT8 → YOLOv8
Factory 기본 모델 → YOLOv8
모델 SHA-256 추적
Native .so + YOLOv8 실제 보드 RKNN inference PASS
Manager 원클릭 검증 버튼 PASS
문서 업데이트
```

내일부터는 **모델/Native 엔진 자체보다 실제 입력/차량/신규 보드 검증 단계**로 넘어간다.
