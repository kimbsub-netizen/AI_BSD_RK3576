# Side BSD 소프트웨어 검증 및 시험 결과 보고서

> 문서 ID: SBSD-SVTR-001  
> 문서 상태: DRAFT — EXTERNAL SNAPSHOT
> 개정: v0.2  
> 최초 작성일: 2026-09-13  
> 최종 갱신일: 2026-09-13  
> Snapshot cutoff: 2026-09-12 — 이후 실행 결과는 내부 근거에만 누적
> 적용 대상: Side BSD / Forward Type III·SAV / Reverse Assist / 관련 공통 모듈  
> 현재 Software 기준선: `main @ 7e427666300e5e800da7c5b4989ddf101a65f196`  
> 최신 확인된 자동 회귀 근거: Compile PASS / Repository 169/169 PASS  
> 문서 책임 범위: 소프트웨어 논리·정적 Verification, 자동시험 및 보드 진단 도구 Mock 시험 결과
> 범위 제외: 녹화영상·실보드·실차 Validation 결과
> 내부 근거: `../../../verification/sidebsd/current/test_execution_summary.md`
> Approval: WORKING / EXTERNAL-APPROVAL-PENDING

---

## 1. 문서 목적

본 문서는 Side BSD 소프트웨어 검증 근거를 특정 기준선에서 검토하여 발행하기 위한 대외용 Snapshot 초안이다.

본 문서가 다루는 범위는 다음과 같다.

- 요구사항·설계·Production 코드 간 논리 정합성 검토
- Production 경로의 자동 단위·회귀시험
- 경계값, fail-safe, fault/stale/cache 및 상태 전환 검증
- Forward Side BSD, TTC, Turn Signal 및 Reverse Assist 소프트웨어 검증
- Board Diagnostic Tool의 보드 입고 전 Mock/Fault Injection 검증
- 아직 수행하지 않은 Lab·실보드·실차·표준 적합성 시험의 TBD 관리

계속 갱신하는 실행 원본은 `verification/sidebsd/current/test_execution_summary.md`다. 본 문서는 발행 전 해당 원본을 검토하여 revision을 올리고 내용을 고정하며, 발행 후 같은 revision의 내용을 덮어쓰지 않는다.

---

## 2. 판정 용어

| 상태 | 의미 |
|---|---|
| `PASS` | 명시된 기준선·환경·시험 범위에서 기대 결과와 일치 |
| `FAIL` | 명시된 기대 결과와 불일치 |
| `CLOSED_SOFTWARE` | 정의된 소프트웨어 범위에서 구현과 자동시험 근거가 완료됨 |
| `TBD` | 시험 조건, 장비 또는 데이터가 준비되지 않아 향후 수행 필요 |
| `NEEDS_BOARD` | 실제 보드/BSP/NPU/카메라가 있어야 확정 가능 |
| `NEEDS_VEHICLE` | 실제 장착 차량과 동적 시나리오가 있어야 확정 가능 |
| `NOT_CLAIMED` | 현재 근거로 해당 적합성 또는 성능을 주장하지 않음 |

자동시험 `PASS`는 해당 assertion 범위의 통과를 의미한다. 실제 영상 성능, 물리 거리 정확도, 보드 성능, 실제 I/O 동작, 차량 안전성 또는 ISO 전체 적합성을 자동으로 의미하지 않는다.

---

## 3. 현재 종합 판정

기준일: 2026-09-13

