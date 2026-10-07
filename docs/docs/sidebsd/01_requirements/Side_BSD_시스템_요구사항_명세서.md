# Side BSD 시스템 요구사항 명세서 (SRS)

> **문서 상태:** CURRENT — Type III / SAV 규범 요구사항 통합
> **문서 ID:** SBSD-SRS-001
> **버전:** v2.0
> **작성일:** 2026-09-13
> **적용 대상:** Side BSD System  
> **Software 기준선:** `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`
> **승인 상태:** SELF-REVIEWED
> **문서 성격:** 규범적 시스템 요구사항 문서 (Normative System Requirements)  
> **제품 설명 문서:** `Side_BSD_제품사양서.md`  
> **기존 설계 Baseline:** `../02_design/Side_BSD_시스템_기술_설계서.md`  
> **현재 검증 Workspace:** `../../../verification/sidebsd/`
> **이력 검증 Snapshot:** `../../../verification/history/legacy_docs_verification/sidebsd/REQUIREMENT_TRACEABILITY_MATRIX.md` v0.9
> **Validation 절차 문서:** `../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md`
> **사용자 안전/한계 문서:** `../05_operation/Side_BSD_사용자_안전_및_시스템_한계.md`

---

## 1. 문서 목적

본 문서는 이미 개발되어 있는 Side BSD 시스템의 기능과 제약사항을 정식 요구사항으로 정리하고,
향후 국제규격 GAP 분석과 논리검증/단위테스트 보완의 기준으로 사용하기 위한 시스템 요구사항 명세서이다.

본 작업의 목적은 기존 시스템을 처음부터 다시 설계하는 것이 아니다.

현재 개발 상태는 다음을 전제로 한다.

```text
기존 제품 정책
    ↓
현재 시스템 기술 설계서
    ↓
현재 구현 코드
    ↓
기존 논리검증 / 단위테스트 완료
    ↓
[현재 단계]
기존 내용을 정식 요구사항으로 역추출 및 문서화
    ↓
요구사항 ↔ 설계 ↔ 테스트 추적성 정리
    ↓
국제규격 GAP 추가
    ↓
실제로 누락된 항목만 설계/코드/테스트 보완
    ↓
실차 Calibration / Validation
```

따라서 본 문서는 기존 구현 Baseline을 최대한 보존하면서,
요구사항으로 관리해야 할 제품 동작과 구현 세부사항을 분리한다.

---

## 2. 문서 체계 및 우선순위

Side BSD 문서의 역할은 다음과 같이 구분한다.

| 문서 | 역할 |
|---|---|
| `Side_BSD_제품사양서.md` | 고객/사내 설명용 제품 기능 요약 |
| **본 문서** | 제품이 만족해야 할 정식 시스템 요구사항 |
| `Side_BSD_시스템_기술_설계서.md` | 요구사항을 구현하기 위한 아키텍처/알고리즘/런타임 설계 |
| `Side_BSD_CODEBASE_OVERVIEW.md` | 코드 구조와 의존성 |
| `03_calibration/*` | ROI/거리/Ego Motion 등 보정 절차 |
| `04_validation/*` | 논리검증, 실보드 및 실차 검증 절차 |
| `05_operation/*` | 배포/운영 기준 |
| `05_operation/Side_BSD_사용자_안전_및_시스템_한계.md` | 사용자 안전 안내 및 시스템 한계 최소 문구 Baseline |

본 문서는 Side BSD System의 **단일 규범 요구사항 기준**이다. 과거
`../history/Side_BSD_TypeIII_SAV_요구사항_추가서_v1.1.md`의 요구사항은 v2.0에 통합되었으며,
해당 추가서는 변경 근거 확인을 위한 이력 문서로만 보존한다.

Baseline 확정 이후 변경 흐름은 다음을 원칙으로 한다.

```text
요구사항 변경
→ 설계 영향 검토
→ 코드 영향 검토/수정
→ 논리검증 및 단위테스트
→ 필요 시 실차 검증
→ 관련 문서 갱신
```

---

## 3. 요구사항 출처 및 상태 표기

### 3.1 Source

| 표기 | 의미 |
|---|---|
| `BASELINE` | 현재 시스템 기술 설계서와 구현에 이미 존재하는 동작을 요구사항으로 승격 |
| `POLICY` | 제품 목적/운용 정책으로 결정된 요구사항 |
| `STANDARD` | 국제규격 검토로 추가되거나 확인되어야 할 요구사항 |
| `FIELD-TBD` | 실차/실보드 데이터가 있어야 최종 확정 가능한 항목 |

### 3.2 Implementation Status

| 표기 | 의미 |
|---|---|
| `Implemented` | 현재 기술 설계 Baseline에 구현된 기능 |
| `Interface Pending` | SW 인터페이스/논리는 존재하지만 실제 차량/보드 I/O 확인 필요 |
| `TBD` | 최종 요구가 아직 확정되지 않음 |
| `New GAP` | 외부 규격 검토 결과 새로 추가될 수 있는 요구사항 |

### 3.3 Verification Status

검증 열의 기존 Requirement ID 상태는 **RTM v0.9 이력 Snapshot**을 기반으로 하며,
Type III / SAV 통합 요구사항은 해당 추가서의 검증 Snapshot을 함께 반영한다.
현재 검증의 authoritative workspace는 `../../../verification/sidebsd/`이고, 과거 검증 캠페인을
요약하던 `Logic/Unit PASS*`나 이력 RTM만으로 현행 개별 판정을 확대하지 않는다.

| Class | 의미 |
|---|---|
| `A` | 현재 요구 범위에 충분한 기존 검증 근거가 있음 |
| `B` | production 기반 자동 테스트 보완 필요 |
| `C` | 요구 범위 결정 또는 설계/코드/운영 계약 GAP이 열려 있음 |
| `D` | 외부 표준 원문 GAP 검토 필요 |
| `E` | 실차/실보드/Calibration/환경/성능 검증 필요 |
| `A/E`, `B/E`, `C/E` | 소프트웨어 검증 상태와 필드/보드 미완료 상태를 병기 |

Evidence 등급은 다음과 같이 구분한다.

| Evidence | 의미 |
|---|---|
| `EV-1` | 실제 production 클래스/함수/정책을 호출한 executable test |
| `EV-2` | production 소스, 상태 전이, config 소비 경로의 static/logical verification |
| `EV-L` | 과거 버그 재현용 로직 복제/mock test. 현행 production PASS 근거로 사용하지 않음 |

검증 열의 기존 Class는 RTM v0.9 Snapshot과 동일하며, 괄호의 Evidence/미완료 범위는 요약 표기이다.
`E`가 포함된 항목은 자동화 테스트 PASS만으로 제품 검증 완료로 간주하지 않는다.

---

## 4. 적용 표준 및 참고 기준

### 4.1 ISO 17387:2026

**ISO 17387:2026 — Intelligent transport systems — Lane change decision aid systems (LCDAS) — Performance requirements and test procedures**

전진 주행 Side BSD 기능의 국제규격 적합성 검토 기준으로 사용한다.

공개된 ISO 정보에서 확인되는 적용 범위는 다음과 같다.

- Lane Change Decision Aid System의 시스템 요구사항과 시험방법
- 자차 측면 또는 후방 인접 차선에서 자차와 동일 방향으로 이동하는 차량에 대한 충돌 위험 경고
- 전진 주행 승용차, 밴, 비굴절식(straight) 트럭 대상
- 모터사이클 및 굴절식 차량은 적용범위에서 제외

