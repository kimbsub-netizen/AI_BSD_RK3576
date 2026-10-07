# Side BSD 운영·배포 가이드

> Status: CURRENT  
> Document ID: SBSD-OPS-001
> Revision: v1.1
> Approval: SELF-REVIEWED
> Applies to: Side BSD  
> Updated: 2026-09-13
> Baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`  
> Software regression: Compile PASS / Repository 169/169 PASS

## 1. 목적

Lab 녹화영상, Rockchip 보드, 실제 차량 배포 단계에서 사용하는 운영 원칙과 설정 전환 기준을 정리한다.

관련 문서:

```text
../03_calibration/Side_BSD_캘리브레이션_가이드.md
../00_index/CURRENT_VALIDATION_ROADMAP.md
../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md
```

## 2. 운영 원칙

1. ROI/거리/Ego Motion 등 실측값은 `config_calibration.py`에서 관리한다.
2. TTC/경보/성능 threshold는 `config_logic.py`에서 관리하되 변경 근거를 남긴다.
3. Forward 최신 정책은 `ROI + Distance severity-max`, `TTC 2.5s escalation-only`이며, Distance가 30m를 초과해도 WARNING ROI 안이면 ROI fail-safe를 유지한다.
4. TEST/측정용 옵션은 차량 배포 전에 Production 정책으로 전환한다.
5. 실제 보드/차량 evidence가 없는 항목을 배포 완료로 표시하지 않는다.

## 3. 단계별 운영 모드

| 항목 | Lab recorded-video | Rockchip board | Final vehicle |
|---|---|---|---|
| `OPERATION_MODE` | `TEST` | `PRODUCTION` | `PRODUCTION` |
| 화면/ROI/FPS debug | 필요 시 ON | 성능 측정 시 최소화 | OFF |
| 상세 log | ON 권장 | ON 권장 | 승인된 진단정책 |
| `PERF_MONITOR_ENABLED` | 선택 | ON 권장 | 승인 정책 |
| `REAR_ALWAYS_ON` | replay 목적 시 가능 | 실제 Reverse 입력 우선 | `False` |
| `REVERSE_ASSIST_CALIBRATED` | evidence 전 `False` | 실측 완료 후 검토 | final calibration 완료 시 `True` |
| `TEST_REVERSE_GEAR_OVERRIDE` | TEST에서만 사용 가능 | `None` | `None` |
| `EGO_MOTION_BSD_GATE` | 현재 `False` | calibration 후 검증 | 검증 결과에 따라 승인 |
| YOLO model | PC 모델 가능 | 검증된 RKNN | 검증된 RKNN |

실제 옵션명과 default는 배포 직전 현재 source를 다시 확인한다.

## 4. Forward 배포 기준

배포 전 다음 정책이 변하지 않았는지 확인한다.

```text
DANGER ROI
= 차량 측면 위험구간 + rear 0~10m fail-safe

WARNING ROI
= DANGER ROI 포함 + rear 10~30m outer zone

Distance
Distance layer: <=10m DANGER
Distance layer: <=30m WARNING
Distance layer: >30m SAFE
Final alert: >30m이면서 WARNING ROI 밖인 경우 SAFE

base_level = max(roi_level, distance_level)

SAV TTC threshold = 2.5s
TTC = escalation-only
10~30m WARNING + TTC<=2.5s → DANGER
>30m TTC early-warning 없음
```

`IGNORE_AWAY_*`, `INCOMING_FILTER_*`, `FAST_APPROACH_*` legacy/diagnostic 값이 base warning suppression으로 다시 연결되지 않았는지 코드/테스트에서 확인한다.

## 5. Lab 단계

Lab에서는 설정을 확정하기보다 **evidence를 만들고 후보값을 정리**한다.

필수 결과:

```text
Forward ROI coordinates
Forward Distance calibration candidate
ROI + Distance replay result
TTC replay result
Ego Motion threshold candidate
Reverse Side / Rear calibration candidate
Lab Validation Summary
```

Lab에서 CODE_GAP을 발견하면 사용자 승인 없이 Production code를 변경하지 않는다.

## 6. Rockchip 보드 단계

보드에서 다음을 확인한다.

```text
AHD decoder / MIPI CSI
LEFT/RIGHT/REAR capture
RKNN accuracy
completed FPS / latency
CPU/NPU/memory/thermal
long-run stability
actual Turn Signal / Reverse
Display / LED / Buzzer
Fault / Unavailable indication
```

PC Lab의 calibration/정책이 보드에서 그대로 재현되는지 먼저 비교한다.

## 7. 차량 배포 전 필수 확인

```text
[ ] final camera mounting 고정
[ ] LEFT/RIGHT/REAR calibration final
[ ] Forward DANGER/WARNING ROI final
[ ] Forward 10m/30m actual validation
[ ] Reverse Side 1.5m/3m final
[ ] Rear 1.5m/3m final
[ ] Ego Motion final evidence
[ ] actual Turn Signal / Reverse input
[ ] Display / LED / Buzzer
[ ] FP/FN / environment / endurance
[ ] `REAR_ALWAYS_ON=False`
[ ] `TEST_REVERSE_GEAR_OVERRIDE=None`
[ ] debug display production 설정
[ ] validated RKNN model
```

## 8. Calibration 기록

차량/공통 baseline마다 최소 다음을 보존한다.

```text
vehicle or baseline ID
camera model
mount position / height / angle
resolution / flip
source images/videos
rear trailing-edge reference
ROI coordinates
Distance calibration
Ego Motion values
software commit
config revision
validation summary
date / operator
```

## 9. 로그/결과 보존

설정값을 바꿀 때는 다음을 함께 남긴다.

```text
change reason
evidence/log/video
before value
after value
expected effect
validation result
```

공통 logic threshold와 vehicle-specific calibration을 혼합 관리하지 않는다.

## 10. 변경관리

```text
CONFIG_GAP
    → calibration/config 수정 후 replay

CODE_GAP
    → Requirement / Expected / Actual / Impact / Minimal Fix / Regression Plan
    → 사용자 승인
    → Production 수정

BOARD_GAP
    → board/integration 단계에서 처리

VEHICLE_CALIBRATION_GAP
    → final vehicle calibration 재수행
```

## 11. ISO 상태

현재 Type III/SAV Software Baseline 완료는 ISO 17387:2026 full compliance 선언이 아니다.

licensed full-text 및 실제 conformity evidence가 완료되기 전에는 제품문서에서 full compliance를 주장하지 않는다.
