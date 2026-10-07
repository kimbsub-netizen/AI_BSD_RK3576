# AI 작업용 프로젝트 컨텍스트
## 목적
이 문서는 AI가 Side BSD 프로젝트의 코드를 수정할 때 먼저 읽어야 하는 **압축형 작업 기준서**입니다.

`docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md`는 Side BSD 폴더/파일 역할과 데이터 흐름을 파악하는 지도이고, 본 문서는 **변경 시 지켜야 할 시스템 동작 규칙과 금지사항**을 요약합니다.
설계 의도는 `docs/sidebsd/Side_BSD_시스템_기술_설계서.md`, 현재 동작은 실제 소스 코드로 확인합니다. 둘이 다르면 어느 한쪽을 임의로 정답으로 선택하지 않고 **설계-구현 불일치**로 보고합니다.

---

## 1. 작업 기본 원칙

1. 기존 구조를 존중하고, 요청 범위를 벗어난 리팩터링을 하지 않는다.
2. 기존 동작을 임의로 변경하지 않는다.
3. 설정값으로 관리되는 값은 코드에 하드코딩하지 않는다.
4. `TEST` 전용 동작을 `PRODUCTION`에 노출하지 않는다.
5. 안전장치/FAULT 처리/캐시 만료 로직을 단순화하거나 제거하지 않는다.
6. Side BSD와 Rear Camera는 목적과 필터 정책이 다르므로 동일 로직으로 통합하지 않는다.
7. 코드 수정 후에는 관련 호출부, 설정값, 로그/테스트 경로까지 함께 확인한다.
8. 불확실한 요구사항은 임의로 추측하지 말고 기존 코드/설계 문서에서 근거를 확인한다.

---

## 2. 시스템 핵심 구조

- 입력: LEFT / RIGHT / REAR 3채널
- 목표 하드웨어(설계): Rockchip RK3576, 6 TOPS NPU
- 권장 양산 모델(설계): YOLO11n INT8 / RKNN
- 현재 기본 개발 설정: `src/model/yolo11n.pt` (`config_env.py`의 실제 값을 배포 전 재확인)
- 실행 진입점: `run_side_bsd.py`
- 메인 컨트롤러: `app_side_bsd.py`

핵심 모듈:
- `modules/side_camera.py`: 사이드 BSD
- `modules/rear_camera.py`: 후방 주차/후진 보조
- `modules/side_frame_scheduler.py`: 좌/우 추론 스케줄링
- `shared/velocity_tracker.py`: 상대속도
- `shared/distance_calibrator.py`: 거리 추정
- `shared/ego_speed_estimator.py`: Sparse Optical Flow
- `shared/ego_motion_fusion.py`: 주행상태 융합
- `shared/hardware_io.py`: CAN/GPIO/UART 등 물리 I/O
- `shared/visualizer.py`: OSD/HUD
- `shared/recording_validator.py`: TEST 녹화 검증

---

## 3. 실행 모드 불변 규칙

### PRODUCTION
- 항상 실시간 `time.perf_counter()` 기준
- 테스트 영상의 타임스탬프 로그를 사용하지 않음
- 실제 차량/보드 동작 기준

### TEST
- 검증된 녹화 영상이면 촬영 당시 CSV 타임스탬프 사용
- 검증되지 않은 녹화는 조용히 실시간 시간으로 대체하지 말고 오류 처리
- `TEST_ROI_DIRECT_ALERT_MODE`는 TEST에서만 의미 있음
- 이 옵션은 ROI 좌표 확인용이며 정식 거리/속도 판정 검증용이 아님

---

## 4. Side Camera 핵심 동작

기본 흐름:
1. YOLO 추론
2. ByteTrack ID 추적
3. 객체 바닥점 계산
4. ROI 판정
5. DistanceCalibrator로 거리 추정
6. VelocityTracker로 상대속도 계산
7. 오작동 필터
8. 경보 판정
9. 결과 캐시

