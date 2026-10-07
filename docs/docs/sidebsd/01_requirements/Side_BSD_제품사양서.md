# Side BSD 제품 표준 사양서

> **문서 상태:** PRODUCT STANDARD  
> **문서 ID:** SBSD-PS-001
> **버전:** v1.3
> **작성일:** 2026-09-14
> **제품군:** Camera-based Side BSD / Reverse Assist for RV  
> **Software 기준선:** `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`
> **승인 상태:** SELF-REVIEWED
> **문서 목적:** 고객사/OEM 기술협의, 내부 제품기획·설계·품질 기준, 사용자 설명서 작성 기준  
> **적용 우선순위:** 본 문서는 제품의 상위 기능·운용 사양을 정의한다. 상세 소프트웨어 요구사항은 Type III/SAV 요구사항을 통합한 단일 SRS에서 관리한다.

---

## 1. 문서 목적 및 적용 범위

본 문서는 RV·모터홈과 같이 차체가 길고 운전석에서 차량 측면과 후방을 직접 확인하기 어려운 차량을 위한 **카메라 기반 Side Blind Spot Detection(BSD) 및 Reverse Assist 제품의 표준 기능과 제품 특성**을 정의한다.

본 사양서는 다음 사용자를 위한 상위 제품 기준 문서다.

- 고객사/OEM 차량 전장·ADAS 담당자
- 제품 기획·영업 기술지원 담당자
- 시스템/소프트웨어/하드웨어 설계 담당자
- 시험·품질·인수 담당자
- 사용자 설명서 및 서비스 문서 작성자

본 제품은 **경고 전용 운전자 보조 시스템**이며 차량의 제동, 조향, 가속 또는 구동을 직접 제어하지 않는다.

### 1.1 용어 및 약어

본 문서에서 사용하는 주요 용어와 약어는 다음과 같다. ISO 기반 용어와 본 제품의 위험도/HMI 정책 용어는 구분하여 해석한다.

| 용어 / 약어 | 정의 | 구분 |
|---|---|---|
| **RV** | Recreational Vehicle. 본 제품의 주요 적용 대상인 모터홈 등 장축 차량군 | 일반 |
| **ADAS** | Advanced Driver Assistance Systems. 운전자의 인지·판단을 보조하는 운전자 보조 시스템 | 일반 |
| **Side BSD / BSD** | Side Blind Spot Detection / Blind Spot Detection. 차량 측면 및 측후방의 사각영역 위험 대상을 감지하여 운전자에게 경고하는 기능 | 제품 기능 |
| **LCDAS** | Lane Change Decision Aid System. 차선 변경 시 측면 또는 후방 인접 차선의 차량과의 충돌 위험을 운전자에게 경고하는 시스템 | ISO 17387 |
| **Type III** | ISO 17387의 LCDAS coverage 분류 중 Blind Spot Warning 기능과 Closing Vehicle Warning 기능을 함께 제공하는 유형 | ISO 17387 |
| **SAV** | Slow Approaching Vehicle. ISO 17387의 target vehicle closing-speed 분류 중 최대 접근 상대속도 10 m/s를 기준으로 하는 유형. 본 제품은 SAV를 초기 Rear Closing Risk 기준으로 사용한다. | ISO 17387 / 제품 적용 |
| **TTC** | Time To Collision. 현재 상대 접근속도가 유지된다고 가정할 때 Target Vehicle이 Subject Vehicle과 충돌하기까지의 추정 시간. 본 제품에서는 2.5 s를 위험도 격상 threshold로 사용한다. | ISO 17387 / 제품 적용 |
| **Closing Speed** | Target Vehicle과 Subject Vehicle 사이의 상대 접근속도. 양수이면 Target Vehicle이 후방에서 Subject Vehicle에 접근하는 상태를 의미한다. | ISO 17387 |
| **Subject Vehicle** | 본 Side BSD 시스템이 장착되어 기준이 되는 차량 | ISO 17387 |
| **Target / Target Vehicle** | 감지·추적 및 위험도 판단의 대상이 되는 주변 객체 또는 차량 | ISO 17387 / 제품 |
| **ROI** | Region of Interest. 영상 내에서 대상 존재 또는 위험도를 판단하기 위해 설정한 관심 영역 | 영상처리 |
| **DANGER ROI** | 본 제품에서 DANGER 위험도를 판정하기 위한 영상 영역. Forward 기준 차량 측면 위험구간과 Rear 0~10m를 포함하도록 Calibration한다. | 제품 정의 |
| **WARNING ROI** | 본 제품에서 WARNING 위험도를 판정하기 위한 영상 영역. DANGER ROI를 포함하며 Forward 기준 Rear 30m까지 확장한다. | 제품 정의 |
| **HMI** | Human-Machine Interface. Display, Direction OSD, LED, Buzzer 등 운전자에게 상태와 경고를 전달하는 인터페이스 | 일반 / 제품 |
| **OSD** | On-Screen Display. 카메라 영상 위에 중첩하여 표시하는 방향·상태·경고 그래픽 | 제품 HMI |
| **SRS** | System Requirements Specification. 시스템이 만족해야 할 시험 가능하고 추적 가능한 정식 시스템 요구사항 명세서 | 문서 체계 |
| **Calibration** | 카메라 장착조건, ROI 및 거리추정 등 차량별 파라미터를 설정·보정하고 유효성을 확인하는 과정 | 제품 / 검증 |
| **Rear Trailing-edge Plane** | 차량의 실제 후단을 기준으로 정의한 거리 기준면. Forward rear distance의 0m 기준점 | 제품 좌표 기준 |
| **SAFE / NORMAL** | 정상 입력과 정상 처리가 이루어진 상태에서 현재 경고가 요구되지 않는 비경고 상태 | 제품 상태 |
| **Fault / Unavailable** | 채널·입력·처리 이상 또는 유효 판단조건 미확보로 해당 기능의 정상 위험판정을 신뢰할 수 없는 상태. SAFE/NORMAL과 구분한다. | 제품 상태 |

