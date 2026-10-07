# Side BSD 캘리브레이션 가이드

> Status: CURRENT  
> Document ID: SBSD-CAL-001
> Revision: v1.1
> Approval: SELF-REVIEWED
> Applies to: Side BSD  
> Updated: 2026-09-13
> Baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`

## 1. 목적

Forward/Reverse Side BSD의 거리, ROI, Ego Motion 캘리브레이션 기준을 최신 Production 정책에 맞춰 정리한다.

## 2. 공통 원칙

- 차량/카메라 실측값은 `config_calibration.py`에서 관리한다.
- LEFT/RIGHT/REAR 값을 서로 복사하지 않는다.
- 장착 위치·높이·각도·해상도·반전이 바뀌면 관련 calibration을 다시 확인한다.
- Forward rear distance 0m 기준은 **subject vehicle rear trailing-edge plane**이다.
- 카메라 optical center를 Forward 0m 기준으로 사용하지 않는다.

## 3. Forward Distance Calibration

도구:

```text
tools/tool_distance_calibrator.py
```

권장 rear marker:

```text
3 / 5 / 6 / 7 / 8 / 9 / 10 / 15 / 20 / 30m
```

필수 확인점:

```text
10m
30m
10m 주변점
30m 주변점
```

가능하면 각 주요 거리에서 `inner / center / outer` lateral 위치를 확인한다.

현재 `DistanceCalibrator`는 object foot-point의 `y_norm` 기반 1D 모델이므로 lateral 위치별 오차를 별도로 기록한다.

## 4. Forward ROI Calibration

도구:

```text
tools/tool_roi_tuner_v2.py
```

최종 의미:

```text
DANGER ROI
= 긴 RV 차량 측면 위험구간
+ rear trailing-edge 뒤 0~10m fail-safe

WARNING ROI
= DANGER ROI 전체 포함
+ rear 10~30m outer evaluation zone

DANGER ROI ⊂ WARNING ROI
```

작업 기준:

- DANGER ROI가 차량 측면 위험구간과 실제 0~10m를 포함해야 한다.
- WARNING ROI가 DANGER ROI 전체와 실제 30m까지 포함해야 한다.
- 정상 설정에서 실제 30m 밖 대상은 WARNING ROI 밖이 되도록 한다.
- 차체, 미러, 하늘, 불필요한 외부영역은 제외한다.
- LEFT/RIGHT는 각각 설정한다.

## 5. ROI + Distance 이중 방어

Production 기본 판정:

```text
roi_level:
    DANGER ROI  → DANGER
    WARNING ROI → WARNING

distance_level:
    <=10m  → DANGER
    <=30m  → WARNING
    >30m   → SAFE

base_level = max(roi_level, distance_level)
```

따라서 ROI와 Distance는 서로 보완적으로 검증한다.

필수 cross-check:

```text
[ ] 실제 10m와 DANGER ROI 경계 정합
[ ] 실제 30m와 WARNING ROI 경계 정합
[ ] DANGER ROI ⊂ WARNING ROI
[ ] 실제 차량 측면 hazard가 DANGER ROI에 포함
[ ] 실제 30m 밖 대상이 WARNING ROI 밖
```

## 6. TTC 검증 선행조건

현재:

```text
SAV TTC threshold = 2.5s
TTC = escalation-only
```

TTC replay 전에 10~30m 거리 보정, tracker continuity, timestamp 신뢰성을 먼저 확인한다.

TTC는 기존 DANGER/WARNING을 낮추지 않는다.

## 7. Ego Motion

현재 기본값:

```text
STOP     = 2.0
MOVE     = 4.0
HIGHWAY  = 12.0
```

현재 `EGO_MOTION_BSD_GATE=False`다.

보드/차량 적용 전 다음 LEFT/RIGHT pair 영상을 확보한다.

```text
STOPPED
LOW_SPEED
MOVING
HIGHWAY
TURN
low-texture/tunnel
```

## 8. Reverse Side Calibration

Forward calibration을 그대로 재사용하지 않는다.

```text
Reverse Side DANGER  <=1.5m
Reverse Side WARNING <=3.0m
```

권장 marker:

```text
0.5 / 0.8 / 1.0 / 1.5 / 2.0 / 2.5 / 3.0m
```

실측 evidence 전 `REVERSE_ASSIST_CALIBRATED=False`를 유지한다.

## 9. Rear Calibration

```text
Rear DANGER  <=1.5m
Rear WARNING <=3.0m
```

권장 marker:

```text
0.5 / 0.8 / 1.0 / 1.5 / 2.0 / 2.5 / 3.0 / 4.0 / 6.0 / 10.0m
```

## 10. Evidence 기록

```text
vehicle/baseline ID
camera model
mount position/height/angle
resolution/flip
rear trailing-edge reference
marker layout
source image/video
calibration output
validation result
software commit
config revision
date/operator
```

## 11. 다음 단계

```text
ROI physical boundary
→ Distance validation
→ ROI + Distance replay
→ Velocity/TTC replay
→ Ego Motion
→ Reverse Assist
```

상세 작업:

```text
../00_index/CURRENT_VALIDATION_ROADMAP.md
../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md
```
