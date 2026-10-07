# Side BSD 현재 Validation 로드맵

> Document ID: SBSD-ROADMAP-001
> Revision: v1.1
> Status: CURRENT
> Approval: SELF-REVIEWED
> Updated: 2026-09-13
> Software baseline: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`
> Current repository base: `main @ 7efb0b917f1f4ba1f7507dee5512543fb587f36e` + document verification working tree

## 1. 현재 판정

```text
Software implementation baseline : CLOSED_SOFTWARE
Document governance              : L0 COMPLETE
Lab recorded-video dataset       : NOT_STARTED
Rockchip board validation         : NEEDS_BOARD
Vehicle validation                : NEEDS_VEHICLE
ISO 17387:2026 full conformity    : NOT_CLAIMED
```

`CLOSED_SOFTWARE`는 정의된 소프트웨어 논리와 자동시험 범위의 완료를 의미한다. 실제 영상 성능,
물리 거리 정확도, 보드 성능, 차량 I/O 또는 전체 시스템 안전성 완료를 의미하지 않는다.

## 2. L0 — 문서 및 Software 기준선

상태: `COMPLETE`

- Type III/SAV 요구사항은 단일 SRS v2.0에 통합했다.
- 과거 요구사항 추가서와 인수시험 가이드는 `history/`에서 변경 근거로만 보존한다.
- 현재 동작 기준선은 Production source `7e427666...`이다.
- 해당 기준선 이후 `src/apps/side_bsd/`, `src/modules/`, `src/shared/`의 Production 변경은 없다.
- 제품사양, SRS, 설계, Calibration, Validation, 운영 문서의 역할을 분리했다.
- Software 실행 근거는 `verification/sidebsd/current/test_execution_summary.md` 한 곳에서 누적한다.
- 대외 Software 결과보고서는 내부 근거를 검토한 고정 Snapshot으로만 발행한다.

Forward 정책의 짧은 표기는 다음 조건을 생략해서는 안 된다.

```text
Distance layer : <=10m DANGER / <=30m WARNING / >30m SAFE
Final alert    : max(ROI level, Distance level, TTC escalation, Turn Signal escalation)
>30m final SAFE: target이 WARNING ROI 밖일 때
```

## 3. L1 — 입력영상 및 Ground Truth 준비

상태: `NEXT`

필수 입력:

- LEFT / RIGHT / REAR 연속 RAW 영상
- 차량·카메라 장착 위치, 높이, 각도, 해상도 및 반전 상태
- Forward rear trailing-edge 기준 3/5/6/7/8/9/10/15/20/30m marker
- Forward 10m/30m 주변점 및 lateral position 자료
- Reverse LEFT/RIGHT/REAR 1.5m/3m 자료
- TTC용 marker 통과시각 또는 reference speed
- Turn Signal / Reverse 상태 타임라인

수집 방법은 `../04_validation/Side_BSD_Validation_데이터_수집_절차서.md`를 따른다.

## 4. L2~L9 — Lab Validation

실행 순서:

```text
L2 ROI 물리경계
→ L3 Forward Distance Calibration
→ L4 ROI + Distance fail-safe replay
→ L5 Velocity / TTC replay
→ L6 Turn Signal / HMI software replay
→ L7 Ego Motion calibration
→ L8 Reverse Assist replay
→ L9 Lab closure
```

상세 시험조건, Expected/Actual 및 단계별 완료기준은
`../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md`에서만 관리한다.

Lab closure에는 최소 다음 근거가 필요하다.

- dataset manifest 및 Ground Truth revision
- source/config/calibration/model revision
- ROI와 거리 경계 결과
- TTC 및 상태전환 replay 결과
- FP/FN 측정 결과와 아직 확정되지 않은 acceptance threshold
- 원본 로그·영상 위치와 hash

## 5. Board Validation

상태: `NEEDS_BOARD`

Board 입고 후 순서:

```text
배포 패키지 검증
→ Board Diagnostic P1
→ AHD/MIPI/V4L2 채널 mapping
→ RKNN/NPU model load
→ GPIO/Vehicle I/O
→ Display/LED/Buzzer
→ latency/FPS/frame drop/thermal
→ disconnect/reconnect/recovery
→ endurance
```

배포와 진단 절차는 `../05_operation/Side_BSD_보드_배포_패키지_가이드.md`를 따른다.

## 6. Vehicle Validation

상태: `NEEDS_VEHICLE`

- 최종 장착 위치와 ROI
- Forward 10m/30m 및 Reverse 1.5m/3m 물리경계
- same-speed, receding, closing/TTC 시나리오
- 실제 Turn Signal/Reverse 입력과 HMI 출력
- motorcycle-sized target
- 주·야간, 우천, 터널, 역광, 진동, 온도 및 장시간 운용
- 제품 acceptance 기준에 따른 FP/FN 및 경보지연

## 7. ISO 17387:2026

상태: `NOT_CLAIMED`

Licensed full text, clause-by-clause mapping, 적용 차종 판단 및 표준 시험 evidence가 완료되기
전에는 full compliance를 주장하지 않는다.

## 8. 바로 다음 작업

```text
1. 현재 문서 정리분에 대한 증분 논리검증 기록
2. 현재 작업트리 기준 Repository 자동시험 재실행
3. L1 영상/Ground Truth 준비
```
