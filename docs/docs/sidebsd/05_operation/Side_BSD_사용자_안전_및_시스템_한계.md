# Side BSD 사용자 안전 및 시스템 한계

> **문서 상태:** CURRENT
> **문서 ID:** SBSD-SAF-001
> **버전:** v1.1
> **작성일:** 2026-09-11
> **적용 대상:** Side BSD System
> **Software 기준선:** `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`
> **승인 상태:** SELF-REVIEWED
> **문서 성격:** 운영·사용자 안전 안내 및 제품 설명 최소 문구 Baseline
> **관련 요구사항:** `SBSD-SYS-007`, `SBSD-SAF-001`, `SBSD-SAF-002`, `SBSD-SAF-003`

---

## 1. 목적

본 문서는 Side BSD 제품 설명 및 사용자 안내에 포함해야 하는 최소 안전 문구와 시스템 한계를 정의한다.

Side BSD는 차량 주변의 위험 정보를 Display, LED 및 Buzzer로 운전자에게 제공하는 **경고 전용 운전자 보조 시스템**이며 차량의 제동, 조향, 가속 또는 구동을 직접 제어하지 않는다.

---

## 2. 필수 사용자 안전 안내

1. **경고가 발생하지 않았다는 사실은 차선 변경, 후진 또는 차량 이동이 안전하다는 보증을 의미하지 않는다.**
2. **운전자는 Side BSD 경고 여부와 관계없이 주변 상황을 직접 확인하고 안전 운전을 수행해야 한다.**
3. Side BSD는 운전자의 시야 확인과 판단을 보조하는 시스템이며 운전자의 주의 의무와 안전 판단을 대체하지 않는다.
4. 시스템이 정상적인 위험판단을 수행할 수 없는 상태는 정상 `SAFE` 상태와 동일한 의미로 해석해서는 안 된다.

---

## 3. 현재 Software 상태

현재 software baseline:

```text
main @ 7e427666300e5e800da7c5b4989ddf101a65f196
Compile PASS
Repository 169/169 PASS
```

Software에서는 다음 정책이 구현/검증되었다.

```text
DANGER ROI = 차량 측면 위험구간 + rear 0~10m fail-safe
WARNING ROI = DANGER ROI 포함 + rear 10~30m
base_level = max(roi_level, distance_level)
SAV TTC threshold = 2.5s
TTC = escalation-only
>30m TTC early-warning 없음
```

이 결과는 실제 카메라/차량 성능 검증 완료를 의미하지 않는다.

---

## 4. 남은 Validation 상태

현재는 실제 보드 이전 **Lab 녹화영상 검증 단계**다.

다음 항목은 Lab/Board/Vehicle evidence가 필요하다.

### Lab

- 실제 촬영영상 기반 ROI 물리경계
- rear trailing-edge 기준 10m/30m 거리 정확도
- ROI + Distance 이중 방어 replay
- Velocity/TTC 2.5s replay
- Ego Motion threshold
- Reverse Side / Rear calibration

### Board

- 실제 3채널 Camera input
- Rockchip NPU 처리성능
- Turn Signal / Reverse input
- Display / LED / Buzzer
- Fault / Unavailable 표시
- 장시간 안정성

### Vehicle

- 최종 장착조건
- final ROI/Distance calibration
- Forward 10m/30m 실제 경보경계
- Reverse 1.5m/3.0m 실제 경보경계
- False Positive / False Negative
- 실제 환경조건
- physical HMI latency

따라서 software 자동검증 PASS만으로 실제 도로·주차 환경에서 모든 위험을 감지한다고 간주해서는 안 된다.

---

## 5. Calibration 및 장착조건 변경

차량 형상, 카메라 사양, 장착 위치·높이·각도 또는 관련 장착사양이 변경되어 Calibration 유효성에 영향을 줄 가능성이 있는 경우 기존 Calibration의 유효성을 재검토해야 한다.

동일한 승인 차량/카메라/장착 Baseline과 허용 공차 범위에 속하는 경우 승인된 공통 Calibration Baseline을 사용할 수 있다.

---

## 6. ISO 관련 한계

현재 Type III/SAV Software Baseline 완료는 `ISO 17387:2026 full compliance` 선언이 아니다.

licensed full-text clause-by-clause 검토와 실제 conformity evidence가 완료되기 전에는 full compliance로 표현하지 않는다.

---

## 7. 요구사항 추적성

| Requirement | 본 문서 반영 내용 | Evidence |
|---|---|---|
| `SBSD-SYS-007` | 경고 미발생을 안전 보증으로 해석하지 않음 | Inspection / EV-2 |
| `SBSD-SAF-001` | 경고 전용 보조 시스템, 차량 직접 제어 없음 | Inspection / EV-2 |
| `SBSD-SAF-002` | 경고 미발생을 안전 보증으로 표현하지 않음 | Inspection / EV-2 |
| `SBSD-SAF-003` | 운전자의 주변 확인 및 안전 운전 필요 | Inspection / EV-2 |

`SBSD-SAF-004`의 전체 invalid-state 동작은 RTM의 software/실차 검증 상태를 따른다.

---

## 변경 이력

| Version | Date | Description |
|---|---|---|
| v1.0 | 2026-09-11 | 사용자 안전 최소 문구와 시스템 한계 Baseline 최초 확정 |
| **v1.1** | **2026-09-11** | **Type III/SAV software closure, 169/169 PASS, Lab→Board→Vehicle validation 상태 및 ISO full-compliance 한계 반영** |
