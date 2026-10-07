# Side BSD 고객·영업 배포용 제품사양서

> **문서 상태:** CUSTOMER DISTRIBUTABLE  
> **문서 ID:** SBSD-PS-EXT-001  
> **버전:** v1.0  
> **작성일:** 2026-09-14  
> **제품군:** Camera-based Side BSD / Reverse Assist for RV  
> **배포 대상:** 사내 영업, 승인된 고객사/OEM 기술협의  
> **배포 기준:** 고객 제안·기술협의·제품 설명에 사용 가능. 웹사이트, 공개 저장소, 불특정 다수 대상 공개 배포는 별도 승인 후 사용한다.  
> **문서 목적:** 제품의 기능, 운용범위, 주요 성능기준 및 사용자 인터페이스를 고객 관점에서 설명한다.

---

## 1. 제품 개요

본 제품은 RV·모터홈과 같이 차체가 길고 운전석에서 차량 측면과 후방을 직접 확인하기 어려운 차량을 위한 **카메라 기반 Side Blind Spot Detection(BSD) 및 Reverse Assist 시스템**이다.

LEFT / RIGHT Side Camera와 REAR Camera를 이용하여 차량 측면·측후방 및 후진 시 주변 객체를 감지하고, 위험 수준에 따라 Display, 방향 표시, LED 및 Buzzer를 통해 운전자에게 정보를 제공한다.

본 제품은 **경고 전용 운전자 보조 시스템**이며 차량의 제동, 조향, 가속 또는 구동을 직접 제어하지 않는다.

제품은 다음 두 가지 운용 기능으로 구성된다.

| 기능 | 운용 상태 | 목적 |
|---|---|---|
| **Forward Side BSD** | 전진 주행 | 좌/우 측면 및 측후방 차선변경 위험 보조 경고 |
| **Reverse Assist** | 후진 | 좌/우 측후방 및 후방 근접 장애물 보조 경고 |

---

## 2. 주요 용어

| 용어 / 약어 | 의미 |
|---|---|
| **BSD** | Blind Spot Detection. 차량 측면 및 측후방의 사각영역 위험을 운전자에게 알리는 기능 |
| **LCDAS** | Lane Change Decision Aid System. 차선 변경 시 주변 차량과의 충돌 위험을 운전자에게 알리는 시스템 |
| **Type III** | ISO 17387의 LCDAS 기능 개념 중 Blind Spot Warning과 Closing Vehicle Warning을 함께 제공하는 유형 |
| **SAV** | Slow Approaching Vehicle. 후방에서 접근하는 차량의 상대 접근위험을 평가하기 위한 ISO 17387의 분류 중 하나 |
| **TTC** | Time To Collision. 현재 상대 접근상태가 유지될 경우 예상되는 충돌까지의 시간 |
| **Closing Speed** | 후방 대상 차량이 본 차량에 접근하는 상대속도 |
| **HMI** | Human-Machine Interface. Display, OSD, LED, Buzzer 등 운전자 경고 인터페이스 |
| **OSD** | On-Screen Display. 카메라 영상 위에 표시되는 방향·상태·경고 그래픽 |
| **Fault / Unavailable** | 해당 카메라 또는 기능의 정상적인 위험판단을 사용할 수 없는 상태 |

> `DANGER`, `WARNING`, `SAFE/NORMAL`은 본 제품의 운전자 경고정책을 설명하기 위한 제품 용어이다.

---

## 3. 시스템 구성

### 3.1 입력

| 입력 | 용도 |
|---|---|
| LEFT Camera | 좌측 Forward Side BSD / Reverse Assist |
| RIGHT Camera | 우측 Forward Side BSD / Reverse Assist |
| REAR Camera | 후방 영상 / Reverse Assist |
| LEFT Turn Signal | 좌측 차선변경 의도 반영 |
| RIGHT Turn Signal | 우측 차선변경 의도 반영 |
| Reverse Signal | Reverse Assist 전환 |

### 3.2 출력

| 출력 | 기능 |
|---|---|
| Display | Camera 영상 및 방향별 WARNING/DANGER 표시 |
| Direction OSD | 후방/측면 영상의 좌·우 방향 경고 표시 |
| LED | 차량 HMI 연동용 방향 경고 |
| Buzzer | 위험상황의 청각 경고 |
| Fault / Unavailable | 정상 비경고 상태와 구분되는 기능상태 표시 |