### 주행 중 객체 클래스
기본적으로 차량 관련 COCO 클래스만 사용한다.
후진 기어가 ON이면 주차 보조 목적상 클래스 제한을 해제하여 전체 객체 감지를 사용한다.

### 마주오는 차량 필터

**설계-구현 불일치:**

- 설계서는 `INCOMING_FILTER_NEAR_M` 이내를 무조건 통과시키고, 그보다 멀면서
  `INCOMING_FILTER_FAR_M` 이내인 속도 미확정 객체는 순간속도가
  `FAST_APPROACH_INSTANT_VELOCITY_MS` 이상일 때만 통과시키도록 정의한다.
- 현재 `side_camera.py`는 속도 미확정이고 `dist_m <= INCOMING_FILTER_FAR_M`이면
  모두 통과시킨다.
- `INCOMING_FILTER_NEAR_M`은 import만 되고 판정에 사용되지 않으며,
  `FAST_APPROACH_INSTANT_VELOCITY_MS`도 판정 비교에 사용되지 않는다.
- 영향은 3~5m 구간 반대편 차량의 False Positive 증가 가능성이다. 어느 동작을
  최종 요구사항으로 채택할지는 확인이 필요하며, 수정 시 3m/5m 및 8m/s 전후
  경계 테스트를 수행한다.

### 멀어지는 객체 필터
- 상대속도가 자차 상태별 `IGNORE_AWAY_VELOCITY_MS_*`보다 작으면 무시
- 대표 기본값은 -5.0 m/s
- 상태별 임계값은 별도 관리한다

### 경보
- 실제 거리와 ROI/경보 조건은 반드시 현재 설정값을 확인한다.
- 방향지시등에 따른 WARNING → DANGER 격상 로직을 유지한다.
- 정차 억제는 `EGO_MOTION_BSD_GATE` 설정에 의해 제어된다.

---

## 5. Rear Camera 핵심 동작

Rear Camera는 Side BSD와 다르게 **속도 필터를 사용하지 않는다.**

목적:
- 후진/주차 시 정지 물체(벽, 기둥, 보행자 등)도 감지

흐름:
1. YOLO
2. ByteTrack
3. REAR ROI
4. 거리 추정
5. 거리 기반 경보
6. 후진 기어와 부저 연동
7. 결과 캐시
8. 필요 시 Side BSD 상태를 OSD로 표시

후방 추론은 Side Scheduler와 독립적이다.

**설계-구현 불일치:** 기술 설계서는 `ThreadPoolExecutor` 기반 비동기 추론을
기술하지만, 현재 코드는 재시작 가능한 `multiprocessing.Process` 기반
`RearInferenceSupervisor`를 사용한다. 요청 큐 크기와 pending 상태로 미완료 작업이
쌓이지 않도록 제한하며, timeout 시 worker를 재시작하고 session ID로 이전 결과를
격리한다. 현재 동작을 검토할 때는 process/queue/session 경계를 기준으로 하고,
설계서는 별도 동기화가 필요하다.

---

## 6. Side Scheduler 핵심 규칙

평상시:
`ALTERNATING`

경보 발생:
`PRIORITY` → 위험 방향 연속 추론

이후:
`COOLDOWN` → 다시 `ALTERNATING`

중요:
- 좌/우 동시 경보는 FIFO 우선순위
- 지속 경보는 COOLDOWN 후 자동 재등록 가능
- 한쪽 방향의 영구 독점을 막기 위해 재등록 시작 방향을 번갈아 처리
- 상태 변경은 Lock으로 보호
- `PRIORITY_BOOST_FRAMES` 기본 3
- `PRIORITY_COOLDOWN_FRAMES` 기본 5

추론 FPS:
- `INFERENCE_FPS_PER_SIDE >= CAMERA_FPS`이면 풀-추론
- 그보다 낮으면 fractional rate accumulator 기반 교번 추론
- 좌/우 각각 목표 FPS를 장기 평균으로 맞추는 것이 목적

---

## 7. VelocityTracker 핵심 불변 조건

상대속도 부호:
- 접근 = 양수 (+)
- 멀어짐 = 음수 (-)

