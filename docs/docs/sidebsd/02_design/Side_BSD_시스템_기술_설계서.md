# Side BSD 시스템 기술 설계서

> Status: CURRENT  
> Document ID: SBSD-DES-001
> Revision: v1.1
> Approval: SELF-REVIEWED
> Applies to: Side BSD  
> Last verified: 2026-09-13
> Current software baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`  
> Automated regression: Compile PASS / Repository 169/169 PASS

## 1. 문서 목적

이 문서는 현재 구현된 Side BSD 시스템의 아키텍처, 데이터 흐름, 모듈 책임과 런타임 정책을 설명한다.

현재 실제 동작은 소스코드를 우선하고, 요구 동작은 Forward Type III/SAV 요구사항을 통합한 단일 SRS를 적용한다. 과거 요구사항 추가서는 변경 근거 확인용 history이며 현행 규범이 아니다.

## 2. 시스템 구성

```text
LEFT   ┐
       ├─ Forward Side BSD / Reverse Assist
RIGHT  ┘

REAR   ── Rear Parking / Reverse Assist
```

실행 경로:

```text
run_side_bsd.py
→ src/apps/side_bsd/app_side_bsd.py
   ├─ modules/side_camera.py
   ├─ modules/ttc_estimator.py
   ├─ modules/side_frame_scheduler.py
   ├─ modules/rear_camera.py
   ├─ modules/rear_inference_supervisor.py
   └─ shared/*
```

## 3. 주요 모듈 책임

| 모듈 | 책임 |
|---|---|
| `app_side_bsd.py` | 시스템 초기화, 입력 루프, 좌/우/후방 결과 조율, HMI/I/O |
| `side_camera.py` | Forward/Reverse Side policy, Detection, Tracking, ROI/Distance, Velocity/TTC, Alert |
| `ttc_estimator.py` | closing speed 기반 TTC 계산 |
| `side_frame_scheduler.py` | Forward LEFT/RIGHT 추론 자원 배분 |
| `rear_camera.py` | Rear Detection/Tracking/ROI/Distance/Alert |
| `rear_inference_supervisor.py` | Rear worker process 격리, submit/poll/timeout/restart |
| `distance_calibrator.py` | image Y → distance 추정 |
| `velocity_tracker.py` | tracker ID별 상대속도 추정 |
| `ego_speed_estimator.py` | per-camera Optical Flow |
| `ego_motion_fusion.py` | LEFT/RIGHT flow 융합 |
| `alert_state_machine.py` | alert confirm/hold/clear |
| `hardware_io.py` | 차량/보드 I/O abstraction |
| `performance_monitor.py` | inference/FPS 관찰 |

## 4. Forward Side BSD 처리 흐름

현재 Forward 설계는 `WARNING ROI`를 outer evaluation gate로 사용하고, 그 내부에서 ROI severity와 Distance severity를 독립적으로 계산한다.

```text
Camera Frame
→ YOLO Detection
→ ByteTrack
→ WARNING ROI outer gate
→ ROI severity
→ DistanceCalibrator severity
→ base_level = max(ROI, Distance)
→ VelocityTracker
→ TTCEstimator (10~30m WARNING escalation only)
→ Turn Signal escalation
→ AlertStateMachine
→ cache
→ OSD / Hardware Output
```

### 4.1 ROI 구조

```text
DANGER ROI ⊂ WARNING ROI

DANGER ROI
= 긴 RV 차량 측면 위험구간
+ rear trailing-edge 뒤 0~10m fail-safe

WARNING ROI
= Forward 평가 outer zone
= DANGER ROI 전체 포함 + rear 10~30m
```

정상 calibration에서 실제 >30m target는 WARNING ROI 밖으로 설정한다.

### 4.2 거리정책

Forward rear distance 0m 기준은 **subject vehicle rear trailing-edge plane**이다.

```text
DANGER_DIST_M  = 10.0
WARNING_DIST_M = 30.0
```

Distance severity:

```text
<=10m  → DANGER
<=30m  → WARNING
>30m   → SAFE
```

### 4.3 ROI + Distance fail-safe

```text
roi_level:
    DANGER ROI  → DANGER
    WARNING ROI → WARNING

distance_level:
    <=10m  → DANGER
    <=30m  → WARNING
    >30m   → SAFE

base_level = severity_max(roi_level, distance_level)
```

설계 의도는 DistanceCalibrator가 잘못된 값을 출력해도 DANGER ROI가 2차 방어선으로 작동하도록 하는 것이다.

### 4.4 Velocity / TTC

```text
SIDE_BSD_TTC_ENABLED = True
SIDE_BSD_SAV_TTC_THRESHOLD_S = 2.5
```

TTC는 **escalation-only**다.

```text
base DANGER                        → TTC/velocity로 downgrade 금지
base WARNING                       → TTC/velocity로 downgrade 금지
10~30m WARNING + valid TTC<=2.5s   → DANGER
TTC UNKNOWN                        → SAFE 근거가 아님
>30m                               → TTC early-warning 없음
```

따라서 과거 설계의 `Velocity filter → warning decision` 구조로 현재 Forward 동작을 설명하지 않는다.

`IGNORE_AWAY_*`, `INCOMING_FILTER_*`, `FAST_APPROACH_*` 설정은 legacy/diagnostic compatibility 성격이며 현재 0~30m base warning을 SAFE로 낮추는 제품정책이 아니다.

### 4.5 Turn Signal

```text
same-side WARNING + Turn Signal → DANGER
opposite-side Turn Signal       → 해당 side에 영향 없음
```

현재 Forward buzzer는 DANGER라고 항상 울리는 구조가 아니며 Turn Signal 및 `EGO_MOTION_BSD_GATE` 정책의 영향을 받는다. Visual severity와 buzzer trigger를 분리해서 해석한다.

## 5. SideCamera 모드 전환

```text
reverse=False                        reverse=True
FORWARD_BSD  ────────────────────→  REVERSE_ASSIST
     ↑                                     │
     └─────────────────────────────────────┘
                  reverse=False
```

mode edge는 다음 state의 session boundary다.

```text
detection cache
alert confirm/hold
ByteTrack IDs
VelocityTracker history
inference freshness
scheduler priority/boost/cooldown
```

후진 해제 첫 cycle은 LEFT/RIGHT를 새 Forward policy로 갱신한 뒤 scheduler로 복귀한다.

## 6. REVERSE_ASSIST 처리

```text
LEFT + RIGHT every input cycle
→ YOLO (all model classes)
→ ByteTrack
→ reverse-specific ROI
→ reverse-specific DistanceCalibrator
→ 1.5m DANGER / 3.0m WARNING
→ AlertStateMachine
→ directional OSD / LED / buzzer
```

Reverse에서는 Forward Velocity/TTC escalation, Turn Signal escalation, Forward Ego Motion buzzer gate를 적용하지 않는다.

현재 예측형 횡방향 RCTA를 주장하지 않는다. Reverse는 calibrated ROI/거리 기반 warning-only 보조 범위다.

## 7. Rear 처리

```text
Rear Frame
→ RearInferenceSupervisor.submit()
→ child multiprocessing.Process
→ YOLO + RearCamera.infer()
→ result queue
→ main process poll
→ session validation
→ cache update
→ OSD / buzzer / fault
```

후방 worker는 thread가 아니라 별도 process다.

현재 거리:

```text
REAR_DANGER_DIST_M  = 1.5
REAR_WARNING_DIST_M = 3.0
```

Rear에는 Forward Velocity/TTC 정책을 적용하지 않는다.

## 8. Tracking / Distance

LEFT/RIGHT/REAR 모두 ByteTrack을 사용한다.

Forward Side는 tracker ID를 Relative Velocity history에 사용한다.

`DistanceCalibrator`는 object foot-point의 `y_norm` 기반 1D 모델이며, 실제 10m/30m 정확도와 lateral bias는 녹화영상/실차 evidence로 확인해야 한다.

실차/랩 데이터가 없는 기본 calibration 값은 최종 양산값이 아니다.

## 9. Ego Motion

각 SideCamera에 `EgoSpeedEstimator`가 있고 LEFT/RIGHT flow를 `EgoMotionFusion`에서 융합한다.

```text
STOPPED
LOW_SPEED
MOVING
HIGHWAY
```

현재 기본 threshold:

```text
STOP     = 2.0
MOVE     = 4.0
HIGHWAY  = 12.0
```

현재 `EGO_MOTION_BSD_GATE=False`이며 Forward severity 결정 자체를 막는 필수 gate가 아니다.

## 10. Scheduler

`SideFrameScheduler`는 Forward에서 LEFT/RIGHT 추론 자원을 배분한다.

```text
CAMERA_FPS = 30
INFERENCE_FPS_PER_SIDE = 30
PRIORITY_BOOST_FRAMES = 3
PRIORITY_COOLDOWN_FRAMES = 5
```

Reverse에서는 LEFT/RIGHT를 매 cycle 모두 처리하고 scheduler state를 reset한다.

실제 Rockchip 3-channel performance는 보드 단계에서 검증한다.

## 11. Cache / Stale / Fault

`MAX_INFERENCE_RESULT_AGE_SEC`를 넘긴 결과는 alert에 사용하지 않는다.

mode transition에서 아직 fresh한 cache도 폐기한다.

채널 fault/unavailable은 정상 SAFE와 구분한다.

현재 software regression에는 stale/cache/fault isolation 경로가 포함된다.

## 12. TEST / PRODUCTION

### TEST

- recorded video replay
- validated timestamp 사용 가능
- `TEST_ROI_DIRECT_ALERT_MODE`
- `TEST_REVERSE_GEAR_OVERRIDE`

### PRODUCTION

- real-time clock
- TEST direct alert 비활성화
- TEST reverse override 무시
- actual HardwareIO input 사용

## 13. Hardware I/O

현재 interface abstraction은 존재하지만 다음 physical integration은 보드/차량 단계 evidence가 필요하다.

```text
Turn Signal
Reverse
Buzzer
LED
Fault / Unavailable
Display/HMI
```

## 14. Software Verification 상태

현재 baseline:

```text
Compile          : PASS
Repository tests : 169/169 PASS
```

Forward Type III/SAV Software Scope는 현재 CLOSED다.

## 15. 남은 Validation

```text
Lab recorded-video dataset
→ ROI physical mapping
→ Distance calibration
→ ROI+Distance fail-safe replay
→ Velocity/TTC replay
→ Ego Motion calibration
→ Reverse Assist replay
→ Lab Closure
→ Rockchip board / AHD / MIPI / NPU / HW I/O
→ Final RV validation
```

ISO 17387:2026 licensed full-text conformity 검토는 별도의 Standard evidence 작업이다.

## 16. 문서 동기화 규칙

```text
source change
→ production-path tests
→ requirement/RTM update if needed
→ product specification
→ technical design
→ codebase overview
→ validation / operation documents
```