> **용어 해석 주의:** `DANGER`, `WARNING`, `SAFE/NORMAL`, `DANGER ROI`, `WARNING ROI`는 본 제품의 위험도 및 HMI 정책 용어이며 ISO 17387의 경고단계 명칭을 그대로 재현한 것으로 해석하지 않는다.

---

## 2. 제품 개요

Side BSD는 LEFT / RIGHT Side Camera와 REAR Camera를 이용하여 차량 측면·측후방 및 후진 시 주변 객체를 감지하고, 위험 수준에 따라 Display, 방향 표시, LED 및 Buzzer를 통해 운전자에게 정보를 제공한다.

제품은 두 가지 운용 기능으로 구성된다.

| 기능 | 운용 상태 | 목적 |
|---|---|---|
| **Forward Side BSD** | 전진 주행 | 좌/우 측면 및 측후방 차선변경 위험 보조 경고 |
| **Reverse Assist** | 후진 | 좌/우 측후방 및 후방 근접 장애물 보조 경고 |

Forward Side BSD와 Reverse Assist는 감지 거리, 대상 처리, 경고 조건 및 HMI 정책을 서로 분리하여 운용한다.

---

## 3. 제품 핵심 특징

### 3.1 긴 RV 차체를 고려한 Side Blind-zone Coverage

일반 승용차보다 차체가 긴 RV 특성을 고려하여 Side Camera가 커버하는 **차량 측면 위험구간부터 차량 후단 이후 영역까지 하나의 연속된 Side BSD 경고영역**으로 관리한다.

카메라와 차량 후단 사이의 긴 측면 구간은 단순한 후방 거리값으로 환산하지 않고 Image ROI occupancy로 직접 관리한다.

### 3.2 ROI + Distance 이중 안전 판정

Forward Side BSD는 Image ROI와 Distance estimation을 서로 독립된 위험판정 경로로 사용한다.

```text
ROI level       ┐
                ├─ severity max ─→ Base Warning Level
Distance level  ┘
```

두 경로의 결과가 서로 다르면 더 높은 위험도를 유지한다. 따라서 거리추정 오차가 발생하더라도 DANGER ROI가 2차 방어선으로 동작하고, 반대로 ROI 경계 오차가 발생하더라도 거리판정이 근거리 위험을 유지할 수 있다.

### 3.3 TTC 기반 위험도 증강