| 검증 영역 | 현재 상태 | 근거 또는 제한 |
|---|---|---|
| 요구사항·설계·코드 논리 검토 | `PASS` / 이력 보존 | Side BSD 논리검증 01~11, GAP·Fix·최종 검토 기록 |
| Forward 기본 경고 정책 | `CLOSED_SOFTWARE` | ROI/Distance 독립 판정 및 severity-max 결합 |
| 10m/30m 소프트웨어 경계 | `PASS` | Production-path 경계시험 |
| TTC 2.5초 격상 정책 | `PASS` | 2.5초와 경계 외 조건 시험 |
| Turn Signal 격상·반대편 비간섭 | `PASS` | Production routing 회귀시험 |
| Reverse Assist 소프트웨어 격리 | `PASS` | Forward 정책 비간섭 및 상태 초기화 회귀 범위 |
| fault/stale/cache/scheduler | `PASS` | Repository regression 및 Closure 근거 |
| 컴파일 및 Repository 자동시험 | `PASS` | 2026-09-11 기준 169/169, FAIL 0, ERROR 0 |
| Board Diagnostic Tool P0 | `PASS` | Mock/Fault Injection 및 GitHub Actions workflow 성공 |
| Lab 녹화영상 검증 | `TBD` | Ground Truth 영상 미확보 |
| 실제 ROI 물리경계 | `TBD` | 실제 장착·거리 marker 기반 검증 필요 |
| 실제 거리 Calibration 정확도 | `TBD` | LEFT/RIGHT/REAR 실측 자료 필요 |
| 실제 TTC·상대속도 정확도 | `TBD` | 동적 replay 및 reference 계측 필요 |
| 실제 Rockchip 보드 동작·성능 | `NEEDS_BOARD` | BSP/AHD/MIPI/RKNN/NPU/성능 실측 필요 |
| 실제 Vehicle I/O와 HMI 출력 | `NEEDS_BOARD` / `NEEDS_VEHICLE` | Turn Signal, Reverse, Display, LED, Buzzer 물리 확인 필요 |
| 실제 차량 FP/FN·환경·내구 | `NEEDS_VEHICLE` | 주·야간, 우천, 터널, 진동 및 장시간 시험 필요 |
| ISO 17387:2026 전체 적합성 | `NOT_CLAIMED` / `TBD` | 정식 표준 전문 검토 및 적합성 시험 미완료 |

현재 판정은 **정의된 Software Scope는 완료**, Lab·실보드·실차 Validation과 ISO 전체 적합성은 미완료라는 의미다.

---

## 4. 검증 대상 및 기준선

### 4.1 주요 Production 대상

- `src/apps/side_bsd/app_side_bsd.py`
- `src/modules/side_camera.py`
- `src/modules/side_frame_scheduler.py`
- `src/modules/rear_camera.py`
- `src/modules/rear_inference_supervisor.py`
- `src/modules/ttc_estimator.py`
- Side BSD가 실제 사용하는 `src/shared/` 및 관련 `src/modules/`

### 4.2 요구사항 및 설계 기준

- `docs/sidebsd/01_requirements/Side_BSD_제품사양서.md`
- `docs/sidebsd/01_requirements/Side_BSD_시스템_요구사항_명세서.md`
- `docs/sidebsd/02_design/Side_BSD_시스템_기술_설계서.md`
- `docs/sidebsd/00_index/CURRENT_IMPLEMENTATION_BASELINE.md`

### 4.3 확인된 시험 기준선

| 실행 ID | 일자 | 코드/시험 기준선 | 범위 | 결과 |
|---|---|---|---|---|
| SV-LEGACY-01 | 2026-09-09 | 당시 Side BSD 결함 수정 기준선 | 초기 논리검증, 수정 후 지정 단위시험 12건 | 12/12 PASS |
| SV-TYPEIII-01 | 2026-09-11 | `main @ 7e427666300e5e800da7c5b4989ddf101a65f196` | Type III/SAV Production-path 및 Repository regression | Compile PASS, 169/169 PASS |
| SV-BDIAG-P0-01 | 2026-09-12 | `0d4b589fabf8bf3f8dbe08032752b1da882d0bc5`, PR #15 | Board Diagnostic P0 Mock/Fault Injection/CLI/Report 회귀 | GitHub Actions run #48 SUCCESS |

`SV-LEGACY-01`의 12건은 이후 Repository 169건과 중복될 수 있으므로 누적 합계에 더하지 않는다. 현재 대표 자동시험 수치는 최신 Software Closure 기준의 **169/169 PASS**다.

---

## 5. 시험 환경 및 조건

