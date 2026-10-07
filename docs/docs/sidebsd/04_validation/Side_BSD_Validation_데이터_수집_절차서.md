# Side BSD Validation 데이터 수집 절차서

> Status: CURRENT  
> Document ID: SBSD-VAL-DATA-001
> Revision: v1.1
> Approval: SELF-REVIEWED
> Applies to: Side BSD  
> Updated: 2026-09-13  
> Software baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`

## 1. 목적

본 문서는 Side BSD의 Lab recorded-video 검증과 이후 보드/실차 검증에 필요한 영상·사진·Ground Truth 수집 방법을 **현장에서 실제로 수행하기 쉬운 형태**로 정리한다.

촬영의 기본 원칙은 시나리오별로 짧은 영상을 하나씩 따로 만드는 것이 아니다.

```text
현장
→ 필요한 기준점/실측정보를 준비
→ LEFT/RIGHT/REAR를 가능한 한 연속 RAW 영상으로 녹화
→ 필요한 상황을 한 세션 안에서 순서대로 수행

Lab
→ 긴 RAW 영상에서 필요한 구간을 잘라냄
→ ROI / Distance / TTC / Turn Signal / Ego Motion / Reverse 검증에 재사용
```

즉 **현장 촬영시간을 최소화하고, 시나리오 분리는 Lab에서 수행하는 것**을 기본 방식으로 한다.

---

## 2. 촬영 공통 원칙

- 카메라는 목표 장착 높이·각도·해상도·반전 상태로 고정한다.
- 장착조건이 바뀌면 관련 ROI/Distance calibration 유효성을 다시 검토한다.
- Forward rear distance 기준은 **RV rear trailing-edge plane = 0m**로 한다.
- 거리 기준점은 줄자 또는 레이저 거리계 등으로 실제 거리를 측정한다.
- 영상과 함께 최소한 `camera side / session ID / mounting condition / 실측거리 / 특이사항`을 기록한다.
- Ground Truth가 없는 일반 주행영상도 기능 replay에는 사용할 수 있으나, 정량 Calibration/Validation 증거와 구분한다.
- LEFT/RIGHT 동시 분석이 필요한 경우 가능하면 같은 주행의 pair로 녹화한다.
- 시나리오별로 녹화를 시작/정지할 필요가 없다. 하나의 긴 RAW 파일 안에 여러 상황이 포함되어도 된다.

---

## 3. Forward 현재 정책

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

TTC
10~30m + TTC<=2.5s → DANGER
>30m early-warning 없음
```

---

## 4. 현장 촬영 권장 순서 — 최소 작업 방식

현장에서는 아래 4개 Session을 기준으로 자료를 확보한다.

```text
Session A : 정지차량 Distance / ROI Calibration
Session B : 정지 RV + 접근 Target Vehicle TTC Ground Truth
Session C : 일반 주행 Continuous RAW
Session D : Reverse Assist Continuous RAW
```

Ego Motion 자료는 Session C 안에서 최대한 함께 확보하고, 부족한 조건만 추가 촬영한다.

---

# 5. Session A — 정지차량 Distance / ROI Calibration

## 5.1 차량 상태

```text
RV = 완전 정지
카메라 = 최종 적용 예정 위치/높이/각도로 고정
rear trailing-edge = 0m
```

## 5.2 거리 marker

LEFT/RIGHT 각각 다음 지점을 권장한다.

```text
3 / 5 / 6 / 7 / 8 / 9 / 10 / 15 / 20 / 30m
```

특히 필수:

```text
10m
30m
10m 주변점
30m 주변점
```

거리별로 라바콘/꼬깔콘 또는 식별 가능한 marker를 세우고 사진 또는 짧은 영상을 촬영한다.

가능하면 10m / 30m에서는:

```text
inner / center / outer lateral position
```

도 추가한다.

## 5.3 ROI 확인용 구도

한 번의 정적 촬영에서 가능하면 다음이 함께 보이도록 한다.

- 긴 RV 차량 측면 위험구간
- rear 0~10m
- rear 10~30m
- rear >30m 영역
- 차체/미러/인접차선 관계

이 자료를 Lab에서 `tool_roi_tuner_v2.py` 및 `tool_distance_calibrator.py`에 사용한다.

### 중요

라바콘 자체가 Production 검출 대상이라는 의미가 아니다.

Calibration에서는 **콘이 놓인 실제 지면 위치를 Ground Truth 기준점**으로 사용한다. 가능하면 10m / 30m 등 핵심점에는 실제 Person 또는 Vehicle을 추가로 배치한 영상도 확보하여 Production detection + distance 경로를 별도로 확인한다.

---

# 6. Session B — TTC Ground Truth 간이 시험

TTC Ground Truth를 일반 도로주행 중 정밀하게 만들려고 하면 현장 부담이 커진다.

따라서 1차 Lab 검증용 TTC 자료는 다음과 같은 **controlled pass** 방식으로 확보한다.

## 6.1 기본 구성