10~30m WARNING 영역의 접근 대상에 대해서는 tracked distance history로 상대 접근속도와 TTC(Time To Collision)를 계산한다.

TTC는 기존 경고를 억제하는 조건으로 사용하지 않으며 **위험도가 높은 접근대상을 더 빠르게 DANGER로 격상하기 위한 증강 정보**로만 사용한다.

### 3.4 운전자 차선변경 의도 연동

동일 방향 Turn Signal이 활성화된 상태에서 해당 Side가 WARNING이면 DANGER로 격상한다. 반대 방향 Turn Signal은 다른 Side의 위험도에 영향을 주지 않는다.

### 3.5 후방 영상 유지와 Side BSD 동시 표시

RV 운전자가 주행 중 후방 카메라 화면을 지속적으로 확인하는 사용환경을 고려하여, REAR 화면 위에 LEFT/RIGHT Side BSD 상태를 방향별 OSD로 표시할 수 있다.

### 3.6 Forward / Reverse 정책 분리

후진 시에는 Forward용 상대속도/TTC 억제 개념을 사용하지 않고 근거리 장애물 보조에 맞는 별도 ROI와 거리 경계를 적용한다.

### 3.7 채널 단위 이상상태 격리

특정 카메라 채널의 결과가 유효하지 않거나 stale 상태가 되면 해당 채널을 정상 SAFE로 오인하지 않고 Fault/Unavailable로 구분한다. 가능한 범위에서 정상 채널과 메인 HMI 처리는 계속 유지한다.
 - stale 상태 : 카메라 신호가 갱신되지 않고 과거 상태 그대로 멈춰 있는 '데이터 동결' 상태
---

## 4. 시스템 구성

### 4.1 구성 블록

```text
 LEFT Camera  ─┐
               ├─ Detection / Tracking ─┐
 RIGHT Camera ─┘                        │
                                        ├─ ROI / Distance / Velocity / TTC
 REAR Camera  ─── Rear Detection ───────┘

 LEFT Turn Signal ─┐
 RIGHT Turn Signal ├─ Vehicle State / Mode Logic
 Reverse Signal ───┘

                ↓
         Risk / Alert Decision
                ↓
   Display / Direction OSD / LED / Buzzer
```

### 4.2 표준 입력

| 입력 | 용도 |
|---|---|
| LEFT Camera | 좌측 Forward Side BSD / Reverse Assist |
| RIGHT Camera | 우측 Forward Side BSD / Reverse Assist |
| REAR Camera | 후방 영상 / Reverse Assist |
| LEFT Turn Signal | 좌측 차선변경 의도 반영 |
| RIGHT Turn Signal | 우측 차선변경 의도 반영 |
| Reverse Signal | Reverse Assist 모드 전환 |

Forward Side BSD의 현재 제품 기준은 **직접적인 자차 속도신호를 필수 입력으로 요구하지 않는다.** 상대 접근위험은 tracked target의 거리 변화로 계산한다.

### 4.3 표준 출력

| 출력 | 기능 |
|---|---|
| Display | Camera 영상 및 방향별 WARNING/DANGER 표시 |
| Direction OSD | REAR/Side 영상 가장자리의 좌·우 경고 표시 |
| LED | 차량/고객 HMI 연동용 방향 경고 출력 |
| Buzzer | 위험상황의 청각 경고 |
| Fault / Unavailable | 정상 SAFE와 구분되는 기능상태 출력 |

전기적 레벨, 커넥터, 통신 프로토콜 및 차량별 HMI 연동 상세는 고객사/차종별 Interface Specification에서 정의한다.

---

## 5. Forward Side BSD 기능 사양

### 5.1 기능 목적

전진 주행 중 LEFT / RIGHT Side Camera를 이용하여 차량 측면과 측후방의 차선변경 위험을 감지하고 운전자에게 방향별 경고를 제공한다.

좌측과 우측은 독립적으로 위험도를 판단하며 동시에 서로 다른 경고 수준을 가질 수 있다.

### 5.2 거리 기준점

Forward rear distance의 기준점은 **Subject Vehicle Rear Trailing-edge Plane**, 즉 차량의 실제 후단 기준면이다.

```text
Rear trailing edge = 0 m
Rearward direction = positive distance
```

