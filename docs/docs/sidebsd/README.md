# Side BSD 문서

> Status: CURRENT  
> Applies to: Side BSD (`src/apps/side_bsd/`)  
> Updated: 2026-09-13

이 디렉터리는 RV용 Camera-based Side BSD의 공식 제품 문서를 관리합니다.

## 먼저 읽을 문서

프로젝트의 문서 목록, 현재 상태와 권장 읽기 순서는 [`00_index/README.md`](00_index/README.md)를 기준으로 합니다. 현재 구현 사실 기준선은 [`00_index/CURRENT_IMPLEMENTATION_BASELINE.md`](00_index/CURRENT_IMPLEMENTATION_BASELINE.md)에서 확인합니다.

## 문서 구조

```text
docs/sidebsd/
├─ README.md
├─ 00_index/          # 문서 목록, 현재 상태, 읽는 순서
├─ 01_requirements/   # 제품/시스템 요구사항
├─ 02_design/         # 시스템/소프트웨어/알고리즘 설계
├─ 03_calibration/    # Side BSD 전용 보정 절차
├─ 04_validation/     # 영상/보드/실차 Validation 계획과 결과
├─ 05_operation/      # 배포/운영/현장 가이드
└─ history/           # 과거 계획과 리뷰 자료
```

## 테스트와 검증

- 실행 가능한 자동 테스트: `tests/side_bsd/`, `tests/common/`
- 논리/정적 검증, 요구사항↔코드 GAP: `verification/sidebsd/`
- 녹화영상/실보드/실차 Validation: `docs/sidebsd/04_validation/`

`tests/`의 PASS는 실차 검증 완료를 의미하지 않습니다.

## 현재 우선순위

현재 Software policy / Production implementation / production-path regression 이후 단계는 Lab recorded-video → Calibration → Board → Vehicle Validation 순서입니다. 상세 순서는 `00_index/CURRENT_VALIDATION_ROADMAP.md`를 따릅니다.

## 코드 수정 원칙

문서 정리 및 논리 검증 과정에서 production 코드를 임의로 수정하지 않습니다. 코드 변경이 필요하면 요구사항, 현재 동작, 영향, 최소 수정안과 테스트 계획을 먼저 정리한 뒤 별도 변경 작업으로 진행합니다.
