# Side BSD 리뷰 난이도 및 권장 모델

변경 파일 수보다 안전 영향과 검증 난이도를 우선해 등급을 정한다. 한 줄 수정이라도
경보 누락, 거리·속도 단위, TEST/PRODUCTION 경계에 영향을 주면 L3 이상으로 올린다.

## L1 — 정형적·저위험 작업

- 문서 경로 및 오탈자
- 로그·표시 문구
- 동작에 영향을 주지 않는 설정 설명

## L2 — 제한된 기능 작업

- 일반적인 코드 리뷰
- 단일 비안전 모듈 기능 추가
- 범위가 제한된 리팩터링
- 기존 동작을 명세화하는 테스트 코드

## L3 — 복합 또는 실시간 작업

- 여러 모듈에 걸친 기능 추가
- 주행 ↔ 후진 정책 변경
- Scheduler / VelocityTracker 상호작용
- concurrency, cache, timeout, 실시간성·성능 문제

## L4 — 핵심 안전·알고리즘 작업

- DistanceCalibrator
- Optical Flow / Ego Motion
- 상대속도 모델
- WARNING/DANGER 및 부저 경보 알고리즘
- False Negative 가능성이 있는 변경
- 핵심 아키텍처 변경과 설계 타당성 검증

## 권장 모델

- L1: GPT-5.6 Luna
- L2: GPT-5.6 Terra
- L3: GPT-5.6 Sol, reasoning `medium` 또는 `high`
- L4: GPT-6 Astra, reasoning `high` 또는 `max`
- GPT-6 Astra를 사용할 수 없는 환경의 L4 대안: GPT-5.6 Sol, reasoning `high` 또는 `max`

모델 제공 여부는 계정과 실행 환경에 따라 다를 수 있다. 현재 모델 역할은
[OpenAI 모델 문서](https://developers.openai.com/api/docs/models)를 기준으로 확인한다.