Side Camera optical center 또는 카메라 장착위치는 0m 기준점으로 사용하지 않는다.

### 5.3 Forward 경고영역

**그림 1. RV Side BSD 표준 경고영역 개념도**

<img src="./Images/RV%20Side%20BSD%20표준%20경고영역%20개념도.png" alt="로고" width="900" height="300">


```text
차량 진행방향 <-

 [Side Camera]====================[RV Rear Trailing Edge]====10m=========30m
       │                                  │                 │            │
       │<------ Vehicle-side zone ------->│<--- 0~10m ----->│<10~30m --->│
       │                                  │                 │            │
       │<------------- DANGER ROI ------------------------->│            │
       │                                                                  │
       │<--------------------- WARNING ROI ------------------------------->│

                         실제 30m 초과 / WARNING ROI 밖 → No Warning
```

표준 정책:

| 물리 영역 / 판단 | 기본 위험도 |
|---|---|
| 차량 측면 위험구간 | DANGER ROI |
| Rear 0~10m | DANGER |
| Rear 10~30m | WARNING |
| 실제 Rear >30m 및 WARNING ROI 밖 | No Warning |

`DANGER ROI ⊂ WARNING ROI`의 nested 구조를 사용한다.

DANGER ROI는 **차량 측면 위험구간 + Rear 0~10m**를 절대 영역으로 포함하여 거리추정 오차에 대한 보조 방어기능을 제공한다.

### 5.4 기본 위험도 판정

Forward 기본 위험도는 ROI와 Distance를 독립적으로 계산한 뒤 더 높은 결과를 적용한다.

```text
ROI Level
  DANGER ROI  → DANGER
  WARNING ROI → WARNING

Distance Level
  <=10m → DANGER
  <=30m → WARNING
  >30m  → SAFE

Base Level = max(ROI Level, Distance Level)
```

**그림 2. 이중 안전 판정 예시**

```text
실제 위험 target가 DANGER ROI 안에 존재
              │
              ├─ ROI 판단      = DANGER
              └─ 거리추정 오류 = 18m → WARNING

최종 Base Level = DANGER
```

반대로 실제 Rear 7m target가 ROI 경계에서 WARNING으로 판정되더라도 Distance Level이 DANGER이면 최종 위험도는 DANGER를 유지한다.

### 5.5 TTC / Relative Closing Risk

Forward Side BSD의 초기 closing-risk 기준은 SAV이며 TTC threshold는 다음과 같다.

```text
SAV TTC Threshold = 2.5 s
```

적용 정책:

| 조건 | 결과 |
|---|---|
| Base DANGER | TTC 상태와 관계없이 DANGER 유지 |
| Base WARNING + TTC > 2.5s | WARNING 유지 |
| Base WARNING + TTC ≤ 2.5s | DANGER 격상 |
| TTC UNKNOWN | Base Level 유지 |
| 실제 >30m / WARNING ROI 밖 | TTC early-warning 없음 |

**TTC 2.5s 해석 예시**

TTC는 차량의 절대 주행속도가 아니라 **후방 대상과의 거리 및 상대 접근속도(Closing Speed)**로 결정된다.

```text
TTC = Rear Distance / Closing Speed

예시)
Subject Vehicle = 80 km/h
Target Vehicle  = 116 km/h
Closing Speed   = 36 km/h = 10 m/s
Rear Distance   = 25 m

TTC = 25 m / 10 m/s = 2.5 s
→ Rear 25m는 Base WARNING 영역이지만 TTC ≤ 2.5s이므로 DANGER로 격상
```

즉, **뒤 차량이 내 차량보다 약 36 km/h 빠르게 25m 후방에서 접근하는 상황**이 TTC 2.5s 경계의 대표적인 예다. 위 80/116 km/h는 TTC의 의미를 설명하기 위한 예시이며, 2.5s가 특정 절대 차량속도를 의미하는 것은 아니다.

따라서 TTC와 상대속도는 기본 DANGER/WARNING을 SAFE로 낮추는 용도로 사용하지 않는다.

### 5.6 Turn Signal 연동

