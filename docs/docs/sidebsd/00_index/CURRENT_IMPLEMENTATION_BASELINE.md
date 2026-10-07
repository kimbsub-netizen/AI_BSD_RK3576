# Side BSD 현재 구현 기준선

> Document ID: SBSD-BASE-001
> Revision: v1.1
> Status: CURRENT
> Approval: SELF-REVIEWED
> Applies to: Side BSD
> Last verified: 2026-09-13
> Software baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`
> Automated regression: 최신 실행 결과는 `../../../verification/sidebsd/current/test_execution_summary.md` 참조

## 1. 역할

이 문서는 현재 Production 구현의 식별자와 핵심 동작을 짧게 고정하는 구현 Snapshot이다.
상세 요구사항은 SRS, 구조와 알고리즘은 기술설계서, 코드 경로는 CODEBASE OVERVIEW에서 관리한다.

```text
현재 실제 동작 : Production source
요구 동작      : ../01_requirements/Side_BSD_시스템_요구사항_명세서.md
현재 시험 근거 : ../../../verification/sidebsd/current/
실환경 검증    : ../04_validation/
```

## 2. Production 범위

- 실행: `src/apps/side_bsd/app_side_bsd.py`
- 설정: `src/apps/side_bsd/config/`
- Side 처리: `src/modules/side_camera.py`
- Scheduler: `src/modules/side_frame_scheduler.py`
- Rear 처리: `src/modules/rear_camera.py`
- Rear supervisor: `src/modules/rear_inference_supervisor.py`
- TTC: `src/modules/ttc_estimator.py`
- 공통 런타임: `src/shared/`

기준선 `7e427666...` 이후 현재 repository base `7efb0b9...`까지 위 Production 범위의 변경은 없다.
그 이후 추가된 배포·진단 도구와 문서는 별도 기준선과 시험 근거로 관리한다.

## 3. Forward Side BSD 구현 요약

```text
DANGER ROI  = 차량 측면 위험구간 + rear trailing-edge 뒤 0~10m fail-safe
WARNING ROI = DANGER ROI 전체 + rear 10~30m outer evaluation zone

Distance level:
  <=10m DANGER
  <=30m WARNING
  >30m SAFE

base_level = max(roi_level, distance_level)
```

따라서 Distance가 30m를 초과해도 target이 WARNING ROI 안이면 최종 결과는 WARNING 이상이다.
정상 Calibration에서 실제 30m 밖 target은 WARNING ROI 밖이 되도록 검증해야 한다.

TTC는 기본경고를 낮추지 않는 escalation 전용이다.

```text
10~30m base WARNING + valid TTC <=2.5s → DANGER
TTC UNKNOWN/receding/non-closing           → 기본경고 downgrade 금지
>30m                                      → TTC early-warning 미사용
same-side Turn Signal + WARNING            → DANGER
```

## 4. Reverse Assist 구현 요약

```text
Reverse Side DANGER  <=1.5m
Reverse Side WARNING <=3.0m
Rear DANGER          <=1.5m
Rear WARNING         <=3.0m
```

- Forward 정책과 mode/session을 분리한다.
- mode edge에서 cache, tracking, alert 및 scheduler 상태를 초기화한다.
- 실제 Calibration 전에는 `REVERSE_ASSIST_CALIBRATED=False` 상태를 구분한다.

## 5. Fault, 동시성 및 Hardware 경계

- LEFT/RIGHT scheduler와 Rear process supervisor를 분리한다.
- stale/fault/cache/timeout 및 worker recovery 경로가 존재한다.
- invalid channel을 정상 SAFE로 해석하지 않고 Fault/Unavailable로 구분한다.
- 실제 AHD/MIPI/RKNN/NPU/GPIO/Display/LED/Buzzer 동작은 Board/Vehicle evidence가 필요하다.

## 6. 현재 판정

```text
Software logic / regression : CLOSED_SOFTWARE
Lab recorded-video          : NOT_STARTED
Physical ROI / distance     : NEEDS_VERIFICATION
Rockchip board / NPU / I/O  : NEEDS_BOARD
Vehicle performance / FP-FN : NEEDS_VEHICLE
ISO full conformity         : NOT_CLAIMED
```

자동시험 PASS는 실제 영상 성능, 보드 성능, 차량 안전성 또는 ISO 적합성을 의미하지 않는다.

## 7. 변경 시 규칙

Production 또는 요구사항이 변경되면 다음을 수행한다.

1. 변경 기준 commit과 요구사항 ID를 기록한다.
2. Production diff와 공유 import/call path를 확인한다.
3. 관련 자동시험과 Repository 전체 회귀시험을 실행한다.
4. `verification/sidebsd/current/`에 증분 논리검증과 GAP을 기록한다.
5. 본 Snapshot과 관련 공식 문서 revision을 갱신한다.

다음 실제 작업은 `CURRENT_VALIDATION_ROADMAP.md`를 따른다.