본 프로젝트는 ISO 17387:2026 전체 원문에 대한 조항별 GAP 분석이 완료되기 전까지
**ISO 17387:2026 완전 준수(fully compliant)를 주장하지 않는다.**

ISO 세부 Clause 번호와 시험조건은 전체 원문 확인 후 별도 Traceability Matrix에 기록한다.
검증되지 않은 조항 번호를 본 문서에 임의로 기재하지 않는다.

### 4.2 ISO/IEC/IEEE 29148:2018

요구사항 공학 및 요구사항 문서 구조의 참고 기준으로 사용한다.

현재 국제표준은 ISO/IEC/IEEE 29148:2018이며,
본 프로젝트 규모에 맞게 과도한 산출물을 만들기보다는 다음 특성을 우선한다.

- 명확성
- 단일 의미성
- 필요성
- 실현 가능성
- 검증 가능성
- 추적 가능성

---

# 5. 시스템 목적 및 경계

## 5.1 목적

Side BSD 시스템은 차량의 좌측/우측 및 후방 카메라 영상을 기반으로 운전자가 직접 확인하기 어려운 영역의 위험 대상을 감지하고,
Display, LED 및 Buzzer를 통해 운전자에게 위험 정보를 제공하는 **경고 전용 운전자 보조 시스템**이다.

## 5.2 시스템 경계

| 항목 | 시스템 조건 |
|---|---|
| LEFT Camera | 입력 사용 |
| RIGHT Camera | 입력 사용 |
| REAR Camera | 입력 사용 |
| LEFT Turn Signal | 입력 사용 |
| RIGHT Turn Signal | 입력 사용 |
| Reverse 상태 | Reverse Assist 운용을 위해 사용 |
| **Brake Signal** | **입력 없음 / 사용하지 않음** |
| Vehicle Speed | 직접 입력 없음 |
| Steering Angle | 입력 없음 |
| Display | 경고 및 영상 출력 |
| LED | 경고/방향 출력 |
| Buzzer | 청각 경고 출력 |
| Vehicle Control | 출력 없음 |

차량 속도를 직접 입력받지 않는 것은 현재 Side BSD의 시스템 제약조건이다.
차량의 주행 상태를 추정하는 구체적인 알고리즘은 시스템 기술 설계서에서 정의한다.

---

# 6. 요구사항 ID 규칙

| Prefix | 영역 |
|---|---|
| `SBSD-SYS` | 시스템 공통 |
| `SBSD-IF` | 외부 입력/출력 |
| `SBSD-FWD` | Forward Side BSD |
| `SBSD-MODE` | 운용 Mode / 상태 전환 |
| `SBSD-REV` | Reverse / Parking Assist |
| `SBSD-HMI` | Display / LED / Buzzer |
| `SBSD-SAF` | 안전 및 Fail-safe 성격 |
| `SBSD-DIAG` | Fault / Availability / stale 방어 |
| `SBSD-CAL` | Calibration |
| `SBSD-PERF` | 성능 |
| `SBSD-TEST` | TEST / PRODUCTION 격리 |
| `SBSD-VER` | 검증/추적성 |
| `SBSD-STD` | 국제규격 GAP |

검증 방법은 `T(Test)`, `I(Inspection)`, `A(Analysis)`, `D(Demonstration)`로 표시한다.

---

# 7. 시스템 공통 요구사항

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-SYS-001 | 시스템은 운전자에게 주변 위험을 알리는 보조 경고 시스템이어야 한다. | POLICY | Implemented | A (EV-2) | I |
| SBSD-SYS-002 | 시스템은 차량의 제동, 조향, 가속 또는 구동계를 직접 제어해서는 안 된다. | POLICY | Implemented | A (EV-2) | I/T |
| SBSD-SYS-003 | 시스템 고장 또는 전원 OFF가 차량의 기본 주행 제어 기능을 직접 수행하거나 대체해서는 안 된다. | POLICY | Implemented | A/E (EV-2 + Board Pending) | I |
| SBSD-SYS-004 | 시스템은 LEFT, RIGHT, REAR 세 카메라 채널을 지원해야 한다. | BASELINE | Implemented | A/E (EV-1/EV-2 + Board/Vehicle Pending) | T |
| SBSD-SYS-005 | Forward Side BSD와 Reverse Assist는 서로 다른 위험판단 정책으로 운용되어야 한다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-SYS-006 | 정상적으로 위험판단을 수행할 수 없는 상태는 정상 `SAFE` 상태와 구분되어야 한다. | BASELINE | Implemented | B/E (EV-1 partial + Board/Vehicle Pending) | T |
| SBSD-SYS-007 | 시스템의 경고 미발생은 차선 변경 또는 차량 이동의 안전을 보장하는 의미로 사용되어서는 안 된다. | POLICY | Implemented | A (EV-2) | I |

---

# 8. 외부 입력/출력 요구사항

## 8.1 Camera / Vehicle State Input

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-IF-001 | 시스템은 LEFT Camera 영상 입력을 수신할 수 있어야 한다. | BASELINE | Implemented | E (Vehicle Pending) | T |
| SBSD-IF-002 | 시스템은 RIGHT Camera 영상 입력을 수신할 수 있어야 한다. | BASELINE | Implemented | E (Vehicle Pending) | T |
| SBSD-IF-003 | 시스템은 REAR Camera 영상 입력을 수신할 수 있어야 한다. | BASELINE | Implemented | E (Vehicle Pending) | T |
| SBSD-IF-004 | 시스템은 LEFT Turn Signal 상태를 입력받아 좌측 경보 정책에 사용할 수 있어야 한다. | BASELINE | Interface Pending | B/E (EV-2 + Vehicle Pending) | T |
| SBSD-IF-005 | 시스템은 RIGHT Turn Signal 상태를 입력받아 우측 경보 정책에 사용할 수 있어야 한다. | BASELINE | Interface Pending | B/E (EV-2 + Vehicle Pending) | T |
| SBSD-IF-006 | Reverse Assist 사용 시 시스템은 실제 Reverse 상태를 판별할 수 있는 입력을 사용해야 한다. | BASELINE | Interface Pending | A/E (EV-1 guard + Vehicle Pending) | T |
| SBSD-IF-007 | 기본 Side BSD 기능은 차량 속도 신호의 직접 입력을 필수 조건으로 요구해서는 안 된다. | POLICY | Implemented | A (EV-2) | T |
| SBSD-IF-008 | 차량 속도 직접 입력 없이도 Forward Side BSD의 필수 위험판단 기능이 수행될 수 있어야 한다. | BASELINE | Implemented | B/E (EV-1/EV-2 partial + Vehicle Pending) | T |

> **Brake 입력은 존재하지 않으므로 Side BSD 요구사항 및 인터페이스 대상에서 제외한다.**

## 8.2 Output

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-IF-020 | 시스템은 Display를 통해 영상 및 경고 정보를 출력할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1 + Board Pending) | T/D |
| SBSD-IF-021 | 시스템은 LED를 통해 경고 방향 또는 경고 상태를 운전자에게 제공할 수 있어야 한다. | POLICY | Interface Pending | E (Board/Vehicle Pending) | T/D |
| SBSD-IF-022 | 시스템은 Buzzer를 통해 필요한 청각 경고를 제공할 수 있어야 한다. | POLICY | Interface Pending | A/E (EV-1 + Board/Vehicle Pending) | T/D |
| SBSD-IF-023 | 시스템은 차량의 제동/조향/가속 제어를 위한 출력 신호를 제공해서는 안 된다. | POLICY | Implemented | A (EV-2) | I/T |

---

# 9. Forward Side BSD 요구사항

