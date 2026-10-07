# Side BSD System Validation

> Status: CURRENT
> Applies to: Side BSD
> Updated: 2026-10-06

이 폴더는 녹화영상, 실제 Rockchip 보드 및 실제 차량에서 수행하는 Side BSD System
Validation의 계획·절차·결과와 승인된 대외 V&V 결과보고서를 관리한다.

## Verification과의 경계

```text
verification/sidebsd/
= 요구사항·설계·production code 정합성, 논리·정적 분석, 자동시험, GAP/Closure

docs/sidebsd/04_validation/
= Lab recorded-video, 실제 Board, 실제 Vehicle에서의 제품 동작·성능 확인 및 대외 보고
```

각 영역에는 계획과 결과가 모두 존재할 수 있다. 자동시험 PASS는 Lab, Board 또는 Vehicle
Validation PASS를 의미하지 않는다.

## 현재 문서

- `Side_BSD_시스템_Validation_계획_및_절차서.md` — Lab/Board/Vehicle 시험절차와 인수 판정기준
- `Side_BSD_Validation_데이터_수집_절차서.md` — 영상·사진·실측값·Ground Truth 수집 절차
- `Side_BSD_보드_성능_BENCHMARK_판정기준.md` — 실제 보드 YOLO/Side PIPELINE 성능 로그 해석 및 PASS/FAIL 기준
- `RK3576_SideBSD_Native_Pipeline_Performance_2026-10-06.md` — RK3576 Native dual pipeline 성능 측정과 recorded REAR decode 병목 분리 기록
- `Side_BSD_소프트웨어_검증_및_시험_결과_보고서.md` — 내부 근거를 검토해 revision별로 고정하는 대외용 Snapshot

현재 단계와 다음 작업은 `../00_index/CURRENT_VALIDATION_ROADMAP.md`에서 관리한다.
Software Verification 및 자동시험 결과는
`../../../verification/sidebsd/current/test_execution_summary.md`에서 내부 근거를 관리하고,
검토·승인된 Snapshot만 대외 결과보고서에 반영한다. 실행 근거와 대외 보고서를 동시에 계속 갱신하지 않는다.

## 결과 문서 생성 원칙

실제 evidence가 확보된 단계에 대해서만 결과보고서를 생성한다.

```text
Side_BSD_Lab_Validation_결과보고서.md
Side_BSD_Board_Validation_결과보고서.md
Side_BSD_Vehicle_Validation_결과보고서.md
```

결과보고서는 최소한 시험 대상 baseline, Config/Calibration revision, 입력 데이터 또는
Ground Truth, Expected/Actual, 판정, 로그·영상 위치와 미완료 항목을 포함해야 한다.

아직 수행하지 않은 단계는 PASS 결과 문서를 미리 만들지 않고
`NOT_TESTED`, `NEEDS_BOARD`, `NEEDS_VEHICLE` 또는 `VEHICLE_TEST_REQUIRED`로 관리한다.
