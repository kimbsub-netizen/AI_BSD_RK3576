# Side BSD History Index

> Status: CURRENT  
> Applies to: Side BSD  
> Updated: 2026-09-13

이 폴더는 **현재 기준 문서가 아닌 과거 개발 계획, AI 리뷰 가이드, 검증 캠페인 참고자료**를 보관한다.

## 현재 보관 구조

```text
docs/sidebsd/history/
├─ README.md
├─ Side_BSD_TypeIII_SAV_요구사항_추가서_v1.1.md
├─ Side_BSD_인수시험_가이드_v1.0.md
├─ development_plan/
└─ review_guide/
```

`Side_BSD_TypeIII_SAV_요구사항_추가서_v1.1.md`는 SRS v2.0 통합 전 규범 변경분을
보존한 이력 문서다. 현재 요구 동작에는 `../01_requirements/Side_BSD_시스템_요구사항_명세서.md`를 적용한다.

`Side_BSD_인수시험_가이드_v1.0.md`의 완료조건과 결과 기록 기준은
`../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md`에 통합되었다.

### `development_plan/`

과거 개발계획과 검증 준비 계획을 보관한다.

주요 예:

- `Side_BSD_개발계획서.md`
- `ego_motion_flow_development_plan.md`
- `field_test_plan.md`
- 과거 발표자료(PPTX)

현재 구현과 공식 설계 판단에는 이 자료보다 다음 CURRENT 문서를 우선한다.

```text
docs/sidebsd/00_index/
docs/sidebsd/01_requirements/
docs/sidebsd/02_design/
docs/sidebsd/03_calibration/
docs/sidebsd/04_validation/
docs/sidebsd/05_operation/
```

### `review_guide/`

과거 Side BSD 전용 AI 리뷰/변경 작업 가이드와 템플릿을 보관한다.
저장소 전체에 현재 적용되는 AI 작업 규칙은 루트 `AGENTS.md`를 사용한다.

## 논리 검증과의 경계

CURRENT 논리/정적 검증 workspace는 저장소 루트의 다음 경로다.

```text
verification/sidebsd/
```

이전 검증 캠페인 자료는 `verification/history/legacy_docs_verification/sidebsd/`에 보존하며 새 검증 결과를 추가하지 않는다.

## 해석 원칙

history 자료는 당시 판단과 개발 과정의 근거로 보존하지만 현재 구현의 Source of Truth로 사용하지 않는다.
현재 동작은 production source와 `00_index/CURRENT_IMPLEMENTATION_BASELINE.md`를 기준으로 확인한다.
