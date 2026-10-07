# AI_REVIEW_GUIDE.md

## 목적

이 문서는 AI가 Side BSD 프로젝트를 리뷰하거나 수정할 때,
사용자가 세부적인 검토 항목을 알지 못하더라도 **문제의 범위와 검증 수준을 스스로 확장**하도록 하기 위한 상위 리뷰 지침서다.

이 문서는 `docs/sidebsd/review_guide/AI_PROJECT_CONTEXT.md`, `docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md`,
`verification/history/legacy_docs_verification/sidebsd/logic/` 산출물을 대체하지 않는다. 기존 산출물은 선행 증거와
회귀 목록으로 활용하고, 현재 소스에서 유효한지 필요한 범위만 재확인한 뒤
**그 위에 리뷰 관점을 추가**한다.

- `docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md` = Side BSD 프로젝트 구조와 파일 역할 (구 `project_overview.md`의
  상위호환. 두 문서 내용이 다르면 `Side_BSD_CODEBASE_OVERVIEW.md`를 우선한다 — "실제
  코드에서 확인되지 않은 내용은 쓰지 않는다"는 사실성 원칙 하에 작성되었기 때문)
- `docs/sidebsd/review_guide/AI_PROJECT_CONTEXT.md` = 시스템의 핵심 불변 규칙과 작업 시 주의사항
- `verification/history/legacy_docs_verification/sidebsd/logic/*.md` = 논리검증 파이프라인(Phase 1~11)에서 이미
  확인된 설계-코드 일치성 / 모듈별 로직 정확성 / 동시성 / 경보 로직 FN·FP 결과.
  특히 `11_final_logic_review.md`의 "해결된 문제 / 남은 문제" 목록은 매 리뷰
  시작 전에 반드시 확인한다.
- `AI_REVIEW_GUIDE.md` (본 문서) = 위 산출물들이 다루지 않는 **개념·설계 타당성
  수준**의 리뷰 관점을 더하는 지침서 (아래 0장 참조)
- `docs/sidebsd/Side_BSD_시스템_기술_설계서.md` = 상세 설계 기준
- 실제 소스 코드 = 현재 구현의 최종 확인 대상

---

# 0. 논리검증 산출물과의 역할 분담

논리검증 통합 프롬프트(Phase 1~11)와 본 가이드는 서로 다른 질문에 답한다.

| 구분 | 논리검증 파이프라인 | 본 가이드 (AI_REVIEW_GUIDE) |
|---|---|---|
| 핵심 질문 | 코드가 설계/의도대로 분기·계산하는가 | 그 설계·알고리즘 자체가 타당한 접근인가 |
| 다루는 것 | 설계-코드 일치성, 경계값, 동시성, 경보 로직 FN/FP | Concept Validation, 알고리즘 대안 검토, 현장 적용성 판단 |
| 산출물 | `verification/history/legacy_docs_verification/sidebsd/logic/*.md` | 리뷰 결과 (채팅 또는 별도 저장) |
| 다루지 않는 것 | "이 알고리즘보다 더 나은 방법이 있는가" 같은 설계 비평 | 리뷰 범위와 무관한 과거 문제의 전면 재탐색 |

**기존 논리검증 산출물은 선행 증거와 회귀 확인 목록으로 활용한다.** 다만 소스가
변경되었거나 현재 리뷰 결론에 직접 영향을 주는 항목은 실제 코드와 테스트 결과로
재확인한다. 오래된 산출물을 현재 사실로 무조건 인용하지 않는다. 그 위에 본
가이드의 관점(개념 타당성 / 실패 시나리오 / 증거 수준 구분)을 추가로 적용한다.
구체적인 절차는 18장을 따른다.

---

# 1. 최우선 리뷰 원칙

AI는 사용자가 지정한 검토 항목만 기계적으로 확인하지 않는다.

사용자가 예를 들어 다음처럼 요청해도:

> "거리 관련 코드 리뷰해줘."

다음 범위를 스스로 찾아서 검토한다.

1. 해당 기능의 목적
2. 알고리즘의 개념적 타당성
3. 수학적/논리적 타당성
4. 실제 코드 구현과 설계의 일치 여부
5. 입력과 출력의 데이터 흐름
6. 설정값과 calibration 데이터의 연결
7. 예외/경계조건
8. 다른 모듈과의 연동
9. 성능/동시성/실시간성
10. 안전성과 fail-safe/fault 처리
11. 테스트 가능성
12. 실제 차량/현장 적용 위험
13. 설계 자체의 한계
14. 현재 데이터만으로 확정할 수 없는 사항

사용자가 구체적인 검토 항목을 제시한 경우에도,
그 항목이 전체 위험을 포괄하지 않을 가능성을 고려하여 필요한 범위까지 리뷰를 확장한다.

---

# 2. 리뷰 깊이 결정

모든 코드에 동일한 깊이의 분석을 적용하지 않는다.

## 2.1 단순 구현 코드

예:
- 파일 경로 처리
- 단순 변환
- 표시용 코드
- 단순 설정 읽기

검토 중심:
- 버그
- 예외
- 자료형
- 호출 관계
- 유지보수성

## 2.2 핵심 알고리즘 코드

예:
- DistanceCalibrator
- VelocityTracker
- EgoSpeedEstimator
- EgoMotionFusion
- SideFrameScheduler
- 경보 판정 로직

반드시 다음 단계까지 확장한다.

`개념 → 수학/논리 → 구현 → 데이터 → 실패조건 → 실제 적용성`

## 2.3 안전에 직접 영향을 주는 코드

예:
- DANGER/WARNING 판정
- 부저/LED
- 후진 기어
- 방향지시등
- FAULT
- 결과 캐시 만료
- TEST/PRODUCTION 경계

일반적인 코드 리뷰보다 높은 수준으로 검토한다.

특히 다음을 확인한다.

- 놓치면 위험한 경우
- 잘못 울리는 경우
- 오래된 결과가 유지되는 경우
- 예외 발생 시 안전하지 않은 상태로 전환되는 경우
- 테스트 기능이 양산 기능에 침투하는 경우
- 입력 신호 이상 시 잘못된 경보가 발생하는 경우

---

# 3. 알고리즘 리뷰 방법

## 3.1 Concept Validation

먼저 코드가 아니라 **아이디어 자체**를 검토한다.

질문:

- 이 알고리즘은 무엇을 추정/판단하려는가?
- 그 입력값이 실제로 목적을 나타낼 수 있는가?
- 알고리즘이 사용하는 가정은 무엇인가?
- 그 가정이 실제 차량 환경에서도 성립할 가능성이 있는가?
- 특정 조건에서는 근본적으로 실패할 수 있는가?
- 더 단순한 방법으로 동일한 목적을 달성할 수 있는가?
- 현재 구조가 문제를 해결하는 방향과 일치하는가?

개념적으로 문제가 있으면 코드 최적화보다 먼저 지적한다.

---

## 3.2 Mathematical / Logical Validation

수식과 논리를 검토한다.

반드시 확인:

- 단위
- 부호
- 좌표계
- 정규화
- 시간 간격
- 경계값
- 분모 0 가능성
- overflow/underflow
- clipping
- 보간/외삽
- threshold의 방향
- 상태 전이 조건
- hysteresis/debounce
- 샘플 수와 시간 창

예:
- 접근 속도와 이격 속도의 부호가 논리적으로 일관되는가?
- `distance / dt`, `delta_distance / delta_time`의 방향이 올바른가?
- pixel 단위 값과 m/s 단위 값을 혼동하지 않는가?
- normalized coordinate와 pixel coordinate를 혼용하지 않는가?

---

## 3.3 Implementation Validation

설계가 맞더라도 코드가 설계대로 구현되어 있는지 확인한다.

확인:

- 실제 호출 경로
- 함수 인자
- 반환값
- 상태 저장 방식
- 캐시
- config 연결
- 초기화
- 예외 처리
- 스레드 경계
- stale result 처리
- 조건문 우선순위
- 동일 로직의 중복 구현
- 설계서와 실제 코드의 차이

문서와 코드가 다르면 임의로 한쪽을 정답으로 만들지 않는다.

반드시:

`문서 내용 / 실제 코드 / 차이 / 어느 쪽을 기준으로 해야 하는지`

를 구분하여 보고한다.

---

# 4. 데이터 기반 검증

알고리즘을 실제 데이터로 검증할 수 있는지 판단한다.

AI는 다음을 확인한다.

- 어떤 데이터가 필요한가?
- 현재 데이터가 있는가?
- 현재 데이터가 충분한가?
- 어떤 구간에서 오차가 커질 수 있는가?
- 평균 오차만으로 충분한가?
- 최대 오차 또는 percentile이 필요한가?
- 데이터가 특정 거리/속도에 편향되어 있는가?
- calibration point가 특정 구간에 몰려 있는가?
- train/calibration 데이터와 실제 운영 데이터가 같은 분포인가?

필요한 경우 검증 지표를 제안한다.

예:
- RMSE
- MAE
- 최대 절대 오차
- 구간별 오차
- false positive / false negative
- 상태 분류 정확도
- latency
- FPS
- stale result 비율

단, 지표를 제안하는 것과 실제 값이 좋다고 판단하는 것은 구분한다.

---

# 5. 실제 환경 검증

AI는 코드만 보고 실제 차량 성능을 확정하지 않는다.

다음 조건을 검토한다.

- 카메라 설치 높이
- 카메라 설치각
- 렌즈 왜곡
- 차량 차체 형상
- 곡선도로
- 오르막/내리막
- 노면 상태
- 조도 변화
- 야간
- 그림자
- 비/눈 등 영상 품질 변화
- 카메라 흔들림
- 객체 부분 가림
- 급접근 객체
- 정지 객체
- 서로 다른 객체 크기
- 다른 차량/보행자/자전거 등 객체 유형

실제 데이터나 실차 시험이 없으면:

> "확인할 수 없음"

또는

> "실측 검증 필요"

로 명확히 구분한다.

---

# 6. Failure Mode Analysis

정상 상황보다 **어떻게 망가질 수 있는지**를 적극적으로 찾는다.

각 핵심 기능에 대해 다음을 질문한다.

- 입력이 없으면?
- 입력이 늦으면?
- 프레임이 누락되면?
- 프레임 시간이 잘못되면?
- 객체 ID가 바뀌면?
- tracker가 사라졌다가 나타나면?
- calibration이 잘못되면?
- 거리 계산이 None이면?
- timestamp가 역행하면?
- 스레드가 실패하면?
- NPU가 바쁘면?
- 이전 결과가 남아 있으면?
- config가 비정상 값이면?
- 센서 값이 튀면?
- 좌/우 중 한쪽만 정상이라면?

각 문제에 대해:

`발생 조건 → 실제 결과 → 위험성 → 현재 방어 → 방어되지 않는 부분`

순서로 분석한다.

---

# 7. Safety / Fail-safe 검증

경보 시스템에서는 단순히 "정상 동작"보다 안전한 실패가 중요하다.

반드시 확인:

## False Negative

실제로 위험한 객체를 놓치는 경우.

예:
- 너무 오래된 캐시 사용
- 속도 미확정 때문에 위험 객체 억제
- 거리 계산 실패
- ROI 밖으로 잘못 판정
- 필터가 실제 급접근 객체를 제거

## False Positive

위험하지 않은 객체를 위험하다고 판단하는 경우.

예:
- 반대편 차량
- 멀어지는 차량
- 정차 차량
- 그림자/노이즈
- 잘못된 calibration

## Fail-safe

오류 발생 시:

- SAFE로 갈 것인가?
- WARNING으로 유지할 것인가?
- DANGER를 유지해야 하는가?
- FAULT를 별도로 출력해야 하는가?
- 오래된 결과를 사용하면 안 되는가?

기존 설계의 의도를 확인하고 임의로 변경하지 않는다.

---

# 8. 상태 머신 / 시간축 검증

실시간 시스템은 한 프레임만 보면 맞아도 시간축에서 문제가 생길 수 있다.

따라서 다음을 확인한다.

- 상태 전이
- 카운터
- 연속 프레임 조건
- hysteresis
- debounce
- cooldown
- grace period
- timestamp
- stale timeout
- task 완료 순서
- 이전 상태의 잔존

특히:

`정상 → 경고 → 위험 → 해제`

전체 흐름을 시간 순서대로 시뮬레이션해서 생각한다.

---

# 9. Multi-thread / Scheduler 검증

ThreadPool, scheduler, asynchronous inference가 관련되면 추가로 검토한다.

- race condition
- 상태 동시 변경
- Lock 필요성
- Lock 범위
- deadlock 가능성
- task pile-up
- stale future/result
- 이전 세션 결과가 다음 세션으로 넘어가는 문제
- starvation
- priority inversion
- fairness
- 결과 순서 역전

성능 관련 코드에서는 단순히 "FPS가 높다"는 이유로 정상이라고 판단하지 않는다.

`입력 → 추론 → 결과 적용 → 경보 출력`

전체 end-to-end 흐름을 고려한다.

---

# 10. Configuration 검증

config 값 하나만 보고 판단하지 않는다.

반드시:

`config 정의 → 생성자 → 함수 호출 → 실제 사용 → 결과`

전체 경로를 따라간다.

다음을 찾는다.

- 선언만 되고 사용되지 않는 값
- 기본값이 실제 값과 다른 값
- 동일 의미의 값이 여러 곳에 존재
- 하드코딩된 값
- 단위가 다른 값
- 테스트 전용 값이 production에 영향을 줄 가능성
- 변경했는데 실제 동작이 바뀌지 않는 config
- tool 출력과 runtime config의 불일치

---

# 11. Calibration 검증

Calibration이 있는 경우 단순히 "값을 계산한다"로 끝내지 않는다.

검토:

- 측정 방법
- 측정 위치
- 측정 거리 범위
- 측정 포인트 수
- 포인트 분포
- 노이즈
- fitting 모델
- overfitting
- extrapolation
- interpolation
- RMSE
- 실제 운용 범위 포함 여부
- calibration 후 다른 환경에서의 안정성

AI는 calibration point가 적거나 특정 구간에 몰렸다는 이유만으로 무조건 잘못됐다고 단정하지 않는다.
대신 그로 인해 발생 가능한 위험을 설명하고 추가 검증 방법을 제안한다.

---

# 12. Domain-specific 자동 확장

사용자가 기능명만 말해도 관련된 검토 항목을 자동으로 확장한다.

## "거리 추정"

자동으로 확인:

- projection model
- coordinate system
- y_norm
- calibration curve
- fitting
- RMSE
- extrapolation
- camera geometry
- ROI
- foot point
- distance reliability
- None 처리
- distance → alert 연결
- Side / Rear 차이
- 실제 거리 데이터 필요성

## "Optical Flow"

자동으로 확인:

- feature detection
- LK tracking
- motion vector
- direction consistency
- outlier removal
- percentile
- EMA
- camera movement
- road texture
- camera vibration
- curves / slope
- stop/moving classification
- left/right fusion
- threshold derivation
- state transition
- false classification

## "VelocityTracker"

자동으로 확인:

- distance delta
- timestamp
- dt
- sign convention
- smoothing
- EMA
- moving average
- tracker ID stability
- ID 0
- outlier rejection
- clipping
- grace period
- time gap
- stale track
- fast-path
- filter interaction

## "Scheduler"

자동으로 확인:

- state machine
- alternating
- priority
- cooldown
- FIFO
- fairness
- starvation
- frame rate
- fractional accumulator
- thread safety
- concurrent update
- task backlog
- priority persistence
- full-inference bypass

## "Rear Camera"

자동으로 확인:

- reverse gear
- rear activation
- no velocity filter
- stationary obstacles
- rear ROI
- distance threshold
- async inference
- result cache
- timeout
- FAULT
- session generation
- rear BSD OSD

---

# 13. 증거 수준 구분

모든 결론을 같은 수준으로 표현하지 않는다.

## A. 코드로 확정 가능

예:
- 특정 함수가 호출되지 않음
- `None` 처리가 없음
- 조건문이 특정 값에서 잘못 동작
- 단위가 명백히 불일치
- `dt == 0` 가능성을 방어하지 않음

→ "확인된 문제"

## B. 설계상 강하게 의심

예:
- 모델 가정이 실제 환경에서 취약할 가능성
- threshold가 특정 조건에서 지나치게 민감할 가능성

→ "설계상 위험 / 추가 검증 권고"

## C. 실측 데이터가 필요

예:
- 실제 거리 오차가 허용 범위 이내인지
- Optical Flow threshold가 차량별로 충분한지

→ "현재 자료만으로 판단할 수 없음"

## D. 실차 시험 필요

예:
- 실제 차량에서 경보가 적시에 발생하는지
- 진동/곡선/경사로에서 안정적인지

→ "실차 검증 필요"

## 참고: 논리검증 산출물과의 관계

`verification/history/legacy_docs_verification/sidebsd/logic/*.md`에 이미 A(코드로 확정 가능) 수준으로 기록된
문제라도 현재 소스 변경 여부와 관련 테스트 결과를 확인한다. 여전히 유효하면
출처 문서와 현재 코드 위치를 함께 근거로 표기하고, 해결되었으면 과거 문제를
현재 문제처럼 보고하지 않는다. 새로 발견한 문제는 위 A~D 기준으로 분류한다.

---

# 14. 리뷰 결과 작성 형식

사용자가 단순히 "리뷰해줘"라고 했으면 다음 구조를 사용한다.

## 1. 결론

가장 중요한 문제를 먼저 요약한다.

## 2. 핵심 발견사항

각 발견사항은 `CRITICAL/HIGH/MEDIUM/LOW`로 분류하고 다음 필드를 빠짐없이
기록한다.

```text
[CRITICAL/HIGH/MEDIUM/LOW]

문제:
근거: 파일명 / 클래스 / 함수 / 관련 코드 위치
발생 조건:
실제 동작:
예상 동작:
영향:
재현 방법:
권장 수정:
검증 방법:
```

## 3. 알고리즘 검증

- 개념
- 수학/논리
- 구현
- 데이터
- 실제 적용성

## 4. 위험 시나리오

실제로 어떻게 잘못 동작할 수 있는지 예시를 든다.

## 5. 확인된 사실 vs 추가 검증 필요

두 가지를 명확하게 구분한다.

## 6. 수정 권고

- 즉시 수정
- 데이터 검증 후 수정
- 설계 변경 검토
- 당장은 수정하지 않아도 되는 항목

코드를 바로 수정하라는 요청이 없으면 **먼저 리뷰만 수행한다.**

---

# 15. 고수준 알고리즘에 대한 특별 규칙

거리 추정, Optical Flow, 속도 추정, 주행상태 판단 등은
"코드가 실행된다"는 것과 "알고리즘이 옳다"는 것을 동일시하지 않는다.

반드시 다음 네 가지 질문을 분리한다.

1. **Concept**
   - 이 접근 자체가 맞는가?

2. **Math / Logic**
   - 수식과 논리가 맞는가?

3. **Implementation**
   - 코드가 그 논리를 정확히 구현하는가?

4. **Evidence**
   - 실제 데이터로 성능을 입증할 수 있는가?

네 가지 중 하나라도 미검증이면 최종 결론에 그 사실을 명시한다.

---

# 16. 기존 설계를 무조건 옳다고 가정하지 않는다

기술 설계서가 있다고 해서 설계 자체가 반드시 옳다고 가정하지 않는다.

동시에 AI가 단순히 더 "복잡해 보이는" 알고리즘을 선호해서 기존 설계를 불필요하게 변경하지도 않는다.

따라서 다음을 구분한다.

- 설계서와 코드가 일치함
- 설계서와 코드가 불일치함
- 설계서 자체에 잠재적 문제가 있음
- 설계는 타당하지만 실제 데이터 검증이 부족함
- 코드에는 문제가 없지만 현장 적용성 검증이 부족함

---

# 17. 불확실성 처리

AI가 확실히 알 수 없는 내용은 추측하지 않는다.

특히 다음을 임의로 확정하지 않는다.

- 실제 카메라 화각의 정확한 영향
- 실제 차량의 경보 성능
- 실차 false positive / false negative
- calibration 정확도
- NPU 실제 병렬 처리 여부
- 특정 임계값이 모든 차량에서 최적이라는 판단

필요하면 다음처럼 명시한다.

> 현재 소스와 문서만으로는 확인할 수 없습니다.

그리고 가능한 경우:

> 확인하려면 다음 데이터/시험이 필요합니다.

를 제안한다.

---

# 18. 리뷰 전에 반드시 해야 할 일

1. `docs/sidebsd/Side_BSD_시스템_기술_설계서.md`에서 설계 의도와 요구사항을 확인한다.
2. `docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md`를 읽는다 (`project_overview.md`보다 우선).
3. `docs/sidebsd/review_guide/AI_PROJECT_CONTEXT.md`를 읽는다.
4. `verification/history/legacy_docs_verification/sidebsd/logic/11_final_logic_review.md`를 읽고
   "해결된 문제 / 아직 남은 문제" 목록을 확인한다.
5. 리뷰 대상과 관련된 논리검증 산출물이 있으면 먼저 읽는다
   (예: 거리 추정 리뷰 → `05b_distance_calibrator_logic.md`,
   Scheduler 리뷰 → `06_scheduler_concurrency_logic.md` 등).
   이미 "확인된 사실"로 적힌 내용은 선행 증거로 사용하되, 현재 소스가
   변경되었거나 리뷰 결론에 직접 영향을 주면 다시 확인한다.
6. 요청한 기능과 관련된 실제 소스 코드를 찾는다.
7. 호출 관계와 데이터 흐름을 확인한다.
8. 관련 config의 정의부터 실제 소비 지점까지 확인한다.
9. 코드와 문서가 다르면 차이를 기록한다. 논리검증 산출물에 이미 같은
   불일치가 `[상태] MISMATCH`로 기록되어 있고 현재도 유효하다면 중복 문제로
   만들지 말고 해당 문서와 현재 코드 위치를 함께 인용한다.
10. 위 1~17장의 리뷰 프레임을 자동 적용하되, 5번에서 확인한 기존 사실
    위에 개념·설계 타당성 관점(3.1, 15장)을 추가하는 데 집중한다.

---

# 19. 작업 요청이 모호해도 리뷰를 축소하지 않는다

사용자가:

> "거리 부분 좀 봐줘."

라고 요청하면 "어떤 부분인지 알려달라"고 바로 되묻지 않는다.

우선 AI가 스스로 범위를 탐색한다.

예:

`distance_calibrator.py`
→ 이를 호출하는 모듈
→ calibration config
→ ROI와 foot point
→ velocity tracker
→ alert logic
→ 관련 테스트/calibration tools

까지 확인한 후,
추가 질문이 정말 필요한 경우에만 질문한다.

---

# 20. 수정 작업 시의 추가 원칙

리뷰 결과 문제가 발견되었다고 해서 바로 대규모 수정하지 않는다.

먼저:

`문제 → 원인 → 영향 → 최소 수정안 → 회귀 위험`

을 판단한다.

특히 알고리즘 문제는:

- 코드만 고쳐서 해결 가능한지
- calibration 데이터가 필요한지
- threshold 재튜닝이 필요한지
- 설계 변경이 필요한지

를 분리한다.

---

# 21. 최종 목표

AI의 역할은 사용자가 전문가 수준의 질문을 미리 만들어 주는 것을 요구하는 것이 아니다.

사용자가 다음과 같이 말할 수 있어야 한다.

> "거리 관련 부분 전체를 검토해줘."

그리고 AI는 그 요청을 받아:

`구조 파악`
→ `관련 코드 탐색`
→ `설계 확인`
→ `개념 검증`
→ `수학/논리 검증`
→ `구현 검증`
→ `데이터 검증`
→ `실패 시나리오 분석`
→ `안전성 검토`
→ `실차 검증 필요사항`
→ `결론`

까지 스스로 확장하여 분석해야 한다.

단, AI는 실제 데이터가 없는 경우 데이터가 있는 것처럼 가정하지 않고,
실차 시험이 필요한 경우 시험 결과를 추정하지 않는다.
