# Side BSD CODEBASE OVERVIEW

> Document ID: SBSD-CODE-001
> Revision: v1.1
> Status: CURRENT
> Approval: SELF-REVIEWED
> Applies to: Side BSD
> Last verified: 2026-09-13
> Software baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`

## 1. 역할과 범위

이 문서는 현재 Side BSD 코드의 진입점, 의존관계, 데이터 흐름과 동시성 경계를 찾기 위한 코드
지도다. 제품 정책은 SRS, 알고리즘 설명은 시스템 기술설계서를 우선하며 여기서 반복하지 않는다.

## 2. 실행 및 설정

```text
run_side_bsd.py
└─ src/apps/side_bsd/app_side_bsd.py
   ├─ config/config_env.py
   ├─ config/config_calibration.py
   ├─ config/config_logic.py
   └─ display_layout_config.py
```

- `config_env.py`: 실행환경, 모델 및 장치 경로
- `config_calibration.py`: 차량·카메라별 ROI/거리/Ego Motion 보정값
- `config_logic.py`: 경보, TTC, scheduler 및 상태정책 threshold

설정값은 `config → import → 생성자/함수 인자 → 실제 사용` 경로를 끝까지 추적한다.

## 3. 주요 모듈

| 모듈 | 책임 |
|---|---|
| `src/modules/side_camera.py` | LEFT/RIGHT detection, tracking, ROI/distance/TTC, Forward/Reverse 판단 |
| `src/modules/side_frame_scheduler.py` | LEFT/RIGHT 추론 순서와 starvation 제어 |
| `src/modules/rear_camera.py` | REAR detection 및 Reverse Assist 결과 |
| `src/modules/rear_inference_supervisor.py` | Rear worker process, timeout/restart/session 격리 |
| `src/modules/ttc_estimator.py` | 거리 이력 기반 TTC 계산 |
| `src/shared/distance_calibrator.py` | y-coordinate 기반 거리 보정 |
| `src/shared/velocity_tracker.py` | tracker별 거리 변화와 상대속도 |
| `src/shared/ego_speed_estimator.py` | Optical Flow 기반 자차 운동상태 후보 |
| `src/shared/hardware_io.py` | 차량 입력 및 출력 추상화 |

## 4. Forward LEFT/RIGHT 흐름

```text
frame
→ SideFrameScheduler
→ SideCamera.infer()
→ model detection
→ ByteTrack
→ object foot-point
→ ROI + Distance
→ VelocityTracker / TTCEstimator
→ Turn Signal escalation
→ AlertStateMachine / HMI state
```

## 5. Reverse 흐름

```text
Reverse input edge
→ SideBSDSystem._set_side_mode()
→ SideCamera.transition_mode()
→ cache / tracker / velocity / alert / scheduler reset
→ LEFT + RIGHT Reverse Assist
→ RearInferenceSupervisor / RearCamera
→ REAR 화면 + 방향별 OSD 및 경보
```

Forward와 Reverse는 서로 다른 ROI, 거리 보정과 정책을 사용한다.

## 6. Thread / Process 경계

- 메인 루프가 입력, mode, scheduler 및 HMI를 조정한다.
- LEFT/RIGHT는 scheduler 정책으로 추론 기회를 배분한다.
- REAR 추론은 `RearInferenceSupervisor`의 별도 process가 NPU/model context를 소유한다.
- Rear outstanding request는 1개로 제한하고 timeout 시 worker를 재시작한다.
- session id로 이전 Reverse 세션의 늦은 결과를 폐기한다.

## 7. Cache / Stale / Fault 경계

- 추론하지 않은 cycle은 유효기간 안의 cache만 사용한다.
- stale 또는 invalid 결과를 정상 SAFE로 해석하지 않는다.
- mode 전환 시 이전 mode의 detection, tracking, alert와 scheduler 상태를 폐기한다.
- 한 채널 fault가 가능한 범위에서 다른 정상 채널과 메인 루프를 중단시키지 않도록 격리한다.

## 8. TEST / PRODUCTION

- `OPERATION_MODE=TEST`는 측정과 직접 ROI 확인을 위한 별도 동작을 포함할 수 있다.
- 차량 배포 전 `PRODUCTION` 설정, 모델 경로와 Calibration artifact를 확인한다.
- TEST 결과를 실제 Production HMI 또는 차량 안전성 검증으로 확대 해석하지 않는다.

## 9. 자동시험 위치

```text
tests/side_bsd/  # Side BSD 정책, 상태, fault 및 production-path 회귀
tests/common/    # 공유 수학·계약과 배포/진단 지원 도구
```

실행 결과와 논리검증 기록은 `../../../verification/sidebsd/current/`에서 관리한다.

## 10. Hardware 경계

`src/shared/hardware_io.py`의 인터페이스 존재는 실제 Rockchip 보드 연동 완료를 의미하지 않는다.
AHD/MIPI/V4L2, RKNN/NPU, GPIO, Display, LED 및 Buzzer는 Board/Vehicle Validation이 필요하다.

## 11. 문서 우선순위

```text
현재 동작      : Production source
요구 동작      : ../01_requirements/Side_BSD_시스템_요구사항_명세서.md
설계 설명      : Side_BSD_시스템_기술_설계서.md
현재 구현 요약 : ../00_index/CURRENT_IMPLEMENTATION_BASELINE.md
검증 근거      : ../../../verification/sidebsd/current/
과거 근거      : ../history/ 및 ../../../verification/history/
```

과거 Type III/SAV 요구사항 추가서는 SRS v2.0에 통합됐으며 현행 규범으로 사용하지 않는다.