### 5.1 확인된 조건

| 구분 | 확인된 조건 |
|---|---|
| 실행 형태 | PC/CI 기반 Python 자동시험 및 정적·논리 검토 |
| Production-path 시험 | 실제 `SideCamera.infer()` 경로를 통한 입력·경계·fault-injection 검증 |
| 입력 형태 | 단위시험용 합성/Mock 입력 및 의도적으로 조작한 거리·속도·상태 입력 |
| Hardware 조건 | 실제 Rockchip 보드 없이 수행 |
| Camera 조건 | 실제 LEFT/RIGHT/REAR AHD 카메라 입력 없이 수행 |
| Vehicle I/O 조건 | 실제 Turn Signal/Reverse/GPIO 입력 없이 논리 입력으로 수행 |
| HMI 조건 | 논리 상태 및 출력 의도 검증, Display/LED/Buzzer 물리 출력 미검증 |
| Board Diagnostic P0 | 실제 adapter와 동일 Result Model을 사용하는 6개 Mock profile 및 fault injection |

### 5.2 현재 근거에 기록되지 않은 환경 정보

다음 정보는 대외 재현성 확보를 위해 차기 시험 실행부터 기록한다.

| 항목 | 상태 |
|---|---|
| OS 및 상세 버전 | `TBD` |
| Python 상세 버전 | `TBD` |
| 주요 dependency 버전 | `TBD` |
| CPU/GPU/NPU 사양 | `TBD` |
| 시험 실행 명령 전체 원문 | `TBD` |
| 169건의 suite별 세부 건수 | `TBD` — 전체 통과 수만 Closure에서 확인됨 |
| 원본 CI log 또는 보관 URL | `TBD` — Board Diagnostic은 PR #15/run #48 식별자만 확인됨 |

---

## 6. 수행 완료된 논리·정적 검증

### 6.1 검토 범위

과거 논리검증 캠페인에서 다음 항목을 검토했다.

- 프로젝트 구조와 호출 흐름
- 요구사항·설계·코드 정합성
- Velocity Tracker 및 timestamp/dt
- Distance Calibrator 반환 및 거리 신뢰도 처리
- Ego Motion 상태 및 debounce
- Scheduler·동시성·worker timeout/recovery
- Forward 경고 로직과 False Negative/False Positive 가능성
- fault/stale/cache 및 Reverse session 격리
- 단위, 좌표, 부호 및 인터페이스 일관성
- 수정 요청, 수정 결과 및 최종 로직 재검토

### 6.2 초기 검증에서 확인·수정된 주요 결함

| 항목 | 조치 및 결과 |
|---|---|
| 속도 미확정 초근접 객체 경보 누락 | 경보 판단 경로 수정 및 시험 PASS |
| 좌우 timestamp skew에 따른 전체 경보 억제 | 강제 UNKNOWN/부저 차단 제거 및 시험 PASS |
| Rear worker 복구 시 메인 루프 블로킹 | 동기 `join` 제거 및 supervisor 시험 PASS |
| Ego Motion 하강 debounce 우회 | 설정된 `stop_frames` 적용 및 시험 PASS |
| 거리 신뢰도 저하 시 DANGER 강등 | DANGER ROI fail-safe 유지 및 시험 PASS |
| 구형 테스트 API·인자 불일치 4종 | 현행 Production 인터페이스에 맞게 수정 후 PASS |

초기 최종 검토에서 제기된 5~10m 속도 미확정 경보 누락 가능성은 이후 Type III/SAV 정책에서 `0~30m base warning no-downgrade`로 변경되고 Production-path 시험 근거가 추가됐다. 최신 판정에는 2026-09-11 Software Closure를 우선 적용한다.

---

## 7. 수행 완료된 자동시험 결과

### 7.1 최신 Software Closure

2026-09-11 기준 결과:

```text
Compile            : PASS
Repository tests   : 169/169 PASS
FAIL               : 0
ERROR              : 0
```

판정: `CLOSED_SOFTWARE`

### 7.2 확인된 핵심 Production-path 시나리오