```text
RV = 정지
rear trailing-edge = 0m

30m    25m    20m    15m    10m               RV
 |      |      |      |      |                  |
marker marker marker marker marker              0m

Target Vehicle
→ RV 옆 차선에서 일정한 속도로 30m 밖부터 RV 옆까지 접근/통과
```

marker는 주행경로를 막지 않는 안전한 위치에 두고, 영상에서 각 거리 기준점을 식별할 수 있도록 한다.

## 6.2 Target Vehicle 속도

정확한 단일 속도를 제품 요구사항으로 강제하지 않는다.

현장 수행이 쉽다면 **약 30 km/h 일정속도 접근을 1차 권장 세트**로 사용할 수 있다.

30 km/h는 약 8.3 m/s이므로 일정속도를 가정하면 TTC 2.5s 경계가 약 20.8m 부근에 형성되어 현재 SAV 2.5s 정책의 WARNING→DANGER 전환을 관찰하기 좋다.

가능하면 추가 세트:

```text
느린 접근
기준 접근 (예: 약 30 km/h)
빠른 접근
```

정도로만 확보한다. 현장에서 정확히 TTC=2.5s를 맞추려고 반복할 필요는 없다.

## 6.3 Lab에서 Ground Truth 생성

영상에서 각 marker 통과 프레임의 timestamp를 읽는다.

예:

```text
30m 통과 = t30
25m 통과 = t25
20m 통과 = t20
15m 통과 = t15
10m 통과 = t10
```

구간 평균 closing speed:

```text
closing_speed ≈ 거리 변화 / 시간 변화
```

Ground Truth TTC:

```text
TTC ≈ current_distance / closing_speed
```

이를 Production 결과의:

```text
estimated distance
tracked closing velocity
estimated TTC
final WARNING / DANGER
```

와 비교한다.

또한 다음 acquisition delay를 기록한다.

```text
30m WARNING ROI 진입
→ tracker/history 생성
→ valid velocity 생성
→ valid TTC 생성
```

### Evidence 범위

이 controlled pass는 **Lab/controlled vehicle TTC validation evidence**다.

RV와 Target Vehicle이 모두 실제 도로상에서 움직이는 최종 Vehicle Validation이나 ISO full-conformity 시험을 이 시험만으로 대체하지 않는다.

---

# 7. Session C — 일반 주행 Continuous RAW

시나리오마다 녹화 버튼을 끊지 않는다.

LEFT/RIGHT 영상을 계속 녹화한 상태에서 가능한 범위에서 다음 상황을 자연스럽게 포함한다.

```text
정지
저속 출발
일반 주행
측면 동속 차량
뒤에서 접근하는 차량
빠르게 접근하는 차량
멀어지는 차량
Turn Signal OFF → ON
반대쪽 Turn Signal
선회
정지
```

모든 항목이 한 RUN에 들어갈 필요는 없다. 현장 여건에 따라 몇 개의 긴 RUN으로 나누면 된다.

Lab에서는 필요한 구간만 잘라 아래 검증에 재사용한다.

| 추출 구간 | 주요 검증 |
|---|---|
| 차량 측면 동속 target | DANGER ROI / adjacent zone 안정성 |
| rear 0~10m | DANGER 거리/ROI fail-safe |
| rear 10~30m 동속 | base WARNING no-downgrade |
| rear 10~30m 접근 | Velocity/TTC tracking |
| rear 10~30m 빠른 접근 | TTC escalation 후보 |
| rear 10~30m 멀어짐 | receding no-downgrade |
| rear >30m | no-warning |
| Turn Signal OFF/ON | same-side escalation |
| 반대쪽 Turn Signal | opposite-side non-interference |
| detection 재획득 | TTC UNKNOWN / no-downgrade |

Ground Truth 거리/속도가 없는 구간은 **기능 replay 또는 정성 검토용**으로 사용하고, 정량 Distance/TTC accuracy 근거와 구분한다.

---

# 8. Ego Motion 자료

별도 시나리오별 파일을 만들기보다 Session C의 연속 RAW에서 가능한 구간을 추출한다.

필요 상태:

```text
STOPPED
LOW_SPEED
MOVING
HIGHWAY
TURN
low-texture / tunnel
```

권장:

- LEFT/RIGHT를 동일 시점에 녹화
- 각 상태가 최소 수십 초 이상 연속으로 포함되도록 확보
- STOPPED → MOVING, MOVING → STOPPED 전환이 포함되면 좋음

HIGHWAY나 tunnel처럼 같은 세션에서 확보하기 어려운 조건만 별도 RAW session으로 촬영한다.

현재 `EGO_MOTION_BSD_GATE=False`이므로 Forward severity closure의 필수 gate는 아니지만 실제 차량 적용 전 calibration/robustness evidence로 사용한다.

---

# 9. Session D — Reverse Assist Continuous RAW

Reverse도 시나리오별로 영상을 끊지 않는다.

가능하면 marker를 먼저 배치한다.

```text
Reverse Side DANGER  = 1.5m
Reverse Side WARNING = 3.0m
Rear DANGER          = 1.5m
Rear WARNING         = 3.0m
```

