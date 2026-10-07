# Side BSD Documentation Index

> Status: CURRENT  
> Updated: 2026-09-14

이 문서는 Side BSD의 문서 목록, 현재 상태와 권장 읽기 순서를 관리한다.

## 권장 읽기 순서

1. `CURRENT_IMPLEMENTATION_BASELINE.md` — 현재 구현 사실과 software baseline
2. `CURRENT_VALIDATION_ROADMAP.md` — 현재 단계와 다음 Validation 작업
3. `../01_requirements/Side_BSD_제품사양서.md` — 제품 수준 기능 정의
4. `../01_requirements/Side_BSD_시스템_요구사항_명세서.md` — 단일 규범 시스템 요구사항(Type III/SAV 포함)
5. `../02_design/Side_BSD_시스템_기술_설계서.md` — 시스템/알고리즘 설계
6. `../02_design/Side_BSD_CODEBASE_OVERVIEW.md` — 코드 구조와 데이터 흐름
7. `../03_calibration/Side_BSD_캘리브레이션_가이드.md` — Calibration 절차
8. `../03_software/Side_BSD_실행모드_및_Profile_가이드.md` — 실행 환경, Profile, 표시/기록 설정
9. `../04_validation/Side_BSD_소프트웨어_검증_및_시험_결과_보고서.md` — release 시 고정하는 대외용 Software Verification Snapshot (현재 DRAFT)
10. `../04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md` — 영상/보드/실차 Validation과 인수기준
11. `../05_operation/Side_BSD_사용자_안전_및_시스템_한계.md` — 사용자 안전문구와 시스템 한계
12. `../05_operation/Side_BSD_운영_배포_가이드.md` — 운영/배포

Board Diagnostic Tool은 제품 기능과 독립된 지원 도구이므로
`../01_requirements/Side_BSD_Board_Diagnostic_Tool_요구사항서.md`에서 별도 관리한다.

논리/정적 검증과 요구사항↔코드 GAP 기록은 저장소 루트 `verification/sidebsd/`에서 관리한다.

## Source of Truth

```text
현재 실제 동작: production code
자동 회귀 여부: tests/
요구 동작: CURRENT requirements / product policy
논리 정합성: verification/sidebsd/
실차/보드 성능: 04_validation/ evidence
```