| 번호 | 시험 조건 | 기대 결과 | 결과 |
|---:|---|---|---|
| 1 | DANGER ROI + 거리추정 fault injection 20m | ROI fail-safe로 DANGER 유지 | PASS |
| 2 | WARNING-only ROI + 10.0m | DANGER | PASS |
| 3 | WARNING-only ROI + 10.0m + ε | WARNING | PASS |
| 4 | WARNING-only ROI + 30.0m | WARNING | PASS |
| 5 | WARNING-only ROI + 30.0m + ε | SAFE | PASS |
| 6 | 7m + `velocity=None` | DANGER 유지 | PASS |
| 7 | 20m + `velocity=None` | WARNING 유지 | PASS |
| 8 | 20m + receding | WARNING 유지 | PASS |
| 9 | 20m / 접근속도 8.0m/s, TTC=2.5s | DANGER 격상 | PASS |
| 10 | 20m / 접근속도 7.99m/s, TTC>2.5s | WARNING 유지 | PASS |
| 11 | same-side Turn Signal + WARNING | DANGER 격상 | PASS |

1번의 20m는 실제 물리 거리가 20m인 DANGER 대상이라는 의미가 아니다. 대상이 DANGER ROI 안에 있다고 가정하고 DistanceCalibrator 결과만 잘못된 20m로 주입하여 ROI 방어층을 시험한 조건이다.

### 7.3 추가 회귀 범위

Software Closure와 RTM Addendum에 다음 회귀 범위가 PASS로 기록되어 있다.

- 0~30m 기본 경고의 velocity/TTC no-downgrade
- same-side Turn Signal 격상 및 opposite-side 비간섭
- Reverse Assist와 Forward 정책 격리
- stale/fault 처리
- scheduler 및 worker recovery
- ROI/Distance 독립 severity-max 결합

개별 테스트 함수명, suite별 건수 및 실행 로그 원문은 차기 실행 시 본 문서 또는 별도 첨부 evidence에 보강한다.

---

## 8. Board Diagnostic Tool P0 시험 결과

### 8.1 목적과 조건

Board Diagnostic P0는 실제 보드 검증이 아니라 보드 입고 전에 진단 프레임워크, CLI, 보고서, Mock Hardware Layer 및 fault-injection 경로를 준비·검증하는 단계다.

### 8.2 완료 범위

- Diagnostic Framework 및 Result Model
- System/Python Runtime Check
- Camera/NPU/GPIO Check interface
- Mock Hardware Layer
- Fault Injection
- Console/Text Summary 및 JSON Report
- CLI 및 Unit Test
- 기존 GitHub Actions Python Tests workflow 연동

### 8.3 Mock 시험 조건

다음 6개 profile을 사용했다.

1. `all_pass`
2. `camera_failure`
3. `npu_failure`
4. `low_memory`
5. `low_storage`
6. `multiple_failure`

### 8.4 결과

| 항목 | 결과 |
|---|---|
| 기준 commit | `0d4b589fabf8bf3f8dbe08032752b1da882d0bc5` |
| GitHub Actions | `Python Tests`, run #48 |
| Workflow 결과 | SUCCESS |
| 기존 Repository unit/regression workflow | PASS |
| 실제 Rockchip 보드 검증 | `TBD` / P1 이관 |

---

## 9. 요구사항 추적 요약