## 9.1 기본 기능

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-FWD-001 | Forward Side BSD는 전진 운용 상태에서 좌측 및 우측 감지 영역을 감시해야 한다. | BASELINE | Implemented | B/E (EV-1 partial + Vehicle Pending) | T |
| SBSD-FWD-002 | 시스템은 차량 측면 또는 측후방의 유효 위험영역에 존재하는 잠재 위험 대상을 판별할 수 있어야 한다. | BASELINE | Implemented | B/E (EV-1 partial + Vehicle Pending) | T |
| SBSD-FWD-003 | 좌측과 우측 위험판단은 논리적으로 독립되어야 하며 각 방향의 경보 결과를 별도로 유지할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1 + Vehicle/HMI Pending) | T |
| SBSD-FWD-004 | 위험판단은 단순 검출 여부만이 아니라 해당 대상이 유효 감지영역에 존재하는지 고려해야 한다. | BASELINE | Implemented | B/E (EV-1 partial + Vehicle Pending) | T |
| SBSD-FWD-005 | 시스템은 위험영역 내 대상에 대해 실제 거리 기반 경보 판단이 가능해야 한다. | BASELINE | Implemented | A/E (EV-1; C-01 FIXED/VERIFIED + Vehicle Pending) | T |
| SBSD-FWD-006 | 시스템은 프레임 간 대상 연속성을 활용하여 순간적인 검출 변화 때문에 경보가 과도하게 진동하지 않도록 해야 한다. | BASELINE | Implemented | B/E (EV-2 + Vehicle Pending) | T |
| SBSD-FWD-007 | 시스템은 위험이 감소하거나 멀어지는 대상에 대해 불필요한 경보가 지속되지 않도록 해야 한다. 단, receding, non-closing, TTC invalid 또는 velocity 상태만으로 유효 DANGER/WARNING ROI나 0~30 m 거리 조건의 기본경보를 SAFE로 낮춰서는 안 된다. | BASELINE | Implemented | A/E (EV-1 + Vehicle Pending) | T |
| SBSD-FWD-008 | 시스템은 반대 방향 통과 차량 등 실제 차선변경 위험과 관계없는 대상에 대한 불필요 경보를 줄이는 정책을 가져야 한다. 이 억제 정책은 유효 ROI 또는 0~30 m 거리 조건이 유지하는 기본경보를 단독으로 downgrade해서는 안 된다. | BASELINE | Implemented | A/E (EV-1 + Vehicle Pending) | T |
| SBSD-FWD-009 | 오경보 억제 정책은 실제 접근 위험 대상의 경고 누락을 유발하지 않는지 실차에서 확인되어야 한다. | FIELD-TBD | Implemented | A/E (EV-1; C-01 FIXED/VERIFIED + Vehicle Pending) | T/A |

## 9.2 ROI / 거리 / TTC 경보 정책

현재 프로젝트 Baseline:

```text
DANGER ROI  = 차량 측면 사각지대 + rear trailing edge 뒤 0~10 m 절대영역
WARNING ROI = DANGER ROI 전체 포함 + rear 10~30 m 영역

ROI_level:
    DANGER ROI  -> DANGER
    WARNING ROI -> WARNING
    ROI 밖      -> SAFE

Distance_level:
    distance <= 10.0 m -> DANGER
    distance <= 30.0 m -> WARNING
    distance > 30.0 m  -> SAFE

base_level = max(ROI_level, Distance_level)
```

Rear distance의 원점은 RV rear trailing edge다. 차량 측면의 긴 blind-side 구간은
rear distance의 음수값으로 환산하지 않고 Image ROI occupancy로 관리한다.
ROI와 Distance는 서로 독립된 방어층이며, 한 경로의 오차만으로 다른 경로가 유지하는
기본 위험도를 낮출 수 없다.

이 값과 정책은 현재 제품 정책/구현 Baseline이며 ISO 17387:2026의 특정 요구값과
동일하거나 해당 표준의 완전 준수를 입증한다고 간주하지 않는다.

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-FWD-010 | Forward Side BSD는 rear distance `10.0 m 이하`를 DANGER로 판정해야 한다. 또한 DANGER ROI는 거리추정 결과와 독립적으로 DANGER를 유지해야 한다. | BASELINE | Implemented | A/E (EV-1; C-01 FIXED/VERIFIED + Vehicle Pending) | T |
| SBSD-FWD-011 | Forward Side BSD는 rear distance `10.0 m 초과 30.0 m 이하`를 WARNING으로 판정해야 한다. 또한 WARNING ROI는 거리추정 결과와 독립적으로 최소 WARNING을 유지해야 한다. | BASELINE | Implemented | A/E (EV-1 + Vehicle Pending) | T |
| SBSD-FWD-012 | 10 m / 30 m 거리 경계는 실제 장착 차량의 거리 Calibration 완료 후 실차에서 최종 검증되어야 한다. | FIELD-TBD | Implemented | E (Calibration/Vehicle Pending) | T |
| SBSD-FWD-013 | 거리 경계값이 변경될 경우 요구사항, Calibration, 설정 및 시험 기준의 영향을 함께 검토해야 한다. | POLICY | Implemented | A (EV-2) | I |
| SBSD-FWD-014 | 10~30 m 기본 WARNING 대상의 신뢰 가능한 TTC가 `2.5 s 이하`이면 DANGER로 격상해야 한다. TTC는 기본 위험도를 낮추는 조건으로 사용해서는 안 되며, TTC UNKNOWN은 SAFE의 근거가 아니라 격상 불가 상태로 취급해야 한다. | POLICY | Implemented | A/E (EV-1 + Vehicle TTC Pending) | T |
| SBSD-FWD-015 | 실제 대상이 WARNING ROI 밖에 있고 rear distance가 30 m를 초과할 때 사용자 경고와 TTC 조기경고를 제공하지 않아야 한다. 거리추정값만 30 m를 초과했다는 이유로 WARNING/DANGER ROI 안의 대상을 SAFE로 낮춰서는 안 된다. | POLICY | Implemented | A/E (EV-1 + Physical Boundary Pending) | T |

최소 production-path 검증 시나리오는 다음을 포함한다.

```text
DANGER ROI + 잘못된 20m 추정 -> DANGER
WARNING-only ROI + 10.0m      -> DANGER
WARNING-only ROI + 10m+eps    -> WARNING
WARNING-only ROI + 30.0m      -> WARNING
WARNING ROI + >30m 추정       -> WARNING (ROI fail-safe)
7m + velocity=None            -> DANGER
20m + velocity=None/receding  -> WARNING
20m / 8.0m/s                  -> TTC 2.5s -> DANGER
20m / 7.99m/s                 -> TTC >2.5s -> WARNING
```

실제 `>30 m SAFE` 물리경계, ROI 경계, 거리 정확도 및 SAV/TTC 정확도는 녹화영상과
실차 Calibration evidence로 최종 검증한다.

## 9.3 Turn Signal 경보 격상

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-FWD-020 | LEFT WARNING 대상이 존재하는 상태에서 LEFT Turn Signal이 활성화되면 현재 제품 정책에 따라 해당 경보를 DANGER로 격상해야 한다. | BASELINE | Implemented | B/E (EV-2 + Vehicle Pending) | T |
| SBSD-FWD-021 | RIGHT WARNING 대상이 존재하는 상태에서 RIGHT Turn Signal이 활성화되면 현재 제품 정책에 따라 해당 경보를 DANGER로 격상해야 한다. | BASELINE | Implemented | B/E (EV-2 + Vehicle Pending) | T |
| SBSD-FWD-022 | 반대 방향 Turn Signal이 해당 측 경보를 잘못 격상시켜서는 안 된다. | BASELINE | Implemented | B/E (EV-2 + Vehicle Pending) | T |

