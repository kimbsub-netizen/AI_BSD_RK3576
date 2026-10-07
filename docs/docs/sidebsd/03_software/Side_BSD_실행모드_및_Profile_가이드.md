# Side BSD 실행 모드 및 Profile 가이드

> Status: CURRENT  
> Updated: 2026-09-15

## 1. 목적

Side BSD 실행 설정은 두 축으로 분리한다.

```text
OPERATION_MODE = 실행 환경과 시간 기준
SESSION_PROFILE = 실행 목적에 따른 관찰·기록 설정
```

Profile은 PC Preview, 개발 annotation, 기록, 진단 로그와 성능 집계만 바꾼다. ROI,
거리 calibration, TTC, 경고 threshold, AlertStateMachine, VelocityTracker 및
Reverse Assist 정책은 바꾸지 않는다.

## 2. OPERATION_MODE

| 값 | 입력 환경 | 시간 기준 | 시험 override |
|---|---|---|---|
| `TEST` | 검증된 녹화 영상 | recorded timestamp | 허용 |
| `PRODUCTION` | 실차 live camera | `perf_counter` | 금지/무시 |

`PRODUCTION`에서 `TEST_ROI_DIRECT_ALERT_MODE=True` 조합은 시작 시 오류로
차단한다. ROI-direct mode는 정상 거리/TTC 경로를 우회하는 시험 정책이며
화면 표시 옵션이 아니다.

## 3. SESSION_PROFILE

| Profile | 목적 | Preview | Record | 주요 표시/로그 |
|---|---|---:|---:|---|
| `NORMAL` | 일반 제품 동작 | OFF | OFF | 개발 annotation/log OFF |
| `CALIBRATION` | ROI·거리·flow 확인 | ON | ON | ROI, box, class, track, 거리/속도, HUD |
| `DIAGNOSTIC` | 상세 문제 분석 | ON | ON | CALIBRATION 표시 + timing/log/perf |
| `DEMO` | Detection/Tracking/BSD 시연 | ON | ON | box, class, ID, age, trail, 거리, alert |
| `BENCHMARK` | 순수 처리 성능 측정 | OFF | OFF | aggregate performance monitor만 ON |

기본 개발 설정은 다음과 같다.

```python
OPERATION_MODE = "TEST"
SESSION_PROFILE = "DIAGNOSTIC"
```

최종 일반 양산 조합은 다음과 같다.

```python
OPERATION_MODE = "PRODUCTION"
SESSION_PROFILE = "NORMAL"
```

## 4. Profile별 resolved 값

`ON/OFF` 이외의 유일한 값은 `DEBUG_PRINT_INTERVAL`이다.

| 설정 | NORMAL | CALIBRATION | DIAGNOSTIC | DEMO | BENCHMARK |
|---|---:|---:|---:|---:|---:|
| `SHOW_PREVIEW` | OFF | ON | ON | ON | OFF |
| `RECORD_VIDEO` | OFF | ON | ON | ON | OFF |
| `SHOW_ROI` | OFF | ON | ON | OFF | OFF |
| `SHOW_DETECTION_BOXES` | OFF | ON | ON | ON | OFF |
| `SHOW_DETECTION_CLASS` | OFF | ON | ON | ON | OFF |
| `SHOW_TRACK_ID` | OFF | ON | ON | ON | OFF |
| `SHOW_TRACK_AGE` | OFF | ON | ON | ON | OFF |
| `SHOW_TRACK_TRAIL` | OFF | ON | ON | ON | OFF |
| `SHOW_DISTANCE` | OFF | ON | ON | ON | OFF |
| `SHOW_VELOCITY` | OFF | ON | ON | OFF | OFF |
| `SHOW_OBJECT_ALERT_LEVEL` | OFF | ON | ON | ON | OFF |
| `SHOW_SYSTEM_HUD` | OFF | ON | ON | OFF | OFF |
| `SHOW_FPS_ON_FRAME` | OFF | ON | ON | OFF | OFF |
| `DEBUG_PRINT_INFER_TIME` | OFF | OFF | ON | OFF | OFF |
| `DEBUG_PRINT_INTERVAL` | 0 | 100 | 100 | 0 | 0 |
| `LOG_ALARM_TO_CONSOLE` | OFF | ON | ON | OFF | OFF |
| `PERF_MONITOR_ENABLED` | OFF | ON | ON | OFF | ON |
| `SIDE_TARGET_DEBUG_LOG` | OFF | OFF | ON | OFF | OFF |

