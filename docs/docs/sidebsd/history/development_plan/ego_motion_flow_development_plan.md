# Ego Motion Optical Flow 개발 계획 및 알고리즘 반영 계획

## 1. 문서 목적

본 문서는 현재 개발 중인 **사이드 카메라 기반 Ego Motion / 차량 주행상태 판단 시스템**의 향후 개발 방향을 정리한 것이다.

현재 단계에서는 실제 차량용 판정 코드를 추가로 변경하지 않고, 먼저 **분석툴(Analyzer)** 을 이용하여 실제 필드 영상에서 Optical Flow 특성과 차량 영향 여부를 검증한다.

실제 영상 분석 결과를 기준으로 최종 알고리즘을 결정한 뒤, **분석툴과 동일한 핵심 로직을 실제 차량 코드에 반영**한다.

---

## 2. 현재까지 완료된 사항

### 2.1 분석툴의 기본 기능

현재 Analyzer는 다음 기능을 갖는다.

- 좌/우 사이드 카메라 영상을 각각 등록
- 영상 상태 분류
  - STOPPED
  - LOW_SPEED
  - MOVING
  - HIGHWAY
- Sparse Optical Flow 계산
  - Shi-Tomasi Feature Detection
  - Pyramidal Lucas-Kanade Optical Flow
  - Outlier 제거
  - EMA smoothing
- Left / Right / Fused Flow 계산
- 상태별 Flow 통계 계산
- Threshold 자동 추정
- CSV / JSON / Markdown / Config snippet 저장

### 2.2 ROI 기능

ROI는 단순 숫자 입력뿐 아니라 영상 화면에서 **마우스로 직접 지정**할 수 있도록 구성한다.

좌/우 카메라의 ROI는 독립적으로 설정한다.

ROI를 조정한 후 동일 영상을 다시 분석하여 ROI 변경 전/후의 결과를 비교할 수 있다.

### 2.3 추가된 Flow 진단 지표

현재 Analyzer에는 기존 Mean Flow 이외에 다음과 같은 진단용 지표가 추가되어 있다.

- **Mean**: ROI 내 특징점 이동량 평균
- **Median**: ROI 내 특징점 이동량 중앙값
- **P90**: 이동량의 90 percentile
- **Active Ratio**: 일정 이동량 이상인 특징점 비율
- **Coverage**: 움직임이 ROI 공간 전체에 얼마나 퍼져 있는지
- **Dominant Ratio**: 움직임이 특정 영역에 집중되는 정도
- **Direction Consistency**: 이동 방향의 일관성

현재 이 추가 지표들은 **실험용 진단 지표**이며, 아직 실제 차량 판정 로직에 직접 반영하지 않는다.

---

# 3. 현재 단계의 핵심 목표

현재 Analyzer의 목적은 Threshold를 바로 확정하는 것이 아니다.

핵심 목표는 다음과 같다.

> **사이드 카메라 영상에서 자차의 실제 주행에 의한 Optical Flow와 옆 차량/후방 차량 등의 독립적인 움직임으로 발생하는 Optical Flow를 구분할 수 있는지를 실제 데이터로 확인하는 것**

즉, ROI를 완벽하게 차량으로부터 피하는 것이 목적이 아니다.

사이드 카메라 구조상 옆 차선과 후방 영역이 함께 보이므로 차량이 ROI에 들어오는 상황은 피할 수 없다.

따라서 최종 목표는 다음과 같다.

> **차량이 ROI에 들어오더라도 자차의 Ego Motion 판단이 크게 흔들리지 않는 알고리즘을 찾는 것**

---

# 4. 실제 영상 수집 계획

최종 알고리즘을 결정하기 전에 다음 조건의 영상을 확보한다.

## 4.1 STOPPED

### A. 완전 정차

- 자차 정지
- 옆 차량 없음
- 후방 차량 움직임 없음

목적:

- 센서/영상 자체의 기본 Flow Noise 확인
- STOPPED 상태의 기준값 확인

### B. 정차 + 옆 차량 통과

- 자차 정지
- 옆 차선 차량이 빠르게 지나감

가장 중요한 테스트 조건 중 하나다.

목적:

- 차량 때문에 Flow가 얼마나 튀는지 확인
- Mean만 사용할 경우 STOPPED가 잘못 MOVE로 판단되는지 확인
- 차량 Flow가 공간적으로 국부적인지 확인

### C. 정차 + 후방 차량 접근/통과

목적:

- 후방 방향 차량의 Flow 영향 확인
- 카메라 원근 구조에서 차량 접근이 Ego Flow와 얼마나 혼동되는지 확인

---

## 4.2 LOW_SPEED

예:

- 주차장 출차
- 서행
- 혼잡 구간
- 정체 구간

목적:

- STOPPED와 LOW_SPEED의 분리 가능성 확인
- 실제 저속에서 Flow가 얼마나 작은지 확인

---

## 4.3 MOVING

일반 도로에서 정상적인 주행 영상을 확보한다.

목적:

- 일반 주행 상태의 Flow 분포 확인
- LOW_SPEED와의 분리 가능성 확인

---

## 4.4 HIGHWAY

고속도로 또는 고속 주행 구간 영상을 확보한다.

목적:

- 고속 주행에서 Flow가 어떻게 증가하는지 확인
- MOVING / HIGHWAY의 분리 가능성 확인

---

# 5. ROI 설정 실험 방법

ROI는 처음부터 지나치게 좁게 잡지 않는다.

사이드 카메라는 다음 영역을 동시에 볼 수 있다.

```text
┌───────────────────────────┐
│       멀리 후방            │
│                           │
│   옆 차선 차량             │
│                           │
│ 자기 차량 차체 / 바퀴       │
└───────────────────────────┘
```

## 5.1 제외 대상

가능한 경우 다음 자기 차량 영역은 ROI에서 제외한다.

- 자기 차량 차체
- 자기 차량의 고정 구조물
- 자기 차량 바퀴

특히 바퀴는 회전에 의해 특징점이 움직일 수 있으므로 Ego Motion 추정의 기준 영역으로 사용하지 않는 것을 우선한다.

## 5.2 포함 대상

자기 차량 영역을 제외한 뒤에는 다음과 같은 외부 환경을 충분히 포함한다.

- 도로
- 옆 차선
- 후방 도로
- 원거리 배경

차량이 들어오는 것을 완전히 피하려 하기보다, **차량이 들어왔을 때 알고리즘이 견딜 수 있는지**를 검증한다.

---

# 6. Analyzer에서 확인할 핵심 항목

각 영상마다 다음 지표를 기록한다.

| 항목 | 의미 | 확인 목적 |
|---|---|---|
| Mean | 특징점 이동량 평균 | 현재 기존 방식의 기준 |
| Median | 이동량 중앙값 | 일부 큰 Flow의 영향 감소 여부 |
| P90 | 상위 Flow 수준 | 강한 움직임 존재 여부 |
| Active Ratio | 움직이는 특징점 비율 | ROI에서 실제 움직임 규모 |
| Coverage | 움직임의 공간적 확산 | 전체 영역이 움직이는지 확인 |
| Dominant Ratio | 특정 영역 집중도 | 일부 차량만 움직이는지 확인 |
| Direction Consistency | Flow 방향의 일관성 | 자차 이동 특성과 외부 차량 움직임 비교 |

---

# 7. 특히 비교해야 할 두 가지 상황

## 7.1 정차 + 옆 차량 통과

예상 형태:

```text
도로 대부분      → 거의 움직이지 않음
옆 차량 영역     → 큰 Flow
```

따라서 다음 현상이 나타나는지 확인한다.

- Mean이 크게 증가하는가?
- Median은 상대적으로 안정적인가?
- Active Ratio가 높아지는가?
- Coverage는 낮게 유지되는가?
- Dominant Ratio가 높아지는가?
- Direction Consistency가 낮아지는가?

이 결과가 반복적으로 나타난다면 차량 통과는 **국부적인 Flow**로 볼 가능성이 있다.

---

## 7.2 자차 실제 주행

예상 형태:

```text
ROI 전체에 걸쳐 일정한 Flow
```

다음 특성을 확인한다.

- 여러 영역에서 동시에 Flow가 발생하는가?
- Coverage가 증가하는가?
- 특정 한 영역에만 Flow가 집중되지 않는가?
- 방향이 일정하게 유지되는가?

반복적으로 이런 특성이 확인되면 자차 주행은 **전역적인 Flow 패턴**으로 판단할 수 있다.

단, 위 내용은 현재 단계의 가설이며 실제 영상으로 확인해야 한다.

---

# 8. 실제 영상 분석 후 알고리즘 결정 방법

최종 알고리즘은 미리 결정하지 않는다.

실제 데이터 결과에 따라 아래 셋 중 하나를 선택한다.

## 경우 A. 기존 Mean 방식으로 충분한 경우

다음 조건이 충족되면 기존 방식 유지 가능.

- STOPPED와 MOVING이 충분히 분리됨
- 옆 차량 통과 시에도 STOP threshold를 거의 넘지 않음
- LOW_SPEED / MOVING / HIGHWAY 구분이 안정적임

이 경우:

> 현재 Optical Flow + Mean 기반 로직을 유지하고 Threshold만 재튜닝한다.

---

## 경우 B. Mean만 불안정하고 Median 등으로 개선되는 경우

예:

```text
옆 차량 통과
Mean   ↑↑↑
Median →
```