| 조건 | 결과 |
|---|---|
| Same-side WARNING + Turn Signal | DANGER 격상 |
| Same-side DANGER + Turn Signal | DANGER 유지 + 청각경고 조건 성립 |
| Opposite-side Turn Signal | 해당 Side 위험도에 영향 없음 |

Turn Signal과 TTC는 모두 Base Level보다 낮은 결과를 만들 수 없다.

---

## 6. Forward HMI 사양

### 6.1 Display Warning

Side BSD 상태는 LEFT / RIGHT 방향을 독립적으로 표시한다.

| 상태 | 표준 시각 표시 |
|---|---|
| NORMAL | 경고 OSD 없음 |
| WARNING | 해당 방향 **Yellow** 반투명 표시 |
| DANGER | 해당 방향 **Red** 반투명 표시 + 점멸 |

REAR 화면 사용 시 Side BSD 결과를 후방영상 좌·우 가장자리에 각각 표시할 수 있다.

예:

```text
LEFT = WARNING, RIGHT = DANGER
→ REAR 화면 좌측 Yellow / 우측 Red 동시 표시
```

Side Camera 단독 화면에서도 해당 방향의 경고 OSD를 동일한 색상체계로 표시한다.

### 6.2 Buzzer

Forward 운용에서 시각 위험도와 청각경고는 구분한다.

- WARNING: 시각 경고 중심
- DANGER: Red 시각 경고
- Same-side Turn Signal이 활성화된 DANGER: Buzzer를 이용하여 운전자 주의를 강화

차량별 LED/Buzzer 음색, 패턴 및 전기 인터페이스는 HMI/Interface Specification에서 세부 정의할 수 있다.

---

## 7. Reverse Assist 기능 사양

### 7.1 기능 목적

Reverse Signal이 활성화되면 LEFT / RIGHT Side Camera와 REAR Camera를 이용하여 후진 경로 주변의 근거리 장애물을 감지한다.

Forward Side BSD와 달리 후진 보조에서는 **모델이 제공하는 전체 검출 class를 사용할 수 있으며**, Forward용 Velocity/TTC 기반 판단과 Turn Signal 격상을 적용하지 않는다.

### 7.2 표준 경고거리

| 채널 | DANGER | WARNING |
|---|---:|---:|
| LEFT Reverse Assist | ≤ 1.5m | ≤ 3.0m |
| RIGHT Reverse Assist | ≤ 1.5m | ≤ 3.0m |
| REAR Reverse Assist | ≤ 1.5m | ≤ 3.0m |

각 채널은 후진 전용 ROI 및 거리 Calibration을 사용한다.

### 7.3 Reverse HMI

| 상태 | 표시 / 출력 |
|---|---|
| WARNING | 방향별 시각 경고 |
| DANGER | 방향별 시각 경고 + 해당 방향 Buzzer |

후진 중에는 REAR 화면을 기본 주행화면으로 유지하고 LEFT/RIGHT 위험상태를 후방영상 위에 방향별로 함께 표시한다.

Reverse Assist는 단안 영상 기반의 **근접 장애물 보조 기능**이며, 본 표준사양에서 횡방향 충돌궤적을 예측하는 별도 Predictive RCTA 기능으로 정의하지 않는다.

---

## 8. 경고 우선순위 및 상태 관리

위험 수준은 다음 순서를 사용한다.

```text
NORMAL / SAFE < WARNING < DANGER
```

동일 방향에서 여러 객체가 동시에 감지되면 가장 높은 위험 수준을 해당 방향의 대표 상태로 표시한다.

Forward 위험도는 개념적으로 다음과 같이 결합된다.

```text
Final Level = max(
    ROI Level,
    Distance Level,
    TTC Escalation Level,
    Turn Signal Escalation Level
)
```

이 구조에서 TTC 또는 Turn Signal은 기존 위험도를 낮출 수 없다.

---

## 9. Fault / Unavailable 및 안전동작

### 9.1 SAFE와 기능불가 상태의 구분

대상이 없어서 정상적으로 경고가 없는 `SAFE` 상태와 카메라/추론 결과를 신뢰할 수 없는 `Fault / Unavailable` 상태를 구분한다.

stale result, 이전 모드의 cache 또는 유효하지 않은 세션 결과는 현재 위험판정에 재사용하지 않는다.