Turn Signal과 TTC는 `base_level`보다 낮은 결과를 만들 수 없다.

## 9.4 차량 속도 직접 입력이 없는 조건의 자차 운동상태

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-FWD-030 | 시스템은 차량 속도 직접 입력 없이 경보 및 오경보 억제에 필요한 자차 운동상태를 추정할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1 + Vehicle Pending) | T |
| SBSD-FWD-031 | 자차 운동상태 추정은 LEFT/RIGHT 중 한쪽의 일시적인 영상 특성 변화만으로 전체 상태가 부당하게 변경되지 않도록 복수 카메라 정보를 활용할 수 있어야 한다. | BASELINE | Implemented | B/E (EV-1 partial + Vehicle Pending) | T |
| SBSD-FWD-032 | 자차 운동상태 판단이 일시적으로 불안정하더라도 실제 위험 대상이 단순히 정상 SAFE로 소거되는 동작이 발생하지 않는지 검증해야 한다. | POLICY | Implemented | B/E (EV-1 partial + Vehicle Pending) | T |

> `STOPPED / LOW_SPEED / MOVING / HIGHWAY`, Optical Flow 알고리즘 및 각 threshold 값은 **설계/Calibration 항목**이며 본 요구사항에서 특정 구현방식으로 강제하지 않는다.

---

# 10. Mode / Session 전환 요구사항

현재 기술 설계 Baseline은 `FORWARD_BSD ↔ REVERSE_ASSIST` 전환을 단순 flag 변경이 아닌
운용 세션 경계로 처리한다.

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-MODE-001 | Reverse 상태가 활성화되면 시스템은 `FORWARD_BSD`에서 `REVERSE_ASSIST` 정책으로 전환해야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-MODE-002 | Reverse 상태가 해제되면 시스템은 `REVERSE_ASSIST`에서 `FORWARD_BSD` 정책으로 복귀해야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-MODE-003 | Mode 전환 시 이전 Mode의 경보 유지상태, 오래된 판단결과 및 일시 상태가 새 Mode의 경보 판단에 영향을 미쳐서는 안 된다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-MODE-004 | Mode 전환 후 최초 처리 구간에서 LEFT/RIGHT 상태가 서로 다른 Mode의 결과로 혼재되지 않아야 한다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-MODE-005 | Reverse 해제 후 Forward BSD로 복귀할 때 자차 운동상태는 가능한 최신 상태를 사용할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1/EV-2; field/board pending) | T |
| SBSD-MODE-006 | 유효시간이 남아 있는 이전 Mode의 결과라도 운용 세션이 변경되면 현재 Mode의 경보에 재사용해서는 안 된다. | BASELINE | Implemented | A (EV-1) | T |

---

# 11. Reverse / Parking Assist 요구사항

9.2절의 ROI+Distance fusion 및 TTC 정책은 Forward Side BSD에만 적용한다.
Reverse Assist는 기존 `1.5 m DANGER / 3.0 m WARNING`, all-class detection 및
Turn Signal 비연동 정책을 유지한다.

> 본 절은 제품 자체의 Reverse Assist 요구사항이며 ISO 17387:2026 적용범위와 분리하여 관리한다.

## 11.1 기본 Reverse Assist

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-REV-001 | Reverse 상태에서는 REAR Camera를 주 후방 감지 채널로 사용할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-002 | Reverse 상태에서는 LEFT와 RIGHT Side Camera를 모두 근접/교차 위험 감지에 사용할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-003 | Reverse 상태에서는 LEFT와 RIGHT가 한쪽 방향에만 편향되지 않도록 모두 지속적으로 위험판단 기회를 가져야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-004 | Reverse Assist는 정지 장애물을 포함한 근접 위험 대상을 감지할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-005 | Reverse Assist는 Forward Side BSD의 멀어짐/상대속도 기반 억제 정책 때문에 정지 또는 속도 미확정 대상이 누락되어서는 안 된다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-006 | Reverse Assist의 경보 레벨은 Turn Signal ON/OFF에 의해 변경되어서는 안 된다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-REV-007 | Forward Side BSD에서 사용하는 대상 클래스 제한이 Reverse Assist의 근접 장애물 탐지를 부당하게 제한해서는 안 된다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |

## 11.2 Reverse 거리 정책

현재 프로젝트 Baseline:

```text
DANGER  : distance <= 1.5 m
WARNING : distance <= 3.0 m
```

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-REV-010 | Reverse Assist DANGER 거리 기준은 현재 Baseline에서 `1.5 m 이하`로 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-011 | Reverse Assist WARNING 거리 기준은 현재 Baseline에서 `3.0 m 이하`로 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-012 | LEFT/RIGHT Reverse Assist는 Forward Side BSD와 분리된 Reverse 전용 ROI와 거리 Calibration을 사용해야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-REV-013 | Reverse 전용 Calibration이 유효하지 않은 채널은 정상 Reverse Assist 기능으로 표시해서는 안 된다. | BASELINE | Implemented | A (EV-1) | T |

## 11.3 현재 Reverse Side 기능의 범위 제한

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-REV-020 | 현재 Side Camera 기반 Reverse 기능은 보정된 Reverse ROI에 진입한 근접/교차 위험 대상을 경고하는 기능으로 정의한다. | BASELINE | Implemented | A/E (EV-2; field/board pending) | I/T |
| SBSD-REV-021 | 신뢰성 있는 횡방향 지면좌표/trajectory/TTC 검증이 완료되기 전에는 현재 기능을 예측형 RCTA 성능으로 간주하지 않는다. | POLICY | Implemented | A (EV-2) | I |
| SBSD-REV-022 | 향후 예측형 RCTA 기능을 요구할 경우 별도 요구사항, Calibration 및 실차 Validation을 정의해야 한다. | FIELD-TBD | TBD | E (field/board pending) | I/A |

---

# 12. HMI 요구사항

## 12.1 공통 HMI

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-HMI-001 | 운전자는 시각 출력으로 경고 발생 방향을 식별할 수 있어야 한다. | POLICY | Implemented | B/E (EV-1/EV-2; field/board pending) | D/T |
| SBSD-HMI-002 | 운전자는 시각 출력으로 WARNING과 DANGER 수준을 구분할 수 있어야 한다. | POLICY | Implemented | B/E (EV-1/EV-2; field/board pending) | D/T |
| SBSD-HMI-003 | 시스템은 필요한 경우 Buzzer를 사용하여 시각 경고를 보조해야 한다. | POLICY | Interface Pending | A/E (EV-1; field/board pending) | D/T |
| SBSD-HMI-004 | LEFT/RIGHT/REAR 각 채널의 정상 SAFE 상태와 Unavailable/Fault 상태는 운전자 관점에서 구분되어야 하며, 장애 채널의 상태 표시가 정상 채널의 유효한 경고/HMI 갱신을 방해해서는 안 된다. | BASELINE | Implemented | A/E (C-02 EV-1 production-path PASS; physical HMI/field pending) | T |
| SBSD-HMI-005 | 서로 다른 방향의 유효 위험이 동시에 존재할 경우 한 방향의 경고 때문에 다른 방향의 위험 정보가 논리적으로 소실되어서는 안 된다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |

## 12.2 Forward 표시

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-HMI-010 | Rear 영상을 Display에 표시하는 운용 중에도 LEFT/RIGHT Forward Side BSD 위험판단은 백그라운드에서 계속 수행될 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-HMI-011 | Rear 영상 표시 중 LEFT 또는 RIGHT Side BSD 경고가 발생하면 해당 방향 위험을 운전자가 인지할 수 있도록 화면에 표시해야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | D/T |