| 요구사항 | 검증 의미 | Software 결과 | 실물 Validation |
|---|---|---|---|
| SBSD-FWD-004/005 | ROI와 거리 독립 판정 및 높은 severity 유지 | PASS | `TBD` |
| SBSD-FWD-007/008 | receding/velocity 상태가 기본경고를 낮추지 않음 | PASS | `TBD` |
| SBSD-FWD-009 | 거리추정 오류 시 DANGER ROI 방어 | fault-injection PASS | `TBD` |
| SBSD-FWD-010 | 10m 이내 DANGER | 경계시험 PASS | `TBD` |
| SBSD-FWD-011 | 10~30m WARNING, 30m 밖 SAFE | 경계시험 PASS | `TBD` |
| SBSD-FWD-014 | TTC≤2.5초 DANGER 격상 | 경계시험 PASS | `TBD` |
| SBSD-FWD-015 | 30m 밖 TTC 조기경고 미사용 | Software PASS | `TBD` |
| SBSD-FWD-020/021/022 | Turn Signal 격상과 반대편 비간섭 | PASS | 실제 입력·출력 `TBD` |
| SBSD-IF-008 | direct ego speed 없이 상대거리/TTC 사용 | Software capability PASS | 정확도 `TBD` |
| SBSD-STD-003 | Type III/SAV 제품정책 | Software CLOSED | ISO/실차 `TBD` |

---

## 10. 향후 시험 및 TBD

### 10.1 Lab 녹화영상 및 Ground Truth

| 시험 | 주요 조건 | 상태 |
|---|---|---|
| Forward ROI 물리경계 | 차량 측면, rear 0~10m, 10~30m, >30m, LEFT/RIGHT | `TBD` |
| Forward Distance | 3/5/6/7/8/9/10/15/20/30m, lateral 위치별 오차 | `TBD` |
| ROI+Distance 이중 방어 replay | ROI 오차와 거리 오차를 각각 주입 | `TBD` |
| Velocity/TTC replay | UNKNOWN, same-speed, receding, TTC 2.5s 경계 | `TBD` |
| Turn Signal/HMI replay | same/opposite side, 동시 위험, rear 화면 OSD | `TBD` |
| Ego Motion | STOPPED, LOW_SPEED, MOVING, HIGHWAY, TURN, tunnel/low-texture | `TBD` |
| Reverse Assist | LEFT/RIGHT/REAR 1.5m·3m, R ON/OFF, 동시 위험 | `TBD` |
| Lab FP/FN | Ground Truth dataset 기반 검출·경보 누락/오경보 | `TBD` |

### 10.2 실제 Rockchip 보드

| 시험 | 상태 |
|---|---|
| Linux BSP/Kernel 및 SSH bring-up | `NEEDS_BOARD` |
| `/dev/video*`, media/V4L2 topology 및 3채널 mapping | `NEEDS_BOARD` |
| AHD/MIPI 실제 frame capture 및 pixel format/resolution/FPS | `NEEDS_BOARD` |
| RKNN Runtime, model load, NPU inference 및 latency | `NEEDS_BOARD` |
| GPIO/Vehicle I/O mapping | `NEEDS_BOARD` |
| Display/LED/Buzzer 실제 출력 | `NEEDS_BOARD` |
| CPU/NPU/RAM/thermal, frame drop 및 장시간 안정성 | `NEEDS_BOARD` |
| Camera disconnect/reconnect 및 fault recovery | `NEEDS_BOARD` |

### 10.3 실제 차량

| 시험 | 상태 |
|---|---|
| 최종 카메라 장착 위치 및 ROI | `NEEDS_VEHICLE` |
| rear trailing-edge 기준 10m/30m Calibration | `NEEDS_VEHICLE` |
| Reverse 1.5m/3m Calibration | `NEEDS_VEHICLE` |
| same-speed·closing target 및 TTC 정확도/지연 | `NEEDS_VEHICLE` |
| 실제 Turn Signal/Reverse 입력과 HMI 출력 | `NEEDS_VEHICLE` |
| motorcycle-sized target | `NEEDS_VEHICLE` |
| FP/FN, 야간, 우천, 터널, 역광 및 진동 | `NEEDS_VEHICLE` |
| 온도, 전원 및 endurance | `NEEDS_VEHICLE` |

### 10.4 표준 적합성

| 시험/검토 | 상태 |
|---|---|
| ISO 17387:2026 정식 전문 확보 | `TBD` |
| Clause 4/5/6 및 Annex 추적 | `TBD` |
| Full conformity test matrix | `TBD` |
| 표준 조건에 따른 실제 시험 | `TBD` |
| ISO 17387:2026 full compliance 선언 | `NOT_CLAIMED` |