## 5. Product HMI, Preview와 recording

Product HMI frame 생성, PC Preview, recording은 서로 독립적으로 동작한다.

```text
Camera Input
    → Detection / Tracking / BSD Logic
    → Product HMI Rendering
        → Production Display Sink (RK3576 연동 지점)
        → SHOW_PREVIEW=True일 때만 cv2.imshow()
        → RECORD_VIDEO=True일 때만 VideoWriter.write()
```

`SHOW_PREVIEW`는 PC 개발용 OpenCV Preview Window와 키 입력/창 정리
(`cv2.imshow()`, `cv2.waitKey()`, `cv2.destroyAllWindows()`)만 제어한다.
양산 HMI 영상이나 Product BSD OSD를 끄는 옵션이 아니다.

`RECORD_VIDEO`는 처리·annotation된 영상을 `VideoWriter`로 저장할지만 제어한다.
양산 HMI 출력이나 Product BSD OSD와 독립적이다.

```text
SHOW_PREVIEW=False, RECORD_VIDEO=True  → window 없음, writer 있음
SHOW_PREVIEW=True,  RECORD_VIDEO=False → window 있음, writer 없음
SHOW_PREVIEW=False, RECORD_VIDEO=False → window/writer 없음, Product HMI frame은 생성
```

`PRODUCTION + NORMAL`에서는 Preview와 recording이 모두 OFF여도
`SIDE_BSD_OSD_ENABLED=True`, `REAR_BSD_OSD_ENABLED=True`이면 Product HMI OSD가
동작한다. SAFE/NORMAL 상태에는 방향 OSD가 없고, WARNING에는 노란색 계열,
DANGER에는 빨간색과 기존 점멸 효과가 표시된다. ROI, detection box/class,
track ID/age/trail, 거리·속도 숫자, System HUD와 FPS는 계속 OFF이다.

현재 PC Python 구현의 production display sink는 의도적으로 비어 있다.
최종 선택·합성 및 Product OSD가 적용된 frame은
`SideBSDSystem._output_hmi_frame()`까지 항상 전달되며, RK3576 Linux/Debian의
DRM/KMS, V4L2 또는 HDMI framebuffer 출력은 이 메서드에 연결한다.

`SHOW_VIDEO`는 `SHOW_PREVIEW`의 deprecated read alias이다.
`SHOW_ROI_AND_BOXES`도 호환 alias이며 신규 코드는 세부 flag를 사용한다.

## 6. DEMO 표시

DEMO는 ROI 밖에서 추적 중인 차량도 display-only snapshot으로 유지한다.
경고 판단용 객체 목록과 분리되어 있으므로 표시 record의 생성 실패가
WARNING/DANGER 결과에 영향을 주지 않는다.

```text
CAR #31 · TRACK 2.8s · 12.4m · WARNING
```

Track age는 TEST에서 recorded timestamp, PRODUCTION에서 monotonic timestamp를
사용한다. 거리 sample 수인 `VelocityTracker.history_cnt`를 재사용하지 않는다.
최근 bottom-center 위치 8개를 trail로 보관하며 mode transition/reset 또는
missed threshold 초과 시 제거한다.

## 7. Target Qualifier

```text
OFF     = 계산 비활성
SHADOW  = 분류·로그만 수행, 경고 결과 변경 금지
ENFORCE = 예약값; 현재도 SHADOW 동작으로 처리하며 suppression 없음
```

현재 권장값은 `SHADOW`이다. `NEW_INSIDE_RECEDING`을 포함한 어떤 qualifier
결과도 객체 skip, SAFE downgrade 또는 WARNING/DANGER suppression에 사용하지 않는다.

## 8. 사용 예

```text
PC 영상 분석                 TEST + DIAGNOSTIC
캘리브레이션                 TEST/PRODUCTION + CALIBRATION
기술 시연                    TEST/PRODUCTION + DEMO
실차 Shadow 데이터 수집      PRODUCTION + DIAGNOSTIC + SHADOW
일반 양산                    PRODUCTION + NORMAL
순수 성능 측정               TEST/PRODUCTION + BENCHMARK
```

실행 시 최종 operation mode, session profile, preview/recording, annotation,
qualifier 및 ROI-direct 상태를 한 번 출력한다.