## 12.3 Reverse 표시

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-HMI-020 | Reverse 상태에서는 REAR 영상을 주 화면으로 사용해야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-HMI-021 | Reverse 상태에서 LEFT/RIGHT 위험이 발생하면 REAR 화면에 해당 방향의 경고를 함께 표시할 수 있어야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-HMI-022 | Reverse WARNING은 시각 경고를 제공해야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-HMI-023 | Reverse DANGER는 시각 경고와 해당 방향의 청각 경고를 제공해야 한다. | BASELINE | Interface Pending | A/E (EV-1; field/board pending) | T |
| SBSD-HMI-024 | REAR 자체 위험과 LEFT/RIGHT Side 위험이 동시에 존재할 경우 유효한 위험 정보들이 상호 배타적으로 소실되어서는 안 된다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |

> LED 색상, Display 색상, 점멸 패턴, Buzzer 패턴 등의 최종 HMI 세부 사양은 실보드/HMI 확정 시 별도 Baseline으로 고정한다.

---

# 13. Fault / Availability / stale-result 요구사항

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-DIAG-001 | 위험판단에 사용할 수 있는 결과는 정의된 최대 유효시간을 가져야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-DIAG-002 | 최대 유효시간을 초과한 결과는 현재 경보판단에 사용해서는 안 된다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-DIAG-003 | 운용 Mode 또는 세션이 변경된 경우 이전 세션의 늦게 도착한 결과를 현재 세션의 결과로 사용해서는 안 된다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-DIAG-004 | 공통 자원이 정상이고 다른 채널이 동작 가능한 조건에서 LEFT/RIGHT/REAR 중 특정 채널의 camera acquisition failure 또는 처리 가능한 inference exception은 해당 채널에 격리되어야 하며, main loop와 정상 채널의 detection/warning/HMI 갱신은 계속되어야 한다. | BASELINE | Implemented | A/E (C-02-01~05 EV-1 PASS; camera/board/field pending) | T |
| SBSD-DIAG-005 | 채널 fault 복구는 bounded retry/backoff 정책을 사용해야 하며, 반복 실패 중인 채널은 Unavailable 상태로 유지하고 정상적인 fresh inference result가 확보된 후에만 Fault/Unavailable 상태를 해제해야 한다. | BASELINE | Implemented | A/E (Rear EV-1 + C-02-07~09 PASS; board/field pending) | T |
| SBSD-DIAG-006 | 장애 채널의 Fault/Unavailable 상태는 실제 위험 경보와 논리적으로 구분되어야 하며, 장애 전 stale alert와 해당 채널의 buzzer/위험 출력은 현재 위험으로 사용해서는 안 된다. | BASELINE | Implemented | A/E (C-02-05/06 EV-1 PASS; physical output/field pending) | T |
| SBSD-DIAG-007 | Calibration이 유효하지 않은 기능을 정상 SAFE 상태로 간주해서는 안 된다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |

## 13.1 C-02 제품 정책 및 Acceptance Criteria

C-02에는 **Option B — 처리 가능한 채널 fault의 best-effort isolation**을 적용한다. 보장 대상 채널은 LEFT, RIGHT, REAR이며 fault source는 개별 채널의 camera acquisition failure와 main loop가 포착하여 격리할 수 있는 inference exception이다. 공통 자원이 정상이고 하나 이상의 다른 채널이 동작 가능할 때 main loop와 정상 채널 처리를 지속하는 것이 정책의 전제다.

다음 fault는 현 단계 channel isolation 보장 범위에서 제외한다.

- shared model/NPU hard hang
- main-process crash
- 전원 fault
- 공통 I/O fault
- 공통 자원 자체가 정상 동작 불가능한 fault

LEFT와 RIGHT Side worker를 각각 별도 process로 분리하는 것은 현 단계 필수 요구사항이 아니다. 구현 구조와 무관하게 아래 acceptance criteria를 만족해야 한다.

### SBSD-DIAG-004 Acceptance Criteria

- LEFT inference exception 발생 시 RIGHT/REAR 처리와 main loop가 지속된다.
- RIGHT inference exception 발생 시 LEFT/REAR 처리와 main loop가 지속된다.
- LEFT 또는 RIGHT camera read failure 발생 시 동작 가능한 정상 채널의 처리가 지속된다.
- REAR fault 발생 시 LEFT/RIGHT Side 처리가 지속된다.
- fault 채널로 인해 정상 채널의 detection, warning 및 HMI update가 중단되거나 소실되지 않는다.

### SBSD-DIAG-005 Acceptance Criteria

- fault 채널의 retry/recovery는 무한 즉시 retry/restart가 아니라 유한한 시도 또는 시간 간격을 갖는 bounded retry/backoff로 수행된다.
- 반복 실패 중에는 해당 채널을 Unavailable 상태로 유지한다.
- scheduler는 반복 fault 채널을 정상 채널처럼 지속적으로 우선 선택하여 정상 채널의 처리 기회를 침해하지 않는다.
- 해당 채널에서 정상적인 fresh inference result가 확보된 후에만 Fault/Unavailable 상태를 해제한다.

### SBSD-DIAG-006 Acceptance Criteria

- 채널 fault가 확인되면 장애 전 stale alert를 즉시 현재 위험 판단에서 폐기하거나 억제한다.
- 장애 채널의 buzzer 및 위험 출력은 억제하며 Fault/Unavailable 상태를 실제 WARNING/DANGER 경보와 구분한다.

### SBSD-HMI-004 Acceptance Criteria

- 장애 채널은 정상 SAFE가 아니라 Fault/Unavailable로 식별된다.
- 장애 채널의 Fault/Unavailable 표시 또는 출력 억제가 정상 채널의 유효한 warning/HMI update를 방해하지 않는다.

위 acceptance criteria는 `tests/side_bsd/test_c02_fault_isolation_production.py` 9개 production-path fault-injection으로 EV-1 PASS했다. C-02 CODE_GAP은 `FIXED / SOFTWARE_VERIFIED (A/E)`로 전환하며, 실차·실보드·실물 HMI와 제외 fault 범위는 미검증으로 유지한다.

### 현재 Candidate 설정

현재 기술 설계의 결과 유효시간 Candidate:

```text
MAX_INFERENCE_RESULT_AGE_SEC = 0.5 s
```

`0.5 s`는 현재 구현 Baseline이지만 실차 위험 시나리오 검증 전 최종 제품 성능 요구값으로 고정하지 않는다.

---

# 14. TEST / PRODUCTION 격리 요구사항

현재 설계에는 녹화영상 검증 및 실제 차량 입력 미연결 상태를 위한 TEST 전용 기능이 존재한다.
이 기능이 양산 운용으로 유출되지 않는 것은 제품 안전 관점에서 요구사항으로 관리한다.

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-TEST-001 | 시스템은 TEST 운용과 PRODUCTION 운용을 명확히 구분할 수 있어야 한다. | BASELINE | Implemented | A (EV-1/EV-2) | T |
| SBSD-TEST-002 | TEST에서는 오프라인 녹화영상과 검증용 시간정보를 사용할 수 있어야 한다. | BASELINE | Implemented | B (EV-2) | T |
| SBSD-TEST-003 | TEST 전용 ROI direct-alert 기능은 PRODUCTION 운용에서 활성화되어서는 안 된다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-TEST-004 | 실제 Reverse 입력을 대신하는 TEST override는 PRODUCTION 운용에서 무시되어야 한다. | BASELINE | Implemented | A (EV-1) | T |
| SBSD-TEST-005 | TEST 기능이 실제 차량 입력 또는 정상 위험판단 경로를 PRODUCTION에서 우회할 수 없어야 한다. | POLICY | Implemented | A (EV-1) | T |