차량별 전기 인터페이스와 HMI 연동방식은 적용 차종 및 고객 요구에 따라 협의한다.

---

## 4. Forward Side BSD 기능

### 4.1 감지 및 경고 영역

Forward Side BSD는 차량 좌우 측면과 차량 후단 기준 최대 약 30m 영역을 주요 경고범위로 사용한다.

| 영역 | 기본 경고 수준 |
|---|---|
| 차량 측면 위험구간 | DANGER |
| 차량 후단 기준 0~10m | DANGER |
| 차량 후단 기준 10~30m | WARNING |
| 정상 적용조건에서 30m 초과 | No Warning |

차량 형상, 카메라 장착위치 및 렌즈 조건에 따라 실제 영상상의 감지영역은 차량별 적용과정에서 조정된다.

### 4.2 TTC 기반 접근위험 판단

후방 접근 차량은 거리뿐 아니라 **상대 접근속도와 TTC(Time To Collision)**를 함께 고려하여 위험도를 판단한다.

본 제품의 SAV 기준 TTC 위험도 격상 기준은 다음과 같다.

```text
TTC Threshold = 2.5 s
```

| 조건 | 운전자 경고 |
|---|---|
| 기본 DANGER | DANGER 유지 |
| WARNING 영역 + TTC > 2.5s | WARNING 유지 |
| WARNING 영역 + TTC ≤ 2.5s | DANGER 격상 |
| TTC 계산이 유효하지 않은 경우 | 기존 거리/영역 기준 경고 유지 |

#### TTC 2.5s 이해 예시

TTC는 특정 차량의 절대속도를 의미하지 않고 **차량 간 거리와 상대 접근속도**에 의해 결정된다.

```text
본 차량 속도      = 80 km/h
후방 차량 속도    = 116 km/h
상대 접근속도     = 36 km/h = 10 m/s
후방 차량 거리    = 25 m

TTC = 25 m / 10 m/s = 2.5 s
```

즉, 뒤 차량이 본 차량보다 약 36 km/h 빠르게 25m 후방에서 접근한다면 TTC는 약 2.5초가 되며, 이와 같은 빠른 접근상황에서는 위험도를 DANGER로 높여 운전자에게 더 적극적으로 알린다.

### 4.3 Turn Signal 연동

운전자의 차선변경 의도를 반영하기 위해 동일 방향 Turn Signal이 활성화된 상태에서 해당 방향의 위험도가 높으면 경고를 강화한다.

반대 방향 Turn Signal은 다른 방향의 위험판정에 영향을 주지 않는다.

---

## 5. Forward HMI

LEFT / RIGHT 방향은 독립적으로 경고를 표시할 수 있다.

| 상태 | 표준 표시 |
|---|---|
| NORMAL | 경고 표시 없음 |
| WARNING | 해당 방향 Yellow 계열 시각 경고 |
| DANGER | 해당 방향 Red 계열 시각 경고 및 점멸 |

후방 영상을 사용하는 경우에도 LEFT/RIGHT Side BSD 상태를 후방영상 좌·우에 동시에 표시할 수 있다.

DANGER 상태에서 운전자의 동일 방향 Turn Signal이 활성화된 경우 Buzzer를 통해 주의를 강화할 수 있다.

차량별 색상, 음색, 점멸패턴 및 전기 인터페이스는 고객 HMI 요구에 따라 조정할 수 있다.

---

## 6. Reverse Assist

Reverse Signal이 활성화되면 LEFT / RIGHT Side Camera와 REAR Camera를 이용하여 후진 경로 주변의 근거리 장애물을 감지한다.

표준 경고거리는 다음과 같다.

| 채널 | DANGER | WARNING |
|---|---:|---:|
| LEFT Reverse Assist | ≤ 1.5m | ≤ 3.0m |
| RIGHT Reverse Assist | ≤ 1.5m | ≤ 3.0m |
| REAR Reverse Assist | ≤ 1.5m | ≤ 3.0m |

후진 중에는 REAR 화면을 기본 영상으로 유지하면서 좌·우 위험상태를 방향별로 함께 표시할 수 있다.

Reverse Assist는 단안 영상 기반의 **근접 장애물 보조 기능**이며, 별도의 횡방향 충돌궤적 예측형 RCTA 기능과는 구분된다.

---

## 7. Fault / Unavailable 및 사용자 안전