하나 또는 몇 개의 연속 RAW에서 다음을 포함한다.

```text
LEFT 1.5m / 3m
RIGHT 1.5m / 3m
REAR 1.5m / 3m
정지 장애물
사람/자전거/차량 등 다양한 대상
LEFT/RIGHT 동시 위험
Reverse ON
Reverse OFF
```

Lab에서 필요한 구간을 분리하여 Reverse ROI / Distance / state reset 검증에 사용한다.

실측 evidence 전 `REVERSE_ASSIST_CALIBRATED=False`를 유지한다.

---

# 10. 녹화 툴 및 Lab 구간 자르기

현행 녹화 툴:

```text
tools/run_cameraREC.bat
→ tools/tool_multi_camera_recorder.py
```

은 긴 RAW 영상을 Lab에서 잘라 사용하는 흐름을 이미 지원한다.

`검증` 탭에 다음 기능이 구현되어 있다.

```text
🔪 구간 지정해서 자르기
```

사용 흐름:

```text
1. RAW 녹화
2. 영상과 *_timestamps.csv 저장
3. [검증] 탭에서 원본 선택
4. [구간 지정해서 자르기]
5. 슬라이더 / ±1 / ±10 frame으로 시작·끝 확인
6. 시작 frame / 끝 frame 지정
7. 새 파일명으로 저장
8. 잘라낸 영상 + 대응 *_timestamps.csv를 함께 사용
9. 잘라낸 파일도 다시 검증 후 TEST replay에 사용
```

### Timestamp 보존

구간을 자를 때:

- 영상과 CSV를 **동일 frame range**로 함께 자른다.
- 잘라낸 CSV의 `frame_idx`는 0부터 다시 부여한다.
- `perf_counter_sec`와 `wall_clock_iso`는 **원본 값을 그대로 보존**한다.
- 따라서 원래 녹화 시점의 실제 frame timing을 유지한 채 Velocity/TTC replay에 사용할 수 있다.

AVI/XVID의 임의 seek 오차를 피하기 위해 실제 자르기 처리는 원본의 처음부터 순차 디코딩하여 지정 frame 구간을 추출한다. 긴 영상은 자르는 데 시간이 걸릴 수 있으나 프레임 범위 정합성을 우선한다.

### 주의

외부 일반 영상편집기로 영상만 임의로 자르거나 재인코딩하면 `_timestamps.csv`와 프레임 대응이 깨질 수 있다.

Side BSD TEST replay용 구간은 가능하면 **현행 Multi Camera Recorder의 구간 자르기 기능**을 사용한다.

---

# 11. 파일명 권장

현장 RAW:

```text
{date}_left_raw_run01.avi
{date}_right_raw_run01.avi
{date}_left_ttc_run01.avi
{date}_right_ttc_run01.avi
{date}_rear_reverse_raw01.avi
```

Lab 추출본:

```text
{date}_left_same_speed_01.avi
{date}_left_rear_closing_01.avi
{date}_left_ttc_boundary_01.avi
{date}_right_receding_01.avi
{date}_ego_stopped_01_left.avi
{date}_ego_stopped_01_right.avi
{date}_reverse_left_1p5m_01.avi
```

원본 RAW는 삭제하지 않고 보존한다.

---

# 12. 현장 최소 체크리스트

현장에서 아래만 확보하면 Lab 작업을 시작할 수 있다.

```text
[ ] 최종 예정 카메라 장착 위치/각도 기록
[ ] RV rear trailing-edge = 0m 기준 확인
[ ] LEFT/RIGHT 거리 marker 자료
    3/5/6/7/8/9/10/15/20/30m
[ ] 10m/30m 주변 및 가능하면 lateral position 자료
[ ] 차량 측면 + rear 0~30m ROI가 보이는 자료
[ ] TTC용 10/15/20/25/30m marker
[ ] 정지 RV + Target Vehicle 접근 RAW 1세트 이상
[ ] 일반 주행 LEFT/RIGHT Continuous RAW
[ ] Turn Signal 포함 구간
[ ] STOPPED / MOVING 구간
[ ] Reverse 1.5m/3m + Continuous RAW
[ ] source / mount / session 메타데이터
```

가능하면 추가:

```text
[ ] TTC 느린/기준/빠른 접근 세트
[ ] HIGHWAY
[ ] TURN
[ ] tunnel / low-texture
[ ] 야간/역광 등 환경조건
```

---

# 13. Lab 복귀 후 처리 순서

```text
RAW 영상 원본 보존
→ Recording Validation
→ 필요한 구간 자르기
→ L1 Ground Truth/Scenario 정리
→ L2 ROI 검증
→ L3 Distance Calibration
→ L4 ROI + Distance replay
→ L5 Velocity / TTC replay
→ L6 Turn Signal / HMI replay
→ L7 Ego Motion
→ L8 Reverse Assist
→ L9 Lab Closure
```

상세 판정은 다음 문서를 따른다.

```text
docs/sidebsd/04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md
docs/sidebsd/00_index/CURRENT_VALIDATION_ROADMAP.md
```