---

# 15. Calibration 요구사항

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-CAL-001 | Side Camera 장착 위치/화각/차량 형상이 변경되면 해당 ROI의 유효성을 다시 확인해야 한다. | BASELINE | Implemented | E (EV-2; field/board pending) | T |
| SBSD-CAL-002 | 거리 기반 경보를 사용하는 카메라 채널은 실제 장착상태에서 거리 Calibration을 수행해야 한다. | BASELINE | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-CAL-003 | Forward Side BSD의 10 m / 30 m 경계는 실제 설치 차량에서 검증해야 한다. | FIELD-TBD | Implemented | E (field/board pending) | T |
| SBSD-CAL-004 | Reverse Assist의 1.5 m / 3.0 m 경계는 실제 설치 차량에서 별도로 검증해야 한다. | FIELD-TBD | Implemented | A/E (EV-1; field/board pending) | T |
| SBSD-CAL-005 | 차량 속도 직접 입력을 사용하지 않는 자차 운동상태 판단의 최종 threshold는 실제 속도별 주행 데이터로 검증해야 한다. | FIELD-TBD | Implemented | A/E (EV-1; field/board pending) | T/A |
| SBSD-CAL-006 | 코드의 기본값 또는 임시 Calibration 값을 실차 검증 없이 최종 양산값으로 승인해서는 안 된다. | POLICY | Implemented | A (EV-2) | I |
| SBSD-CAL-007 | 적용되는 Calibration 데이터는 Calibration revision과 대상 차량/카메라/장착사양을 식별할 수 있어야 하며, Calibration 유효성에 영향을 줄 수 있는 조건 변경 시 기존 Calibration의 유효성을 재검토하고 필요한 경우 재Calibration 및 재검증을 수행해야 한다. | POLICY | Implemented | A/E (C-03 contract closed; EV-2 + field calibration/validation pending) | I |

### 15.1 Calibration artifact 추적성 및 변경관리 규칙

현재 Side BSD의 차량, 카메라 및 장착조건은 제품 Baseline으로 고정 관리하는 것을 원칙으로 한다. Calibration 추적성의 목적은 이 항목들을 runtime 가변 파라미터로 만드는 것이 아니라, **적용 중인 Calibration이 어떤 승인된 차량/카메라/장착 Baseline을 기준으로 생성·검증되었는지 확인할 수 있게 하는 것**이다.

Calibration 적용/승인 기록은 최소한 다음 정보를 문서 또는 관리되는 artifact 참조를 통해 식별할 수 있어야 한다.

- 적용 차량 또는 vehicle configuration / 관련 사양 revision
- 적용 카메라 configuration 및 채널 / 관련 카메라 사양 revision
- 적용 장착 specification 또는 도면 revision
- Calibration revision 또는 식별자
- 적용된 ROI, 거리 Calibration, Ego Motion threshold 등 해당 Calibration artifact/parameter의 참조
- Calibration 상태 및 검증 기록 참조 (`Candidate`, `Validated`, `Production` 등 프로젝트에서 사용하는 승인 상태)

카메라 장착 위치, 높이, 각도와 같은 기구 조건이 정식 장착도면/사양에서 관리되는 경우 동일 수치를 Calibration artifact에 중복 저장할 필요는 없으며, 해당 장착도면/사양의 revision을 참조할 수 있다.

동일한 승인 vehicle/camera/mounting Baseline과 허용 공차 범위에 속하는 양산 차량은 승인된 공통 Calibration Baseline을 사용할 수 있으며, 개별 차량마다 별도 Calibration을 수행하거나 VIN 단위 Calibration artifact를 생성할 것을 요구하지 않는다.

차량 형상, 카메라 모델/렌즈/화각, 장착 위치·높이·각도, bracket 또는 기타 장착사양 등 **Calibration 유효성에 영향을 줄 수 있는 조건이 변경되면** 기존 Calibration의 영향성을 검토해야 한다. 검토 결과 기존 Calibration이 계속 유효하다고 확인되면 재사용할 수 있고, 영향이 있는 경우 해당 ROI/거리/Ego Motion 등 영향받는 Calibration 항목에 대해 재Calibration 및 필요한 실차 검증을 수행한다.

이 추적성 계약의 확정은 Calibration 값 자체의 정확성이나 실차 적용성 검증 완료를 의미하지 않는다. 실제 Calibration 값, 경보 거리 정확도 및 차량 적용성은 Field/Vehicle Validation으로 별도 완료해야 한다.


---

# 16. 성능 요구사항

현재 실차/실보드 최종 성능 측정이 남아 있으므로,
구현된 Candidate 값을 곧바로 최종 제품 요구값으로 확정하지 않는다.

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-PERF-001 | 위험 조건 성립부터 운전자 경고 출력까지의 최대 허용 지연시간을 실차/실보드 결과에 따라 최종 정의해야 한다. | FIELD-TBD | TBD | E (EV-2; field/board pending) | T |
| SBSD-PERF-002 | LEFT/RIGHT/REAR 각 채널의 최소 유효 처리율 또는 최대 처리 간격을 최종 정의해야 한다. | FIELD-TBD | TBD | E (EV-2; field/board pending) | T |
| SBSD-PERF-003 | 한쪽 Side Camera의 지속 위험 또는 우선 처리 때문에 반대쪽 카메라가 허용 범위를 넘도록 starvation되어서는 안 된다. | BASELINE | Implemented | B/E (EV-1/EV-2; field/board pending) | T |
| SBSD-PERF-004 | Forward Side BSD의 False Negative 허용기준을 실제 위험 시나리오 데이터로 정의해야 한다. | FIELD-TBD | TBD | E (field/board pending) | T/A |
| SBSD-PERF-005 | Forward Side BSD의 False Positive 허용기준을 실제 도로 데이터로 정의해야 한다. | FIELD-TBD | TBD | E (field/board pending) | T/A |
| SBSD-PERF-006 | Optical Flow/자차 운동상태 판단은 터널, 저텍스처 등 일시적인 영상 조건 변화에서 경보정책을 불안정하게 만들지 않는지 검증해야 한다. | BASELINE | Implemented | A/E (EV-1/EV-2; field/board pending) | T |
| SBSD-PERF-007 | 시스템은 목표 타깃 보드에서 장시간 운용 안정성을 검증해야 한다. | FIELD-TBD | TBD | E (field/board pending) | T |

---

# 17. 안전 및 사용자 한계 요구사항

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-SAF-001 | 본 제품은 차량을 직접 제어하지 않는 운전자 보조 경고 시스템으로 운용되어야 한다. | POLICY | Implemented | A (EV-2) | I |
| SBSD-SAF-002 | 제품 설명/사용자 문서는 경고 미발생을 안전 보증으로 표현해서는 안 된다. | POLICY | Implemented | A (EV-2; user safety document inspection) | I |
| SBSD-SAF-003 | 운전자는 Side BSD의 경고 여부와 관계없이 주변 확인과 안전 운전을 수행해야 한다는 시스템 한계를 사용자 문서에 명시해야 한다. | POLICY | Implemented | A (EV-2; user safety document inspection) | I |
| SBSD-SAF-004 | 핵심 입력/Calibration/처리 결과를 신뢰할 수 없는 경우 해당 상태를 정상 SAFE로 오인시켜서는 안 된다. | BASELINE | Implemented | B/E (EV-1/EV-2; field/board pending) | T |
| SBSD-SAF-005 | 이전 Mode/세션의 결과로 인한 stale 경보가 현재 운용상태에 영향을 주지 않아야 한다. | BASELINE | Implemented | A (EV-1) | T |