### 9.2 채널 단위 격리

개별 카메라 채널에서 이상이 발생한 경우 가능한 범위에서 해당 채널만 Unavailable로 처리하고 정상 채널의 감지 및 HMI는 계속 유지한다.

공통 전원, 공통 처리자원 또는 시스템 전체 기능에 영향을 주는 고장은 별도 시스템 진단정책을 따른다.

---

## 10. Calibration 및 차량 적용 기준

### 10.1 Forward Distance Calibration

Forward distance는 차량 rear trailing-edge plane을 기준으로 측정한다.

대표 Calibration/Validation point는 다음 거리를 포함한다.

```text
3 / 5 / 6 / 7 / 8 / 9 / 10 / 15 / 20 / 30 m
```

특히 10m와 30m는 제품 HMI 경계 확인을 위한 핵심 기준점이다.

### 10.2 Forward ROI Calibration

Forward DANGER ROI는 다음을 포함하도록 설정한다.

```text
차량 측면 위험구간 + Rear 0~10m
```

Forward WARNING ROI는 DANGER ROI를 포함하며 Rear 30m까지 확장한다.

실제 30m 초과 대상은 정상 차량 Calibration에서 WARNING ROI 밖에 위치하도록 설정한다.

### 10.3 장착조건 관리

다음 조건이 변경되어 영상 geometry에 영향을 줄 경우 Calibration 유효성을 재검토한다.

- 차량 형상 및 rear trailing-edge 위치
- Camera 장착 위치·높이·각도
- Lens/FOV
- 해상도 및 영상 방향/반전

동일한 승인 차량·카메라·장착 Baseline과 허용공차 내에서는 공통 Calibration Baseline을 사용할 수 있다.

---

## 11. 제품 적용 규격 및 설계 근거

### 11.1 ISO 17387:2026 설계 참조

Forward Side BSD는 **ISO 17387:2026 — Lane Change Decision Aid Systems(LCDAS)**의 Type III 기능 개념을 설계 참조로 사용한다.

Type III는 Blind Spot Warning과 Rear Closing Vehicle Warning을 결합하는 개념이며, 본 제품은 긴 RV의 차량 측면 coverage와 rear closing risk를 하나의 Forward Side BSD 기능으로 구성한다.

공개 확인 가능한 ISO geometry 중 본 제품 설계와 연결되는 주요 rear reference는 다음과 같다.

```text
Line O : Rear trailing edge 뒤 10m
Line A : Rear trailing edge 뒤 30m
```

본 제품은 이 geometry를 제품 설계 근거 중 하나로 사용하고 다음 OEM HMI 정책을 적용한다.

```text
0~10m  → DANGER
10~30m → WARNING
>30m   → No Warning
```

`DANGER / WARNING`이라는 HMI 단계 자체는 본 제품의 사용자 경고정책이며 ISO의 용어를 그대로 재현한 것으로 해석하지 않는다.

### 11.2 SAV / TTC 기준

초기 Rear Closing risk 기준은 SAV이며 TTC 2.5s를 위험 증강 threshold로 사용한다.

### 11.3 적용 범위

제품의 현재 ISO 참조 범위는 다음을 기준으로 한다.

- Straight vehicle
- Forward-driving Side BSD / Lane Change Decision Aid
- Type III functional target
- SAV initial closing-risk target
- Straight-road baseline

본 사양서는 제품 기능 기준을 정의하는 문서이며, 별도의 인증서 또는 ISO full-conformity 선언문으로 사용하지 않는다. 공식 규격 적합성 주장은 요구되는 시험조건과 conformity evidence를 별도 문서에서 관리한다.

---

## 12. 제품 기능 요약표

