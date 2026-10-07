# Side BSD 인수시험 가이드

> Status: ARCHIVED — 시스템 Validation 계획 및 절차서에 통합됨  
> Applies to: Side BSD  
> Updated: 2026-09-11  
> Baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`  
> Software regression: Compile PASS / Repository 169/169 PASS

> 이력 안내: 본 문서의 Lab/Board/Vehicle 완료조건과 결과 기록 기준은 2026-09-13에
> `../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md`로 통합되었다.
> 현재 Validation 및 인수 판정에는 통합 절차서를 사용한다.

## 1. 목적

Side BSD의 완료 판정을 `Lab → Board → Vehicle` 단계로 분리한다.

현재는 실제 보드가 없으므로 **Lab 녹화영상 검증 단계**다.

상세 순서는 다음 문서를 따른다.

```text
Side_BSD_남은_작업_로드맵.md
Side_BSD_시스템_검증_가이드.md
```

## 2. Forward 최신 기준

```text
DANGER ROI
= 차량 측면 위험구간 + rear 0~10m fail-safe

WARNING ROI
= DANGER ROI 포함 + rear 10~30m outer zone

Distance
<=10m DANGER
<=30m WARNING
>30m SAFE

base_level = max(roi_level, distance_level)

TTC threshold = 2.5s
10~30m WARNING + TTC<=2.5s → DANGER
TTC/velocity로 base warning downgrade 금지
>30m TTC early-warning 없음
```

## 3. Lab 완료조건

```text
[ ] Ground Truth dataset 준비
[ ] Forward DANGER/WARNING ROI 물리경계 확인
[ ] Forward 10m/30m Distance validation
[ ] ROI + Distance 이중 방어 replay
[ ] velocity UNKNOWN/same-speed/receding no-downgrade
[ ] TTC 2.5s escalation replay
[ ] 실제 >30m no-warning replay
[ ] Turn Signal software policy replay
[ ] Ego Motion calibration evidence
[ ] Reverse Side 1.5m/3m replay
[ ] Rear 1.5m/3m replay
[ ] R ON/OFF state reset replay
[ ] Lab Validation Summary 작성
```

Lab 결과는 다음으로 구분한다.

```text
PASS_LAB
CONFIG_GAP
CODE_GAP
NEEDS_BOARD
NEEDS_VEHICLE
```

`PASS_LAB`은 최종 차량 인수 PASS가 아니다.

## 4. Board 완료조건

Rockchip 보드 도착 후 다음 범주를 검증한다.

```text
3-channel camera input
AHD decoder / MIPI CSI
RKNN/NPU inference
completed FPS / latency
CPU/NPU/memory/thermal
long-run stability
actual Turn Signal / Reverse input
Display / LED / Buzzer
Fault / Unavailable indication
```

Lab에서 확정한 경고정책이 보드에서도 동일하게 재현되어야 한다.

## 5. Vehicle 완료조건

최종 RV에서 다음을 확인한다.

```text
final mounting
final LEFT/RIGHT/REAR calibration
Forward 10m/30m physical boundary
Reverse 1.5m/3m boundary
same-speed adjacent target
0~10m / 10~30m / >30m scenarios
closing/TTC
actual Turn Signal / Reverse
physical HMI
FP/FN
environment / endurance
```

## 6. Reverse Assist 기준

```text
Reverse Side DANGER  <=1.5m
Reverse Side WARNING <=3.0m
Rear DANGER          <=1.5m
Rear WARNING         <=3.0m
```

실제 reverse-specific calibration evidence 확보 전에는 최종 완료로 판정하지 않는다.

## 7. 자동 Software 검증의 의미

현재:

```text
Compile          : PASS
Repository tests : 169/169 PASS
```

이는 Production software policy/logic regression evidence다.

실제 ROI/거리/TTC 정확도, 보드성능, 물리 I/O, FP/FN, 환경성능을 대신하지 않는다.

## 8. 결과 기록

```text
Test ID
Stage: LAB / BOARD / VEHICLE
Date
Source commit
Config revision
Input data / Ground Truth
Expected
Actual
Result
Related log/video
Follow-up
```

## 9. 문제 분류

```text
CONFIG_GAP
CODE_GAP
BOARD_GAP
VEHICLE_CALIBRATION_GAP
STANDARD_GAP
```

CODE_GAP은 요구사항/Expected/Actual/Impact/Minimal Fix/Regression Plan을 먼저 정리한 뒤 사용자 승인 후 Production을 변경한다.

## 10. ISO 상태

현재 Type III/SAV Software implementation 완료는 `ISO 17387:2026 full compliance` 선언이 아니다.

licensed full-text 검토와 실제 test evidence가 완료되기 전에는 full conformity를 주장하지 않는다.