> `SBSD-SYS-007`, `SBSD-SAF-001`, `SBSD-SAF-002`, `SBSD-SAF-003`의 사용자 문구 Baseline은 `../05_operation/Side_BSD_사용자_안전_및_시스템_한계.md`에서 관리한다. `SBSD-SAF-004`의 동작 검증 상태는 본 문서 작성과 별개로 RTM의 B/E 상태를 유지한다.

---

# 18. ISO 17387:2026 GAP 관리 요구사항

현재 단계에서는 공개된 ISO 범위만 요구사항에 반영한다.
전체 ISO 원문을 확보한 뒤 구체적인 성능/시험조건을 본 Baseline과 비교한다.

| ID | 요구사항 | Source | 구현 | 검증 | 방법 |
|---|---|---|---|---|---|
| SBSD-STD-001 | ISO 17387 적합성 검토 범위는 기본적으로 Forward Side BSD와 Reverse Assist를 구분하여 수행해야 한다. | STANDARD | Implemented | A (EV-2) | I |
| SBSD-STD-002 | ISO 17387 적용대상과 프로젝트 타깃 차량 형상의 관계를 제품 적용차종별로 확인해야 한다. | STANDARD | TBD | D | A/I |
| SBSD-STD-003 | Type III / SAV, TTC `2.5 s`, ROI+Distance fail-safe 정책은 현재 프로젝트 Software Baseline으로 적용하고, ISO 17387의 인접 차선 측면/후방 동일방향 차량 경고 요구와 비교해야 한다. ISO 전체 원문 및 conformity evidence 검토 전에는 이 Baseline을 표준 완전 준수로 표현해서는 안 된다. | POLICY/STANDARD | Implemented | A/D/E (Software evidence 확인, ISO 원문·실차 검증 Pending) | A/T |
| SBSD-STD-004 | ISO 17387 전체 원문에서 요구하는 성능 및 시험방법을 현재 실차 Validation 계획과 대조해야 한다. | STANDARD | New GAP | D | A/T |
| SBSD-STD-005 | ISO 17387 전체 원문 검토가 완료되기 전에는 완전 준수 표현을 사용하지 않는다. | STANDARD | Implemented | A (EV-2) | I |

> ISO 세부 Clause 번호/시험명은 전체 원문에서 직접 확인된 후 Traceability Matrix에 추가한다.

---

# 19. 요구사항 ↔ 기존 설계 Baseline 연결

본 절은 현재 v2.0 Baseline의 상위 Traceability이다. 기존 Requirement ID의 Class와 직접 검증 근거는 RTM v0.9 이력 Snapshot을 보존하고, 현행 판정은 `../../../verification/sidebsd/`의 최신 결과를 따른다.

| 요구사항 영역 | 시스템 기술 설계서 주요 연결 |
|---|---|
| SBSD-SYS / IF | §2 시스템 구성, §11 Hardware I/O |
| SBSD-FWD | §4 좌/우 Side BSD 처리 흐름 |
| SBSD-MODE | §4.1 운용 모드와 상태 전이 |
| SBSD-REV | §4.2 REVERSE_ASSIST, §5 후방 처리 |
| SBSD-HMI | §4.2, §5 및 App 통합 출력 정책 |
| SBSD-DIAG | §5.1 worker 구조, §5.2 후방 세션 보호, §9 결과 cache 및 stale 방어 |
| SBSD-TEST | §10 TEST / PRODUCTION |
| SBSD-CAL | §6 추적과 거리, §7 Ego Motion, §13 알려진 미확정 영역 |
| SBSD-PERF | §8 Scheduler, §9 stale 방어, §13 알려진 미확정 영역 |
| SBSD-STD | 본 요구사항 문서에서 신규 관리 |

기존 세부 **Requirement ID ↔ Test Case** 추적성은 RTM v0.9 이력 Snapshot에 보존되어 있다. v2.0 이후 변경분의 현행 추적성은 `../../../verification/sidebsd/`에서 관리한다.

---

# 20. 현재 검증 상태의 분류

## 20.1 현재 소프트웨어 검증 Baseline

현재 자동화 regression 결과는 다음과 같다.

```text
P1 production-path : 8/8 PASS
C-02 production-path: 9/9 PASS
Side BSD           : 50/50 PASS
Repository         : 144/144 PASS
FAIL               : 0
ERROR              : 0
판정                : C-02 FIXED / SOFTWARE_VERIFIED, C-03 CONTRACT CLOSED
```

이 결과는 RTM v0.9 이력 Snapshot이 Requirement ID별로 직접 연결한 EV-1/EV-2 범위에만 적용한다. C-02 production-path 9건과 C-03 문서/운영 계약 종결을 포함하지만 field/board evidence로 확대하지 않는다.
EV-L은 현행 production PASS 근거로 사용하지 않으며, `NO_CODE_GAP`은 실차/실보드/Calibration/성능/환경/전체 안전성 검증 완료를 의미하지 않는다.

RTM v0.9 이력 Snapshot에서 확인되는 주요 소프트웨어 검증 영역은 다음과 같다.

```text
Forward / Reverse Mode 정책
Forward 거리 경계 논리
Reverse 거리 경계 논리
Turn Signal 경보 격상
Forward/Reverse 필터 분리
Mode 전환 상태 초기화
stale result 폐기
Rear session 보호
Rear fault/timeout/recovery
TEST/PRODUCTION guard
Scheduler 편향 방어
동시 경보 조율
```

개별 요구사항의 충분성과 미완료 범위는 본 문서의 검증 열, RTM v0.9 이력 근거 및 현재 검증 workspace의 최신 결과를 함께 확인한다.

## 20.2 실차/실보드에서 최종 확인할 영역

```text
실제 Camera 입력
실제 Turn Signal / Reverse 입력
Display / LED / Buzzer 실제 출력
실제 ROI
거리 Calibration
10m / 30m 실제 경보경계
1.5m / 3.0m 실제 후진 경보경계
자차 운동상태 threshold
경보 latency
채널별 실제 처리성능
False Positive / False Negative
터널/저텍스처 등 실제 환경 안정성
장시간 실보드 안정성
```

## 20.3 국제규격 GAP 후 새로 확인할 영역

```text
ISO 17387:2026 전체 원문 요구사항
현재 위험영역과 ISO coverage 요구의 GAP
현재 상대운동 정책과 ISO 요구의 GAP
ISO 시험조건과 현재 Validation plan의 GAP
적용 차량 형상/범위
필요 시 HMI/진단 추가 요구
```

---

# 21. Requirement Traceability 현재 규칙

RTM v0.9의 Requirement ID별 연결 결과는 v1.4까지의 이력 추적성 Snapshot으로 사용한다. v2.0 이후 현행 결과는 `../../../verification/sidebsd/`에서 갱신한다.

각 Requirement는 다음 중 하나로 분류한다.

```text
A. Existing Requirement + Existing Test PASS
   → 변경 없음

B. Existing Requirement + Test Missing
   → 테스트만 보완

C. Existing Requirement + Design/Code GAP
   → 설계/코드 수정 검토

D. New Standard Requirement
   → 설계/코드/테스트 영향 분석

E. Field Requirement
   → 실차/실보드 Validation으로 이동
```

이 분류를 통해 이미 완료된 구현을 불필요하게 수정하지 않는다.

---

