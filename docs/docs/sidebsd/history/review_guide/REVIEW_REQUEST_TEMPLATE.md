# 리뷰 요청 프롬프트 템플릿

새로운 채팅에서 코드 리뷰를 요청할 때, 아래 프롬프트에서 대괄호 `[ ]`
부분만 채워서 그대로 붙여넣는다.

## 사용법

1. 리뷰 대상(기능/모듈/파일)을 정한다.
2. 아래 "관련 산출물" 목록 중 이번 리뷰 대상과 관련 없는 항목은 지운다.
   확실하지 않으면 목록 전체를 그대로 둬도 된다 — AI가 관련 여부를
   스스로 판단한다.
3. 아래 프롬프트 전체를 새 채팅에 붙여넣는다.

---

## 프롬프트

```
현재 프로젝트는 Side BSD / Rear Parking Assist 시스템이다.

리뷰를 시작하기 전에 다음을 순서대로 읽어라:

1. docs/sidebsd/review_guide/AI_REVIEW_GUIDE.md — 리뷰 시 적용할 사고 프레임과 산출물 간 역할
   분담 (0장 참조)
2. docs/sidebsd/Side_BSD_시스템_기술_설계서.md — 설계 의도와 요구사항
3. docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md — Side BSD 프로젝트 구조
4. docs/sidebsd/review_guide/AI_PROJECT_CONTEXT.md — 시스템 핵심 불변 규칙
5. verification/history/legacy_docs_verification/sidebsd/logic/11_final_logic_review.md — 지금까지 해결된
   문제 / 아직 남은 문제

6. 이번 리뷰 대상과 관련 있는 기존 산출물 (관련 없으면 무시):
   - verification/history/legacy_docs_verification/sidebsd/logic/03_design_code_consistency.md
   - verification/history/legacy_docs_verification/sidebsd/logic/04_architecture_review.md
   - verification/history/legacy_docs_verification/sidebsd/logic/05a_velocity_tracker_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/05b_distance_calibrator_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/05c_ego_motion_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/06_scheduler_concurrency_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/07_warning_logic_review.md

기존 산출물의 "확인된 사실"은 선행 증거로 활용하되, 현재 소스가 변경되었거나
이번 결론에 직접 영향을 주는 항목은 실제 코드와 테스트 결과로 재확인하라.
그 위에 AI_REVIEW_GUIDE.md의 리뷰 프레임(특히 Concept Validation,
실패 시나리오 분석, 증거 수준 구분)을 추가로 적용하라.

이번에 리뷰할 대상:
[여기에 리뷰 요청 내용을 적는다 — 예: "거리 추정 관련 코드 전체를 리뷰해줘"]

코드를 바로 수정하라는 요청이 아니면 리뷰만 수행하고, 위 AI_REVIEW_GUIDE.md
14장 형식(결론 → 핵심 발견사항 → 알고리즘 검증 → 위험 시나리오 → 확인된
사실 vs 추가 검증 필요 → 수정 권고)으로 결과를 정리하라.
```