이런 현상이 여러 영상에서 반복되면 Mean 단독 사용을 재검토한다.

가능한 후보:

```text
Ego Flow = Median
```

또는

```text
Ego Flow = Mean + Median 조건
```

등을 검증한다.

---

## 경우 C. 공간 분포까지 사용해야 하는 경우

예:

```text
옆 차량 통과
Coverage  낮음
Dominant  높음

자차 주행
Coverage  높음
Dominant  낮음
```

이 차이가 반복적으로 확인되면 다음과 같은 형태의 판정 로직을 검토한다.

```text
IF Flow가 충분히 크고
   Coverage가 충분히 높고
   Direction Consistency가 충분히 높으면
       → 자차 주행 Flow
ELSE IF 특정 영역에만 Flow가 집중되면
       → 외부 차량 영향 가능성
ELSE
       → 기존 Flow 판단 유지
```

구체적인 Threshold 값은 실제 데이터로 산출한다.

---

# 9. Threshold 결정 방법

Threshold는 임의의 숫자로 정하지 않는다.

각 상태별 영상에서 실제 분포를 확인한 후 결정한다.

기본 목표는 다음과 같다.

```text
STOPPED
   ↓
STOP threshold

LOW_SPEED / MOVING
   ↓
MOVE threshold

MOVING / HIGHWAY
   ↓
HIGHWAY threshold
```

특히 다음 경계를 우선 검증한다.

### STOP / MOVE

가장 중요하다.

정차 중 외부 차량 때문에 Flow가 증가했을 때도 STOPPED가 유지되어야 한다.

### MOVE / HIGHWAY

고속 주행에서 Flow가 충분히 증가하는지 확인한다.

---

# 10. 실제 차량 코드 반영 원칙

**Analyzer와 실제 차량용 코드의 핵심 계산 로직은 최종적으로 동일해야 한다.**

Analyzer에서만 새로운 보정 로직을 사용하고 실제 차량 코드가 다른 로직을 사용하면 안 된다.

최종 구조는 다음과 같이 가져간다.

```text
             공통 Ego Flow Logic
                     │
          ┌──────────┴──────────┐
          │                     │
       Analyzer            실제 차량 코드
          │                     │
   영상 기반 검증             실시간 실행
          │                     │
   Threshold 결정              상태 판단
```

Analyzer는 계산 결과를 시각화하고 실험하기 위한 도구이고, 실제 차량 코드는 동일한 핵심 로직을 실시간으로 실행한다.

---

# 11. 실제 코드에 반영할 항목

최종 알고리즘이 결정된 후 다음 항목을 실제 차량용 코드에 반영한다.

## 11.1 ROI

- 좌측 ROI
- 우측 ROI
- 자기 차량 영역 Mask

필요하면 카메라별 서로 다른 ROI를 사용한다.

## 11.2 Feature Detection

Analyzer와 동일한 조건을 사용한다.

- maxCorners
- qualityLevel
- minDistance
- blockSize

## 11.3 Optical Flow

Analyzer와 동일한 LK 파라미터를 사용한다.

- winSize
- maxLevel
- criteria

## 11.4 Outlier 제거

Analyzer에서 검증된 방식을 그대로 반영한다.

## 11.5 Flow 통계

최종 실험 결과에 따라 아래 항목 중 필요한 것을 반영한다.

- Mean
- Median
- P90
- Active Ratio
- Coverage
- Dominant Ratio
- Direction Consistency

## 11.6 Temporal filtering

현재 EMA 및 프레임 기반 안정화 조건을 실제 차량 코드와 동일하게 맞춘다.

---

# 12. 실제 차량 상태 판정 구조

최종적으로는 다음 형태를 목표로 한다.

```text
Frame
  ↓
ROI / Mask
  ↓
Feature Detection
  ↓
Lucas-Kanade Optical Flow
  ↓
Outlier 제거
  ↓
Flow 통계 계산
  ↓
외부 차량 영향 판단
  ↓
Ego Flow 산출
  ↓
EMA / Temporal Filtering
  ↓
STOPPED / LOW_SPEED / MOVING / HIGHWAY
```

---

# 13. 가장 중요한 검증 시나리오

최종 로직을 실제 코드에 반영하기 전에 반드시 다음 시나리오를 확인한다.

| 시나리오 | 기대 결과 |
|---|---|
| 정차 + 외부 차량 없음 | STOPPED |
| 정차 + 옆 차량 통과 | STOPPED 유지 |
| 정차 + 후방 차량 이동 | STOPPED 유지 가능 여부 확인 |
| 저속 출발 | LOW_SPEED |
| 일반 주행 | MOVING |
| 고속 주행 | HIGHWAY |
| 저속에서 차량 추월 | 자차 상태 오판 최소화 |
| 고속도로에서 차량 추월 | 자차 상태 오판 최소화 |