---

## 11. 제한사항 및 대외 표현

현재 근거로 사용할 수 있는 표현:

```text
Side BSD의 정의된 Software Scope에 대해 컴파일 및 Repository 자동시험
169건을 수행했으며 169건 모두 통과했다.

ROI/Distance fail-safe, 10m/30m 경계, TTC 2.5초 격상 및 관련
Production-path 소프트웨어 동작을 자동시험으로 확인했다.
```

현재 사용하면 안 되는 표현:

```text
실차 검증 완료
실제 거리 정확도 검증 완료
보드 성능 검증 완료
False Negative/False Positive 목표 충족
ISO 17387:2026 완전 적합
전체 시스템 안전성 입증 완료
```

---

## 12. 근거 문서

1. `docs/sidebsd/00_index/CURRENT_IMPLEMENTATION_BASELINE.md`
2. `verification/history/legacy_docs_verification/sidebsd/SIDE_BSD_TYPEIII_SAV_SOFTWARE_CLOSURE.md`
3. `verification/history/legacy_docs_verification/sidebsd/REQUIREMENT_TRACEABILITY_MATRIX.md`
4. `verification/history/legacy_docs_verification/sidebsd/REQUIREMENT_TRACEABILITY_MATRIX_TYPEIII_SAV_ADDENDUM.md`
5. `verification/history/legacy_docs_verification/sidebsd/logic/01_project_analysis.md` ~ `11_final_logic_review.md`
6. `verification/history/legacy_docs_verification/sidebsd/fix/fix_request.md`
7. `verification/sidebsd/history/Side_BSD_Board_Diagnostic_P0_완료보고서.md`
8. `docs/sidebsd/00_index/CURRENT_VALIDATION_ROADMAP.md`
9. `docs/sidebsd/04_validation/Side_BSD_시스템_Validation_계획_및_절차서.md`

---

## 13. Snapshot 발행 규칙

프로그램 수정 및 재검증 시 내부 실행 근거를 먼저 갱신한다. 대외 Snapshot을 새로 발행할 때만 다음을 수행한다.

1. 변경 목적, 요구사항 ID, 기준 commit 및 변경 파일을 기록한다.
2. `tests/side_bsd/`와 영향받는 `tests/common/`을 실행한다.
3. 공유 코드 변경이면 실제 import/call path에 따라 Forklift/ADAS 회귀시험도 수행한다.
4. `verification/sidebsd/README.md` 절차로 변경분과 영향 범위를 논리 검증한다.
5. 내부 근거를 검토하여 본 문서의 기준선, 종합 판정과 시험 실행 이력을 새 revision에 반영한다.
6. FAIL/GAP은 삭제하지 않고 조치와 재검증 결과를 연결한다.
7. Lab·Board·Vehicle 시험은 본 보고서에 합산하지 않고 `docs/sidebsd/04_validation/`의 단계별 결과보고서에서 관리한다.
8. 자동시험 수치는 서로 다른 기준선의 결과를 단순 합산하지 않는다.

내부 시험 실행 이력 형식:

| 실행 ID | 일자 | 기준 commit/config | 시험 환경·데이터 | 수행 범위·건수 | PASS | FAIL/ERROR | 미검증·제한 | 근거 위치 |
|---|---|---|---|---:|---:|---:|---|---|
| `TBD` | `YYYY-MM-DD` | `TBD` | `TBD` | `TBD` | `TBD` | `TBD` | `TBD` | `TBD` |

---

## 14. 개정 이력

| 개정 | 일자 | 변경 내용 |
|---|---|---|
| v0.1 | 2026-09-13 | 기존 논리검증, Type III/SAV Software Closure, 169/169 자동시험 및 Board Diagnostic P0 결과를 통합하고 향후 항목을 TBD로 분리 |
| v0.2 | 2026-09-13 | 대외 제출용 Software Verification 결과보고서로 문서 역할을 명확화하고 내부 근거 기록과 실환경 Validation 결과의 관리 위치를 분리 |