기본 처리:
- 거리 EMA
- 순간속도 계산
- 속도 이동평균
- 이상치 제거
- tracker 유실 유예 삭제

안전 조건:
- `dt <= 0`이면 속도 계산을 하지 않는다.
- 비정상적으로 큰 속도는 clipping/reject 규칙을 따른다.
- `tracker_id`는 `0`도 유효한 ID이므로 truthy 판정(`if tracker_id:`)으로 검사하지 않는다.
- 유실된 tracker의 이력을 무한정 유지하지 않는다.
- 시간 간격이 허용 범위를 넘으면 트랙을 재시작한다.

---

## 8. Ego Motion

`EgoSpeedEstimator`
- Dense Optical Flow가 아니라 **Sparse Optical Flow**
- Shi-Tomasi + Pyramidal Lucas-Kanade
- 하단 도로 영역을 사용
- 방향 일관성 검사
- 상위 극단값 제거
- EMA smoothing

중요:
이 값은 실제 차량 속도(m/s)가 아니라 화면상의 움직임 크기이다.

`EgoMotionFusion`
- 좌/우 Flow를 융합
- 기본적으로 `max(left_flow, right_flow)`
- 상태:
  - `STOPPED`
  - `STOP_CANDIDATE`
  - `LOW_SPEED`
  - `MOVING`
  - `HIGHWAY`

`EGO_MOTION_BSD_GATE=True`이면 정차/저속에서 BSD 부저를 억제할 수 있다.

---

## 9. DistanceCalibrator

기본 모델:
`d = a / (y_norm - b)`

- `y_norm` = 객체 바닥점 y / 영상 높이
- SciPy `curve_fit` 사용
- 실패 시 선형 fallback
- 소실점 영역 등 계산 불가 구간은 `None`

품질 관련:
- 과적합 의심 시 선형 fallback
- RMSE 기반 품질 진단
- `is_extrapolated()`는 실제 `side_camera.py`에서 호출·소비되고 있음이 코드 확인을 통해 검증되었다 (`docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md` 12장 참조).

---

## 10. 결과 캐시와 FAULT

추론이 스킵된 프레임은 마지막 결과를 재사용할 수 있다.

그러나:
- `MAX_INFERENCE_RESULT_AGE_SEC`를 넘긴 오래된 결과는 경보/OSD에 사용하지 않는다.
- 오래된 결과를 SAFE로 격리하고 채널 FAULT를 출력한다.
- FAULT와 DANGER/WARNING은 서로 다른 상태다.
- 후방 worker 예외/timeout 시 이전 결과를 무조건 유지하지 않는다.
- 후방 세션 종료 시 기존 세션 결과가 다음 세션으로 넘어가지 않도록 세대/캐시 처리를 유지한다.

---

## 11. TEST_ROI_DIRECT_ALERT_MODE

목적:
- 실차 거리 캘리브레이션 전에 ROI 좌표가 제대로 잡혔는지 눈으로 확인

동작:
- TEST 모드에서만 활성
- ROI 진입만으로 WARNING/DANGER
- 거리 계산/속도 계산/마주오는 차량 필터/멀어지는 차량 필터/거리 신뢰도 게이트를 우회
- 부저도 즉시 표시 검증 가능
- PRODUCTION에서는 코드 레벨에서 무조건 비활성

주의:
이 기능을 정식 BSD 판정 로직으로 사용하거나 차량 배포본에 활성화해서는 안 된다.

---

## 12. 오프라인 녹화/재생 검증

정상 워크플로:
`녹화 → 검증 → 캘리브레이션 → config 반영 → 실행 → 로그 분석 → 재튜닝`

녹화:
- 영상 + 프레임별 timestamp CSV

검증:
- 파일 존재
- CSV 형식
- frame_idx 연속성
- timestamp 단조 증가
- 영상 프레임 수 == CSV 행 수

TEST 재생:
- 검증된 녹화만 실측 timestamp 사용
- 검증 실패/파일 변경 시 RuntimeError
- 조용한 `perf_counter` fallback 금지