| 항목 | 표준 사양 |
|---|---|
| 제품 용도 | RV용 Camera Side BSD + Reverse Assist |
| Camera 구성 | LEFT / RIGHT / REAR 3채널 |
| Forward Side 위험영역 | 차량 측면 + Rear 0~30m |
| Forward DANGER | DANGER ROI 또는 Rear ≤10m |
| Forward WARNING | WARNING ROI 또는 Rear 10~30m |
| Forward >30m | 정상 Calibration에서 No Warning |
| 위험 결합 | ROI + Distance severity-max |
| TTC | SAV 2.5s, escalation-only |
| Turn Signal | same-side WARNING→DANGER |
| Forward Display | Yellow WARNING / Red blinking DANGER |
| Reverse Side | ≤1.5m DANGER / ≤3m WARNING |
| Rear Assist | ≤1.5m DANGER / ≤3m WARNING |
| Reverse 대상 | 모델 지원 전체 class |
| Reverse Display | REAR 기본 + LEFT/RIGHT 방향 OSD |
| 출력 | Display / Direction OSD / LED / Buzzer / Fault state |
| 차량 직접제어 | 없음 — Warning-only system |
| Direct ego-speed signal | Forward 핵심 기능의 필수 입력 아님 |
| Standard reference | ISO 17387:2026 Type III design reference |

---

## 13. 사용자 안전 및 시스템 한계

본 제품은 운전자의 주변 확인과 안전 판단을 보조한다.

다음 원칙은 제품 설명서 및 사용자 안내에서 유지한다.

1. 경고가 발생하지 않았다는 사실은 차선변경, 후진 또는 차량 이동이 안전하다는 보증을 의미하지 않는다.
2. 운전자는 시스템 경고 여부와 관계없이 주변 상황을 직접 확인해야 한다.
3. 카메라 오염, 가림, 강한 역광, 악천후, 낮은 조도, 영상 손실 또는 부적절한 Calibration은 감지성능에 영향을 줄 수 있다.
4. Camera/FOV/장착조건이 변경되면 Calibration의 유효성을 확인해야 한다.
5. Fault / Unavailable 상태는 정상 SAFE와 동일하게 해석해서는 안 된다.

---

## 14. 관련 문서 체계

본 제품사양서는 상위 제품정의 문서이며, 상세 내용은 다음 문서에서 관리한다.

| 문서 | 역할 |
|---|---|
| `Side_BSD_시스템_요구사항_명세서.md` | Type III / SAV를 포함한 시험 가능한 단일 규범 시스템 요구사항 |
| `../05_operation/Side_BSD_사용자_안전_및_시스템_한계.md` | 사용자 안전문구 및 시스템 한계 |
| `../02_design/Side_BSD_시스템_기술_설계서.md` | 시스템/알고리즘 설계 |
| `../03_calibration/Side_BSD_캘리브레이션_가이드.md` | 차량/카메라 Calibration 절차 |
| `../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md` | 실영상·보드·차량 Validation 및 인수 절차 |
| `../../../verification/sidebsd/current/test_execution_summary.md` | 현재 Software Verification 및 자동시험 실행 근거 |
| `../../../verification/history/legacy_docs_verification/sidebsd/SIDE_BSD_TYPEIII_SAV_SOFTWARE_CLOSURE.md` | Type III/SAV 기준선 확정 당시의 이력 Snapshot |

제품사양서는 **무엇을 제공하는 제품인가**를 정의하고, SRS는 **소프트웨어가 무엇을 수행해야 하는가**, 설계서/검증서는 **어떻게 구현하고 검증하는가**를 각각 관리한다.

---

## 15. 변경 이력

| Version | Date | Description |
|---|---|---|
| **v1.0** | **2026-09-11** | **회사 표준 제품사양서 형태로 전면 재구성. 제품 개요/특징/시스템 구성/Forward Side BSD/Reverse Assist/HMI/Fail-safe/Calibration/ISO 설계근거/사용자 안전을 통합하고 개발진척·TODO·시험상태 항목을 제거** |
| **v1.1** | **2026-09-13** | **Type III/SAV 추가서의 SRS v2.0 통합과 사용자 안전 문서의 operation 이동에 맞춰 관련 문서 체계를 정합화** |
| **v1.2** | **2026-09-14** | **문서 앞쪽에 용어 및 약어 정의 절 추가. SAV, TTC, Type III, LCDAS, ROI, HMI, OSD, SRS, Fault/Unavailable 등 핵심 용어의 의미와 표준/제품 용어 구분 명확화** |
| **v1.3** | **2026-09-14** | **5.5 TTC 절에 SAV/TTC 2.5s의 실제 접근상황 해석 예시 추가. 절대속도와 상대 접근속도의 차이를 명확화** |
