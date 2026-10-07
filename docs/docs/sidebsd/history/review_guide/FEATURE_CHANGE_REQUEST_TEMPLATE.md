# 기능 변경 요청 프롬프트 템플릿

새로운 채팅에서 **기능 추가/개선/동작 변경/최적화**를 요청할 때,
아래 프롬프트에서 대괄호 `[ ]` 부분만 채워서 그대로 붙여넣는다.

리뷰(현재 코드가 맞는지 검토)만 필요하면 이 템플릿이 아니라
`docs/sidebsd/review_guide/REVIEW_REQUEST_TEMPLATE.md`를 사용한다.

## 사용법

1. 변경 대상(기능/모듈/모드)을 정한다.
2. "분석까지만 할지" "분석 후 구현까지 할지"를 정한다. "구현까지"를
   선택하면 영향 분석과 테스트 계획을 먼저 세운 뒤 같은 작업에서 구현과
   검증까지 진행한다. 다만 요구사항의 핵심 선택이 미정이거나 변경 범위가
   요청을 실질적으로 벗어나는 경우에는 분석 결과를 제시하고 확인을 기다린다.
3. 아래 "관련 산출물" 목록 중 이번 변경과 관련 없는 항목은 지운다.
   확실하지 않으면 목록 전체를 그대로 둬도 된다 — AI가 관련 여부를
   스스로 판단한다.
4. 아래 프롬프트 전체를 새 채팅에 붙여넣는다.

---

## 프롬프트

```
현재 프로젝트는 Side BSD / Rear Parking Assist 시스템이다.

작업을 시작하기 전에 다음을 순서대로 읽어라:

1. docs/sidebsd/review_guide/AI_FEATURE_CHANGE_GUIDE.md — 기능 변경 시 적용할 설계 판단 프레임
   (요구사항 재해석, KEEP/MODIFY/CONDITIONAL/BYPASS/REMOVE/ADD/REPLACE
   판단, 모드 전환·상태 오염 검토 등)
2. docs/sidebsd/Side_BSD_시스템_기술_설계서.md — 설계 의도와 요구사항
3. docs/sidebsd/review_guide/AI_PROJECT_CONTEXT.md — 시스템 핵심 불변 규칙
4. docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md — Side BSD 프로젝트 구조
5. verification/history/legacy_docs_verification/sidebsd/logic/11_final_logic_review.md — 지금까지 해결된
   문제 / 아직 남은 문제
6. verification/history/legacy_docs_verification/sidebsd/logic/10_fix_log.md — 과거에 고친 버그와의 충돌
   여부 확인용 (파일이 아직 없으면 수정 이력이 없는 것이므로 건너뛴다)

7. 변경 대상이 DistanceCalibrator / VelocityTracker / EgoSpeedEstimator /
   EgoMotionFusion / Scheduler / 경보 판정 중 하나에 해당하면,
   docs/sidebsd/review_guide/AI_REVIEW_GUIDE.md의 고수준 알고리즘 검증 절차
   (Concept → Math/Logic → Implementation → Data → Failure Mode → Production)
   를 함께 적용하라 (위 AI_FEATURE_CHANGE_GUIDE.md 19장 참조).

8. 이번 변경 대상과 관련 있는 기존 산출물 (관련 없으면 무시):
   - verification/history/legacy_docs_verification/sidebsd/logic/03_design_code_consistency.md
   - verification/history/legacy_docs_verification/sidebsd/logic/04_architecture_review.md
   - verification/history/legacy_docs_verification/sidebsd/logic/05a_velocity_tracker_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/05b_distance_calibrator_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/05c_ego_motion_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/06_scheduler_concurrency_logic.md
   - verification/history/legacy_docs_verification/sidebsd/logic/07_warning_logic_review.md

기존 산출물의 "확인된 사실"은 선행 증거로 활용하되, 현재 소스가 변경되었거나
이번 변경 판단에 직접 영향을 주는 항목은 실제 코드와 테스트 결과로 재확인하라.
그 위에 아래 두 판단 프레임을 함께 적용하라:

- docs/sidebsd/review_guide/AI_FEATURE_CHANGE_GUIDE.md — "이 기능이 필요한가 / 기존 기능을
  KEEP·MODIFY·CONDITIONAL·BYPASS·REMOVE·ADD·REPLACE 중 무엇으로 다룰
  것인가"를 판단하는 설계 철학 프레임
- verification/history/legacy_docs_verification/sidebsd/논리검증_통합_프롬프트.md Phase 12 — 위 1~8에서 읽은 검증 문서를
  근거로 한 구체적 영향 범위 분석. Phase 12의 8개 분석 항목을 그대로
  적용한다: 영향받는 모듈 / 기존 데이터 흐름 중 개입 지점 / 07번 결정
  로직과의 상호작용 / 04번 구조적 약점과의 관계 / 11번 미해결 문제와의
  상호작용 / config 설계 일관성 / 깨질 수 있는 기존 테스트 / 필요한
  신규 테스트 케이스(Normal/Boundary/Invalid/Missing/Timeout/Recovery/
  Stress/Concurrent/Adversarial 9개 카테고리 기준)

이번 변경 요청:
[여기에 요청 내용을 적는다 — 예: "후진 기능을 개선해줘", "거리 관련
기능에 OO를 추가해줘"]

작업 범위:
[분석만 / 분석 후 구현까지 — 둘 중 하나를 명시한다. 비워두면 분석만
수행한다]

먼저 위 AI_FEATURE_CHANGE_GUIDE.md 21장 형식(요구사항 → 현재 동작 → 발견된
문제 → 기능별 판단표 → 제안 구조 → 영향 범위 → 위험 → 검증 방법)과
Phase 12의 8개 분석 항목을 하나로 합쳐 설계/영향 분석 결과를 정리하고,
중간 이상 규모이거나 안전 로직에 영향을 주는 변경이면
docs/sidebsd/features/{기능명}_impact_analysis.md 로 저장하라 ({기능명}은
이번 변경 요청 내용을 바탕으로 정한다). 단순 로그·문구 등 작은 변경은
채팅의 간단한 영향 분석으로 대체할 수 있다.

"작업 범위"가 "분석만"이면 분석 결과를 제시하고 코드를 수정하지 않는다.
"분석 후 구현까지"이면 요청 범위 안에서 테스트 계획을 세우고 구현·검증까지
진행한다. 단, 핵심 요구사항이 미정이거나 서로 다른 선택지가 안전 동작을
크게 바꾸는 경우에는 구현 전에 필요한 결정을 요청한다.

구현 전에는 verification/history/legacy_docs_verification/sidebsd/논리검증_통합_프롬프트.md의 Phase 8에 따라
영향 범위의 경계값·회귀 테스트 계획을 먼저 세운다. 구현 후에는 Phase 9부터
Phase 11까지를 이번 기능이 영향을 준 범위로 한정해 적용한다. Phase 9~11 결과는
verification/history/legacy_docs_verification/sidebsd/logic/ 의 해당 파일(08_test_plan.md, 09_test_results.md,
11_final_logic_review.md)에 기존 내용을 유지한 채 이어서 갱신하라.

Phase 11에서 새로운 CRITICAL/HIGH 문제가 발견되면 여기서 멈추고 결과를
보고하라 — 임의로 Phase 10(승인 후 수정) 사이클을 다시 시작하지 않는다.
```