시스템은 정상적으로 감지대상이 없어 경고하지 않는 상태와 카메라 또는 기능을 정상적으로 사용할 수 없는 상태를 구분한다.

특정 카메라 채널에 이상이 발생한 경우 해당 방향의 기능이 제한될 수 있으며, 운전자는 Fault / Unavailable 표시가 있는 경우 해당 방향의 시스템 지원을 전제로 주행해서는 안 된다.

본 제품은 운전자의 주변 확인과 안전 판단을 보조하는 시스템이며 다음 원칙을 따른다.

1. 경고가 발생하지 않았다는 사실은 차선변경, 후진 또는 차량 이동이 안전하다는 보증을 의미하지 않는다.
2. 운전자는 시스템 경고 여부와 관계없이 주변 상황을 직접 확인해야 한다.
3. 카메라 오염, 가림, 강한 역광, 악천후, 낮은 조도 또는 영상 손실은 감지성능에 영향을 줄 수 있다.
4. 차량 형상, 카메라, 렌즈 또는 장착조건이 변경되는 경우 적용상태를 재확인해야 한다.

---

## 8. 적용 기준 및 차량 적용

Forward Side BSD는 **ISO 17387:2026 — Lane Change Decision Aid Systems(LCDAS)**의 Type III 기능 개념을 설계 참조로 사용한다.

현재 제품의 주요 적용범위는 다음과 같다.

- Straight vehicle / RV / Motorhome
- Forward-driving Side BSD / Lane Change Decision Aid
- Type III functional target
- SAV initial closing-risk target
- Straight-road baseline

본 문서는 제품 기능과 고객 적용을 위한 설명문서이며 **ISO 17387:2026 전체 조항에 대한 공식 인증서 또는 full-conformity 선언문이 아니다.** 규격 적합성 요구가 있는 프로젝트는 고객 요구조건과 시험범위를 별도로 협의한다.

카메라 장착위치, 차량 형상, 렌즈/FOV 및 영상조건에 따라 차량별 Calibration 또는 적용 확인이 필요할 수 있다.

---

## 9. 제품 기능 요약

| 항목 | 표준 사양 |
|---|---|
| 제품 용도 | RV용 Camera Side BSD + Reverse Assist |
| Camera 구성 | LEFT / RIGHT / REAR 3채널 |
| Forward 주요 경고영역 | 차량 측면 + Rear 0~30m |
| Forward DANGER | 차량 측면 위험구간 또는 Rear ≤10m |
| Forward WARNING | Rear 10~30m |
| TTC | SAV 기준 2.5s 위험도 격상 |
| Turn Signal | 동일 방향 위험상황에서 경고 강화 |
| Forward Display | Yellow WARNING / Red DANGER |
| Reverse Side | ≤1.5m DANGER / ≤3m WARNING |
| Rear Assist | ≤1.5m DANGER / ≤3m WARNING |
| Reverse Display | REAR 기본 + LEFT/RIGHT 방향 표시 |
| 출력 | Display / Direction OSD / LED / Buzzer / Fault state |
| 차량 직접제어 | 없음 — Warning-only system |
| Standard reference | ISO 17387:2026 Type III design reference |

---

## 10. 문서 배포 및 사용 기준

본 문서는 **사내 영업 및 승인된 고객사/OEM 기술협의를 위한 배포용 제품사양서**이다.

본 문서의 내용은 고객 제안서, 기술미팅, 제품 기능 설명 및 차량 적용 검토에 인용할 수 있다.

다만 다음 자료는 본 문서의 배포범위에 포함하지 않는다.

- 소스코드 및 소프트웨어 내부 구현
- 세부 알고리즘 결합 로직
- 모델·추론 내부 파라미터
- 차량별 상세 Calibration 절차 및 내부 튜닝값
- 내부 시험코드, 검증 로그 및 개발 이력
- 내부 요구사항서, 기술설계서 및 검증문서 원문

웹사이트, 공개 저장소, 전시회 배포물 등 **불특정 다수에게 공개되는 자료로 전환할 경우에는 별도의 공개용 검토를 거친다.**

---

## 11. 변경 이력

| Version | Date | Description |
|---|---|---|
| **v1.0** | **2026-09-14** | **내부 제품사양서를 기반으로 영업·고객사 배포용 문서 신규 작성. 제품 기능/성능/HMI/안전/적용규격은 유지하고 내부 구현·Calibration 상세·검증경로·개발정보는 제외** |
