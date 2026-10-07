# Side BSD Type III / SAV 요구사항 추가서

> **문서 상태:** ARCHIVED — SRS v2.0에 통합됨
> **작성일:** 2026-09-11
> **대상 문서:** `Side_BSD_시스템_요구사항_명세서.md` v1.4
> **적용 범위:** Forward Side BSD Type III / SAV 변경분
> **우선순위:** 본 추가서와 SRS v1.4의 관련 문구가 충돌할 경우 본 추가서를 우선 적용한다.

> **이력 안내:** 본 문서의 규범 요구사항은 2026-09-13에
> `../01_requirements/Side_BSD_시스템_요구사항_명세서.md` v2.0으로 통합되었다.
> 현재 요구 동작은 SRS v2.0을 단일 기준으로 사용하며, 본 문서는 변경 근거 확인용이다.

---

## 1. 목적

본 추가서는 Forward Side BSD의 Type III / SAV 동작과 안전정책을 규범적으로 확정한다.

핵심 원칙은 **Image ROI와 Distance estimation을 서로 독립된 방어층으로 사용하고, 더 높은 severity를 기본 경고로 채택하는 것**이다.

---

## 2. 최종 ROI 정책

Rear distance 원점은 RV rear trailing edge다.

```text
DANGER ROI
= 차량 측면 사각지대 + rear trailing edge 뒤 0~10m 절대영역

WARNING ROI
= DANGER ROI 전체 포함 + rear 10~30m 영역
```

따라서 `DANGER ROI ⊂ WARNING ROI` nested 구조를 유지한다.

차량 측면의 긴 RV blind-side 구간은 rear distance 음수값으로 판단하지 않고 image ROI/vehicle-side occupancy로 관리한다.

---

## 3. ROI + Distance 이중 방어

Forward 기본 위험도는 두 경로를 독립적으로 계산한다.

```text
ROI_level:
    DANGER ROI  -> DANGER
    WARNING ROI -> WARNING
    ROI 밖      -> SAFE

Distance_level:
    <=10m -> DANGER
    <=30m -> WARNING
    >30m  -> SAFE

base_level = max(ROI_level, Distance_level)
```

이 구조는 거리추정 오류에 대한 2차 방어다.

예를 들어 실제 7m target가 DANGER ROI 안에 있으나 DistanceCalibrator가 20m로 잘못 추정하면:

```text
ROI_level      = DANGER
Distance_level = WARNING
base_level     = DANGER
```

반대로 ROI 경계 오차로 WARNING이지만 distance=7m이면 Distance 경로가 DANGER를 유지한다.

---

## 4. 30m 초과 정책

`>30m = SAFE`는 **실제 물리 target가 WARNING ROI 밖에 있고 rear distance도 30m를 초과하는 경우**를 의미한다.

거리추정기 하나가 `>30m`를 출력했다는 이유만으로 ROI 안 target을 SAFE로 낮춰서는 안 된다.

```text
DANGER ROI 안  + distance estimate >30m -> DANGER 유지
WARNING ROI 안 + distance estimate >30m -> WARNING 유지
WARNING ROI 밖 + rear distance >30m      -> SAFE
```

30m 밖 TTC early-warning은 제공하지 않는다.

---

## 5. TTC 정책

초기 closing-speed target은 SAV이며 threshold는 다음과 같다.

```text
SIDE_BSD_SAV_TTC_THRESHOLD_S = 2.5 s
```

TTC는 기본경고 억제에 사용하지 않는다.

```text
base DANGER -> TTC/velocity로 downgrade 금지
base WARNING + reliable 10~30m + TTC<=2.5s -> DANGER
ROI/Distance 모두 SAFE인 >30m target -> TTC early-warning 없음
```

TTC UNKNOWN은 `SAFE` 근거가 아니라 `escalation unavailable` 상태다.

---

## 6. Requirement 정정/보완

### SBSD-FWD-007 / 008

Receding, non-closing, TTC invalid 또는 velocity 상태만으로 유효 DANGER/WARNING ROI 및 0~30m 기본경보를 SAFE로 downgrade해서는 안 된다.

### SBSD-FWD-010

Rear distance `10m 이하`는 DANGER다. 또한 DANGER ROI 자체도 거리추정 오류와 독립된 DANGER 방어층이다.

### SBSD-FWD-011

Rear distance `10m 초과 30m 이하`는 WARNING이다. WARNING ROI는 거리추정 오류에 대한 독립 방어층으로 유지한다.

### SBSD-FWD-014

`10~30m` 기본 WARNING target의 유효 TTC가 `2.5s 이하`이면 DANGER로 격상한다. TTC는 기본 위험도를 낮추지 않는다.

### SBSD-FWD-015

실제 target가 WARNING ROI 밖이고 rear 30m를 초과할 때 사용자 경고를 제공하지 않으며 TTC early-warning도 제공하지 않는다.

### SBSD-STD-003

Type III / SAV / TTC 2.5s / ROI+Distance fail-safe 정책은 현재 프로젝트 Software Baseline으로 확정한다. ISO licensed full-text 및 실제 검증 완료 전에는 full compliance를 주장하지 않는다.

---

## 7. Turn Signal

```text
same-side WARNING + Turn Signal -> DANGER
opposite-side Turn Signal       -> 영향 없음
```

Turn Signal과 TTC는 base level보다 낮은 결과를 만들 수 없다.

---

## 8. Reverse Assist

본 추가서의 ROI+Distance fusion 및 TTC 정책은 Forward Side BSD에만 적용한다.

Reverse Assist의 `1.5m DANGER / 3m WARNING`, all-class detection, Turn Signal 비연동 정책은 변경하지 않는다.

---

## 9. Production-path 검증 요구

최소 다음 시나리오를 실제 `SideCamera.infer()` 경로로 검증한다.

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

실제 `>30m SAFE` 물리경계는 랩실 녹화영상 및 이후 실차 ROI calibration evidence로 검증한다.

---

## 10. 남은 Validation

```text
랩실 녹화영상 ROI calibration
rear trailing-edge 기준 10m/30m 거리 정확도
ROI 10m/30m 물리경계 정합성
SAV closing-speed/TTC 정확도
실보드 HMI latency
FP/FN 및 환경조건
ISO licensed full-text clause review
```

---

## 변경 이력

| Version | Date | Description |
|---|---|---|
| v1.0 | 2026-09-11 | Type III/SAV, TTC 2.5s, 10/30m no-downgrade baseline |
| **v1.1** | **2026-09-11** | **DANGER ROI=차량 측면+rear 0~10m 절대 fail-safe, ROI/Distance severity-max fusion, >30m SAFE 물리영역 의미 확정** |
