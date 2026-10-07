# Side BSD 시스템 Validation 계획 및 절차서

> Status: CURRENT  
> Document ID: SBSD-VAL-PLAN-001
> Revision: v1.1
> Approval: SELF-REVIEWED
> Applies to: Side BSD  
> Updated: 2026-09-13  
> Baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`  
> Software regression: Compile PASS / Repository 169/169 PASS

## 1. 목적

Software regression 이후 녹화영상, Rockchip 보드, 실제 RV 단계에서 Side BSD의 ROI, 거리, TTC, Reverse Assist, 성능 및 I/O를 검증한다.

상세 순서:

```text
../00_index/CURRENT_VALIDATION_ROADMAP.md
```

Calibration:

```text
../03_calibration/Side_BSD_캘리브레이션_가이드.md
```

## 2. Forward 기준

```text
DANGER ROI
= 긴 RV 차량 측면 위험구간 + rear 0~10m fail-safe

WARNING ROI
= DANGER ROI 포함 + rear 10~30m outer evaluation zone

rear trailing-edge distance
<=10m  DANGER
<=30m  WARNING
>30m   SAFE (정상 calibration에서 WARNING ROI 밖)

base_level = max(roi_level, distance_level)

SAV TTC threshold = 2.5s
10~30m WARNING + TTC<=2.5s → DANGER
TTC/velocity로 base warning downgrade 금지
>30m TTC early-warning 없음
```

## 3. Lab 입력 데이터

각 영상에 최소 다음을 기록한다.

```text
scenario ID
camera side
mount/camera information
reference distance or physical zone
reference timestamp
target behavior
expected severity
Turn Signal / Reverse test state
software commit
config revision
```

Ground Truth가 없는 영상은 정성 관찰 자료로만 사용한다.

## 4. Forward ROI 검증

```text
[ ] DANGER ROI가 차량 측면 위험구간 포함
[ ] DANGER ROI가 실제 rear 0~10m 포함
[ ] WARNING ROI가 DANGER ROI 전체 포함
[ ] WARNING ROI가 실제 rear 30m까지 포함
[ ] 실제 30m 밖 대상이 정상 설정에서 WARNING ROI 밖
[ ] LEFT/RIGHT 독립 검증
[ ] 불필요한 차체/미러/하늘/외부영역 제외
```

## 5. Forward Distance 검증

필수 경계:

```text
10m
30m
10m 주변점
30m 주변점
```

가능하면 inner / center / outer lateral 위치를 확인한다.

기록:

```text
reference distance
estimated distance
error
lateral position
extrapolation
fit/RMSE
```

## 6. ROI + Distance 이중 방어 검증

현재 Production은 다음 원칙을 사용한다.

```text
base_level = max(roi_level, distance_level)
```

필수 확인:

```text
[ ] 실제 DANGER zone 대상에서 거리추정이 크게 잡혀도 DANGER 유지
[ ] 실제 <=10m 대상에서 ROI가 WARNING으로만 잡혀도 Distance가 DANGER 유지
[ ] 정상 10~30m 대상 → WARNING
[ ] 실제 >30m + WARNING ROI 밖 → SAFE
```

ROI와 Distance의 역할은 서로 보완적이며, 더 높은 severity가 유지되어야 한다.

## 7. Velocity / TTC 검증

과거 `멀어지는 대상이면 base warning 억제`, `Fast-path가 별도 사용자 경고경로`라는 표현은 현재 정책이 아니다.

현재 요구:

```text
10~30m + velocity UNKNOWN → WARNING 유지
10~30m + same speed       → WARNING 유지
10~30m + receding         → WARNING 유지
10~30m + TTC >2.5s        → WARNING 유지
10~30m + TTC <=2.5s       → DANGER
0~10m + TTC invalid/large → DANGER 유지
>30m                       → TTC early-warning 없음
```

로그:

```text
frame/timestamp
tracker_id
reference distance
estimated distance
estimated relative velocity
estimated TTC
ROI level
Distance level
Final level
```

30m 진입 후 유효 velocity/TTC가 만들어지기까지의 시간도 기록한다.

## 8. Turn Signal / HMI Software Replay

```text
[ ] same-side WARNING + Turn Signal → DANGER
[ ] opposite-side Turn Signal → 해당 side에 영향 없음
[ ] TTC DANGER와 Turn Signal 동시조건
[ ] LEFT/RIGHT simultaneous alert
[ ] rear 화면 표시 중 side OSD 유지
```

Visual severity와 buzzer 정책은 구분한다. 실제 물리 출력은 보드 단계에서 확인한다.

## 9. Ego Motion

현재 `EGO_MOTION_BSD_GATE=False`다.

수집 상태:

```text
STOPPED
LOW_SPEED
MOVING
HIGHWAY
TURN
low-texture/tunnel
```

LEFT/RIGHT 차이, threshold separation, debounce, 순간 flow 손실을 확인한다.

## 10. Reverse Assist

```text
Reverse Side DANGER  <=1.5m
Reverse Side WARNING <=3.0m
Rear DANGER          <=1.5m
Rear WARNING         <=3.0m
all model classes
Forward Velocity/TTC 정책 미사용
Turn Signal escalation 미사용
```

확인:

```text
[ ] LEFT/RIGHT reverse ROI
[ ] LEFT/RIGHT reverse distance
[ ] REAR ROI/distance
[ ] 1.5m / 3.0m 경계
[ ] 정지 장애물
[ ] 다양한 검출 class
[ ] LEFT/RIGHT 동시 위험
[ ] R ON/OFF session reset
[ ] rear 화면 고정 + side OSD
```

실측 전 `REVERSE_ASSIST_CALIBRATED=False`를 유지한다.

## 11. 채널 상태 / 복구 확인

실제 영상/보드에서는 다음을 확인한다.

- LEFT/RIGHT 한 채널 입력 중단 시 다른 채널 지속
- stale 결과가 경보로 재사용되지 않음
- 정상 입력 복귀 후 fresh 결과로 상태 복구
- Rear worker timeout/exception 처리
- R 세션 전환 후 이전 결과가 새 세션에 섞이지 않음

전체 보드 전원/공통자원 문제는 보드/시스템 단계에서 별도로 검증한다.

## 12. Rockchip 보드 성능

보드 도착 후:

```text
LEFT/RIGHT/REAR capture FPS
channel completed inference FPS
p95 inference latency
scheduler selection gap
CPU/NPU/memory
thermal/endurance
AHD decoder / MIPI CSI stability
camera reconnect
```

PC software regression PASS만으로 보드 성능 PASS 처리하지 않는다.

## 13. 실제 차량

최종 RV에서:

- final mounting
- final ROI/Distance calibration
- Forward 10m/30m physical boundary
- Reverse 1.5m/3.0m
- same-speed adjacent target
- closing/TTC
- actual Turn Signal/Reverse
- Display/LED/Buzzer
- FP/FN
- night/rain/tunnel/glare/vibration
- temperature/power/endurance

을 검증한다.

## 14. 결과 상태

```text
PASS
FAIL
CONFIG_GAP
CODE_GAP
NEEDS_BOARD
NEEDS_VEHICLE
NOT_TESTED
```

CODE_GAP 발견 시 먼저 다음을 기록한다.

```text
Requirement
Expected
Actual
Impact
Minimal fix
Regression plan
```

사용자 승인 후 Production 변경을 수행한다.

## 15. Lab Closure 체크리스트

```text
[ ] Ground Truth dataset
[ ] Forward ROI physical boundary
[ ] Forward Distance validation
[ ] ROI + Distance replay
[ ] Velocity/TTC 2.5s replay
[ ] Turn Signal replay
[ ] Ego Motion evidence
[ ] Reverse Assist replay
[ ] known FP/FN 정리
[ ] config candidate 정리
[ ] CODE_GAP 여부 판정
[ ] Lab Validation Summary
```

`PASS_LAB`은 최종 차량 인수 PASS가 아니다. Lab 결과는 다음 상태로 기록한다.

```text
PASS_LAB
CONFIG_GAP
CODE_GAP
NEEDS_BOARD
NEEDS_VEHICLE
```

## 16. Board 완료 및 인수 판정기준

실제 Rockchip 보드에서 다음 항목의 evidence를 확보해야 한다.

```text
[ ] LEFT/RIGHT/REAR 3-channel camera input
[ ] AHD decoder / MIPI CSI 안정성
[ ] RKNN/NPU inference
[ ] completed FPS / p95 latency
[ ] CPU/NPU/memory/thermal
[ ] 장시간 안정성 및 camera reconnect
[ ] 실제 Turn Signal / Reverse input
[ ] Display / LED / Buzzer
[ ] Fault / Unavailable indication
```

Lab에서 확정한 경고정책이 보드에서도 동일하게 재현되어야 하며, PC 또는 Mock 자동시험
PASS만으로 Board 완료를 판정하지 않는다.

## 17. Vehicle 완료 및 최종 인수 판정기준

최종 RV에서 다음 항목의 evidence를 확보해야 한다.

```text
[ ] final mounting 및 LEFT/RIGHT/REAR calibration
[ ] Forward 10m/30m physical boundary
[ ] Reverse 1.5m/3m physical boundary
[ ] same-speed adjacent target
[ ] 0~10m / 10~30m / >30m scenarios
[ ] closing target / TTC 2.5s boundary
[ ] 실제 Turn Signal / Reverse input
[ ] physical Display / LED / Buzzer
[ ] False Positive / False Negative
[ ] 환경조건 및 endurance
```

Reverse-specific Calibration evidence와 실제 물리경계 검증이 없으면 Reverse Assist를
최종 완료로 판정하지 않는다.

## 18. Validation 결과 기록

각 결과에는 최소 다음 정보를 기록한다.

```text
Test ID
Stage: LAB / BOARD / VEHICLE
Date
Source commit
Config / Calibration revision
Input data / Ground Truth
Expected
Actual
Result
Related log / image / video
Follow-up
```

문제는 최소 `CONFIG_GAP`, `CODE_GAP`, `BOARD_GAP`,
`VEHICLE_CALIBRATION_GAP`, `STANDARD_GAP`으로 구분한다. CODE_GAP은 요구사항,
Expected/Actual, 영향, 최소 수정안과 회귀 계획을 먼저 정리한 뒤 production 변경 여부를 결정한다.

## 19. 표준 적합성 판정 제한

현재 Type III/SAV Software implementation과 자동시험 완료는
`ISO 17387:2026 full compliance` 선언이 아니다. Licensed full-text 검토와 필요한 실제
Validation evidence가 완료되기 전에는 full conformity를 주장하지 않는다.