특히 **정차 + 옆 차량 통과**를 가장 중요한 False Positive 시험으로 본다.

---

# 14. 개발 순서

## Step 1. Analyzer 완료

현재 상태.

- ROI 마우스 지정
- 좌/우 독립 ROI
- Flow 진단 지표 계산
- 결과 저장

여기서 일단 Analyzer 기능 개발은 멈춘다.

---

## Step 2. 실제 영상 확보

상태별 필드 영상을 확보한다.

특히:

- STOPPED
- STOPPED + 옆 차량 통과
- STOPPED + 후방 차량 통과
- LOW_SPEED
- MOVING
- HIGHWAY

---

## Step 3. ROI 실험

각 카메라에 대해 마우스로 ROI를 여러 가지 형태로 바꿔본다.

목표는:

> 자기 차량 구조물은 최소화하면서 외부 차량이 들어와도 전체적인 Ego Flow 특성을 유지하는 ROI를 찾는 것

---

## Step 4. Flow 지표 비교

영상별로 다음 값을 비교한다.

```text
Mean
Median
P90
Active Ratio
Coverage
Dominant Ratio
Direction Consistency
```

---

## Step 5. 알고리즘 결정

실제 결과에 따라 결정한다.

```text
기존 Mean 유지
        또는
Mean + Median
        또는
Median 중심
        또는
Flow 공간 분포 추가
```

이 단계에서 처음으로 최종 알고리즘을 확정한다.

---

## Step 6. Threshold 결정

확정된 알고리즘을 기준으로:

- STOP threshold
- MOVE threshold
- HIGHWAY threshold
- STOP frame count
- MOVE frame count

등을 결정한다.

---

## Step 7. 실제 차량 코드 반영

Analyzer에서 확정한 **동일 알고리즘과 동일 Threshold**를 실제 차량 코드에 적용한다.

가능하면 공통 Core 함수/모듈 구조로 만들어 Analyzer와 실제 차량 코드의 계산 차이를 최소화한다.

---

## Step 8. 차량 실차 검증

실제 차량에서 다음을 확인한다.

- 정차 안정성
- 출발 반응성
- 저속 반응성
- 일반 주행 반응성
- 고속 주행 반응성
- 옆 차량 추월/추월당함
- 후방 차량 접근
- 야간/저조도
- 그림자
- 노면 변화
- 차선 변경

---

# 15. 결과에 따른 수정 기준

최종 수정 여부는 아래 기준으로 판단한다.

### 수정하지 않음

다음이 충분히 만족되는 경우.

- STOPPED 오판이 거의 없음
- LOW_SPEED / MOVING / HIGHWAY 분리가 가능함
- 옆 차량 영향이 허용 범위 내
- 계산량이 실제 시스템 요구사항에 맞음

→ 기존 알고리즘 유지 + Threshold 튜닝

### 수정함

다음 문제가 반복되는 경우.

- 정차 중 차량 통과로 MOVE 오판
- 일부 강한 Flow 때문에 Mean이 과도하게 상승
- 카메라별 Flow 분포 차이가 큼
- 속도가 증가해도 Threshold가 안정적으로 분리되지 않음

→ 검증된 추가 지표를 실제 알고리즘에 반영

---

# 16. 최종 원칙

1. **실제 데이터 없이 알고리즘을 임의로 복잡하게 만들지 않는다.**
2. **ROI는 차량을 완전히 피하기 위한 것이 아니라 불필요한 자기 차량 영역을 줄이고 외부 환경을 안정적으로 관찰하기 위한 것이다.**
3. **옆 차량 통과는 피할 수 없는 조건으로 보고 알고리즘의 내성을 검증한다.**
4. **현재 추가한 Coverage / Dominant / Direction 등의 지표는 실험 결과에 따라 채택 여부를 결정한다.**
5. **최종적으로 Analyzer와 실제 차량 코드의 핵심 Flow 계산/판정 로직은 동일해야 한다.**
6. **Threshold는 실제 필드 데이터 분포를 기반으로 결정한다.**
7. **Analyzer → 실제 코드 반영 순서를 유지하여 변경 원인을 추적할 수 있게 한다.**

---

# 17. 현재 결론

현재는 **분석툴 개발을 여기서 멈추고 실제 영상을 이용한 검증 단계로 넘어가는 것이 적절하다.**

실제 영상 결과를 확인하기 전에는 Coverage, Dominant, Direction 등의 지표를 최종 알고리즘으로 확정하지 않는다.

실제 데이터에서 가장 안정적으로 자차의 움직임과 외부 차량의 움직임을 구분하는 방법을 확인한 뒤, 그 로직과 Threshold를 실제 차량 코드에 동일하게 반영한다.