# 22. 현재 TBD / Pending 및 OPEN GAP 항목

## 22.1 C-class GAP 추적

| GAP | Requirement ID | 현재 상태 | 처리 제한 |
|---|---|---|---|
| C-02 | SBSD-DIAG-004 (관련: SBSD-DIAG-005/006, SBSD-HMI-004) | FIXED / SOFTWARE_VERIFIED (A/E) — production-path 9/9 PASS | CODE_GAP 종결. 실차·실보드·실물 HMI 및 정책 제외 fault는 미검증 유지 |
| C-03 | SBSD-CAL-007 | CLOSED — Calibration artifact 식별/revision 및 변경 시 재검토 계약 확정 (A/E) | 문서/운영 계약 GAP 종결. 실제 Calibration 값/실차 Validation은 E 유지 |

본 v2.0은 v1.4의 C-02 SOFTWARE_VERIFIED와 C-03 CONTRACT CLOSED 상태 및 사용자 안전/시스템 한계 문서 연결을 유지하면서 Type III/SAV 요구사항을 통합한다. 실제 Calibration, 실차/실보드, 물리 HMI, 성능/환경 Validation은 미완료 상태를 유지한다.

## 22.2 Field / Board Pending

| TBD ID | 항목 | 현재 상태 | 확정 방법 |
|---|---|---|---|
| TBD-001 | 실제 Forward Side ROI | Pending | 실차 장착/측정 |
| TBD-002 | 실제 Reverse Side ROI | Pending | 실차 장착/측정 |
| TBD-003 | 거리 Calibration 최종값 | Pending | 실차 거리 측정 |
| TBD-004 | Ego Motion 최종 threshold | Pending | 속도별 실차 영상 |
| TBD-005 | 경보 latency 상한 | Pending | 실보드/실차 측정 |
| TBD-006 | 최소 유효 채널 처리율 | Pending | 실보드 측정 |
| TBD-007 | False Positive 합격기준 | Pending | 실차 데이터 |
| TBD-008 | False Negative 합격기준 | Pending | 실차 데이터 |
| TBD-009 | HMI 색상/점멸/Buzzer 세부 Pattern | Pending | 제품 HMI 정책/실보드 |
| TBD-010 | ISO 17387:2026 세부 조항 GAP | Pending | 전체 원문 검토 |
| TBD-011 | 예측형 RCTA 필요 여부 | Future | 제품 요구 발생 시 별도 검토 |

**Brake 입력은 TBD가 아니다. 현재 시스템 입력에 존재하지 않으므로 요구사항 대상에서 제외한다.**

---

# 23. Release / Validation 기준

현재 단계에서 소프트웨어 개발 완료와 제품 Validation 완료를 구분한다.

## 23.1 Software Baseline

다음이 확인되면 현재 Software Baseline을 유지할 수 있다.

- 시스템 기술 설계서와 구현의 일치
- 기존 논리검증/단위테스트 PASS
- 요구사항과 기존 테스트의 추적성 확인
- 새로 발견된 Requirement/Test GAP 처리

## 23.2 Vehicle Validation

최종 제품 Validation에는 최소 다음이 필요하다.

- 실차 Camera/Turn Signal/Reverse I/O 확인
- ROI 및 거리 Calibration
- Forward 10 m / 30 m 경계 확인
- Reverse 1.5 m / 3.0 m 경계 확인
- 실제 접근/이탈/동속/반대방향 차량 시나리오
- 실제 Reverse 근접/교차 위험 시나리오
- HMI/LED/Buzzer 확인
- 경보 지연 및 처리성능 측정
- False Positive / False Negative 평가
- 환경 변화 조건 확인

## 23.3 Standard Compliance

ISO 준수를 제품에 표시하려면 별도로 다음을 완료해야 한다.

- ISO 17387:2026 전체 원문 조항 대조
- Requirement Traceability Matrix
- 필요한 시험조건 반영
- 시험 결과 및 미적용 사유 기록

---

# 24. 문서 관리 원칙

본 문서는 Side BSD 제품의 시스템 요구사항 Baseline으로 발전시킨다.

현재 v2.0은 Requirement ID 108개와 C-02 Option B, C-03 Calibration artifact 계약을 유지하면서 Type III / SAV 추가서 v1.1의 규범 요구사항을 본 SRS에 통합한 문서이다.
C-class 문서/코드 GAP과 사용자 안전 문구 Inspection 항목은 정리되었지만 국제규격 원문 GAP과 실차/실보드/실제 Calibration 값/성능/환경 검증은 여전히 미완료다.

기존 동작과 본 요구사항에 불일치가 발견되면 자동으로 코드를 수정하지 않고 다음 순서로 판단한다.

```text
1. 기존 제품 의도가 무엇인지 확인
2. 현재 요구사항이 그 의도를 정확히 표현하는지 확인
3. 설계/코드가 요구사항과 다른지 확인
4. 기존 테스트가 어느 동작을 검증했는지 확인
5. 실제 GAP일 때만 문서/설계/코드/테스트 수정
```

---

## 변경 이력

| Version | Date | Description |
|---|---|---|
| v0.1 | 2026-09-11 | 최초 SRS 초안 |
| v0.2 | 2026-09-11 | 시스템 기술 설계서 Baseline 역추출, Source/구현/검증 상태 추가, Mode/Session/Fault/Test-Production/HMI 요구사항 보완, Brake 입력 제거, ISO 미검증 세부 Clause 제거 |
| **v1.0** | **2026-09-11** | **RTM v0.4 authoritative status 동기화, 106개 Requirement Class 및 EV-1/EV-2/EV-L 구분 반영, C-01 FIXED/VERIFIED와 C-02/C-03 OPEN GAP 반영, P1 8/8·Side BSD 41/41·Repository 135/135 NO_CODE_GAP 반영** |
| **v1.1** | **2026-09-11** | **C-02 Option B 채널 fault best-effort isolation 정책을 기존 SBSD-DIAG-004/005/006 및 SBSD-HMI-004에 구체화. 신규 ID 없이 acceptance criteria와 제외 범위를 추가하고 C / OPEN 및 planned evidence 상태 유지** |
| **v1.2** | **2026-09-11** | **C-02 최소 production 수정과 fault-injection 9/9·Side BSD 50/50·Repository 144/144 PASS를 RTM v0.7과 동기화. C-02 CODE_GAP을 FIXED / SOFTWARE_VERIFIED (A/E)로 전환** |
| **v1.3** | **2026-09-11** | **C-03 Calibration artifact 추적성/변경관리 계약 확정. 차량·카메라·장착사양 revision과 Calibration revision의 연결, 변경 영향 검토 및 필요 시 재Calibration/재검증 규칙을 정의하고 C-03 문서/운영 GAP을 CLOSED (A/E)로 전환. 실제 Calibration/실차 Validation은 E 유지** |
| **v1.4** | **2026-09-11** | **사용자 안전/시스템 한계 문서 Baseline을 추가하여 SBSD-SYS-007 및 SBSD-SAF-001/002/003 추적성을 연결하고 SAF-002/003을 문서 Inspection 근거로 A로 전환. 현재 regression Baseline을 C-02 이후 50/50·144/144로 정합화하고 RTM v0.9 참조로 갱신** |
| **v2.0** | **2026-09-13** | **Type III/SAV 추가서 v1.1을 단일 규범 SRS에 통합. ROI와 Distance의 독립 판정 및 severity-max, TTC 2.5 s escalation-only, 기본경보 downgrade 금지, >30 m 경계, Forward 전용 적용범위와 production-path 경계시험을 반영하고 SBSD-FWD-014/015를 정식 편입** |