---

## 13. 설정 파일 변경 원칙

현재 설정은 3개 관심사로 분리된다.

### `config_env.py`
- 입력/출력 경로
- 모델 경로
- 실행 모드
- 화면/디버그/배포 옵션

### `config_calibration.py`
- 거리 캘리브레이션 데이터
- ROI 좌표
- Optical Flow 관련 카메라 장착/촬영 보정값
- 차량별 실측값이 가장 자주 변경되는 영역

### `config_logic.py`
- 스케줄러
- 결과 캐시 수명
- 경보 거리
- 속도 필터
- VelocityTracker
- Ego Motion 상태 판정
- 후방 추론 주기/timeout
- 급접근 임계값

새로운 튜닝값이 필요하면 어느 설정 영역에 속하는지 먼저 판단한다.

---

## 14. 실보드 배포 전 확인

반드시 확인:
- `.rknn` 모델 경로
- 실제 카메라 입력
- 좌/우/후방 거리 캘리브레이션
- 좌/우/후방 ROI
- Ego Motion threshold
- 디버그 출력 OFF
- 로그 저장 정책
- 카메라 좌우 반전
- CAN/GPIO 기반 방향지시등/후진기어
- 부저/LED 출력
- 채널 FAULT 출력

하드웨어 I/O는 현재 일부 TODO/Mock 상태일 수 있으므로 코드 상태를 확인하고 변경한다.

---

## 15. 현재 알려진 제한사항

- CAN/GPIO가 실제 연동되지 않은 개발 단계에서는 방향지시등/후진기어 입력이 Mock일 수 있다.
- 일부 Ego Motion calibration 출력값은 현재 runtime config에서 소비되지 않을 수 있다.
- 고성능 티어에서는 Scheduler PRIORITY 상태가 우회될 수 있다.
- 개발 툴의 기본값이 실제 운용 범위와 다를 수 있으므로 현장 적용 전 확인한다.

---

## 16. AI 작업 절차

코드 수정 요청을 받으면 다음 순서로 확인한다.

1. `docs/sidebsd/Side_BSD_시스템_기술_설계서.md`에서 설계 의도 확인
2. `docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md`로 Side BSD 구조와 대상 파일 파악
3. 이 문서에서 관련 불변 규칙/안전 조건 확인
4. 실제 대상 소스 코드와 테스트 확인
5. `config_*` 정의부터 실제 소비 지점까지 추적
6. 호출부/데이터 흐름과 설계-구현 불일치 영향 확인
7. 수정
8. 관련 테스트 또는 실행 경로 검증
9. 변경된 동작이 기존 설계 규칙과 안전 조건을 깨지 않는지 재검토

### 절대 하지 말 것
- 문서에 없는 동작을 임의로 추가
- 테스트 옵션을 양산 로직으로 전환
- 안전장치/FAULT/캐시 만료 제거
- Side/Rear의 필터 차이를 무시하고 공통화
- 설정값을 임의의 상수로 하드코딩
- 코드 일부만 보고 전체 동작을 단정
- 기존 기능을 제거하면서 대체 동작을 명시하지 않음

---

## 17. 판단 근거 우선순위

1. `docs/sidebsd/Side_BSD_시스템_기술_설계서.md` — 설계 의도와 요구사항
2. 실제 현재 소스 코드 — 구현 동작
3. 테스트 코드와 실제 테스트 결과
4. 본 `AI_PROJECT_CONTEXT.md`, `docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md` 및 기타 문서

(`project_overview.md`는 최신 소스 코드가 반영되지 않은 구식 문서로 확인되어 폐기되었다. Side BSD 프로젝트 구조 파악은 `docs/sidebsd/Side_BSD_CODEBASE_OVERVIEW.md`를 사용한다.)

우선순위는 불일치를 숨기고 상위 문서를 무조건 정답으로 간주한다는 뜻이 아니다.
설계와 코드가 다르면 설계 내용, 실제 구현, 영향, 확인 필요사항과 권장 수정 방향을
각각 기록한다.
