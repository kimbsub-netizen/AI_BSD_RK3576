# Side BSD Board Diagnostic Tool 요구사항서

Status: CURRENT
Document ID: SBSD-BDIAG-REQ-001
Revision: v0.2
Code baseline: 0d4b589fabf8bf3f8dbe08032752b1da882d0bc5
Last verified: 2026-09-12
Approval: SELF-REVIEWED

## 1. 목적

본 도구는 Side BSD 시스템이 Rockchip 기반 Linux 보드에서 정상적으로 동작하기 위한 보드·OS·카메라·NPU·Python 실행환경·입출력 장치의 상태를 자동 점검하는 것을 목적으로 한다.

1차 목적은 BSD 알고리즘 정확도 검증이 아니라 **현재 Side BSD 애플리케이션을 실행할 수 있는 보드 상태인지 빠르게 판정하는 것**이다.

실제 보드가 없는 현재 단계에서는 Mock 환경에서 동작 가능해야 하며, 실제 보드 입고 후에는 보드별 설정과 Hardware Adapter를 추가하여 실제 Board Diagnostic으로 전환할 수 있어야 한다.

## 2. 적용 대상

대상 시스템은 `src/apps/side_bsd/`를 중심으로 하는 Side BSD 시스템이다.

개념 구성:

```text
Left AHD Camera ─┐
Right AHD Camera ├─ AHD Decoder ─ MIPI CSI ─ Rockchip Linux
Rear AHD Camera ─┘                         │
                                          ├─ Camera Capture
                                          ├─ NPU / Object Detection
                                          ├─ Tracking
                                          ├─ Distance Estimation
                                          ├─ Ego Motion
                                          ├─ BSD Risk Logic
                                          └─ Display / Warning Output
```

실제 AHD Decoder의 Linux 노출 방식, MIPI 구성, `/dev/video*` 할당, RKNN Runtime 경로, GPIO 번호는 보드/BSP 확보 전이므로 TBD로 관리한다.

## 3. 기본 설계 원칙

### 3.1 Production 코드와 분리

진단 도구는 `tools/board_diag/`에 두고 Side BSD Production 알고리즘과 분리한다. 진단 도구 개발로 기존 risk/calibration/tracking 로직을 변경하지 않는다.

### 3.2 실행 모드

최소 다음 두 모드를 지원한다.

- Mock Mode: 실제 보드 없이 진단 흐름과 fault 처리를 검증
- Real Board Mode: Linux 보드의 실제 상태를 조회

권장 실행:

```bash
python -m tools.board_diag.board_check --mock
python -m tools.board_diag.board_check
```

### 3.3 진단 상태

각 진단 항목은 다음 중 하나를 반환한다.

- `PASS`: 정상
- `WARN`: 동작 가능하나 확인 필요
- `FAIL`: Side BSD 실행에 영향을 줄 수 있는 오류
- `SKIP`: 현재 환경/단계에서 검사하지 않음
- `UNKNOWN`: 정보 부족으로 판단 불가

## 4. P0 구현 범위 — 보드 입고 전

P0에서는 다음을 구현한다.

- Diagnostic Framework
- Result Model (`PASS/WARN/FAIL/SKIP/UNKNOWN`)
- Configuration Loader
- System Check
- Python Runtime Check
- Camera Check interface
- NPU Check interface
- GPIO Check interface
- Mock Hardware Layer / Mock profile
- Fault Injection
- Text Summary
- JSON Report
- CLI
- Unit Test
- GitHub Actions 연동(기존 repository test workflow에서 자동 발견)

P0에서는 실제 `/dev/video*` 프레임 획득, RKNN inference, GPIO 출력 제어를 완료 조건으로 요구하지 않는다. 실제 보드/BSP가 있어야 검증 가능한 항목은 명확히 `SKIP` 또는 `UNKNOWN`으로 처리한다.

## 5. System / Linux 진단

Real Board Mode에서 가능한 범위에서 다음 정보를 수집한다.

- OS / platform
- Kernel version
- Architecture
- Hostname
- CPU 수
- 총 메모리 / 사용 가능 메모리
- Storage 전체/여유 공간
- Uptime

보드 사양 기준치가 아직 확정되지 않았으므로 P0에서는 존재 여부와 수집 성공 여부 위주로 판정한다.

## 6. Python Runtime 진단

다음을 확인한다.

- Python version
- 핵심 module import 가능 여부
- 최소 요구 version은 별도 configuration으로 확장 가능

현재 repository의 실제 dependency를 기준으로 점검하되, 보드 BSP가 제공하는 OpenCV/RKNN 패키지가 PC/CI 환경과 다를 수 있으므로 Linux 보드용 버전 조건은 보드 입고 후 확정한다.

## 7. Camera 진단

### 7.1 목표 Role

- LEFT
- RIGHT
- REAR

### 7.2 P0

- Camera check interface 제공
- Mock에서 3채널 PASS/FAIL/누락 재현
- Real Board Mode에서 `/dev/video*` device discovery 가능
- Role mapping은 하드코딩하지 않고 configuration으로 확장 가능

### 7.3 P1

실제 보드 입고 후 다음을 추가/확정한다.

- V4L2 query
- device/driver/card name
- pixel format
- resolution
- FPS
- stream open
- 실제 frame capture
- empty/black frame 진단 보조
- LEFT/RIGHT/REAR role mapping
- snapshot 저장

## 8. NPU 진단

### P0

- NPU check interface
- Mock initialization/model/inference 성공·실패 시나리오
- 실제 runtime이 확정되지 않은 환경에서는 `SKIP`

### P1

Rockchip BSP/SDK 확인 후 다음 단계로 검증한다.

1. RKNN Runtime/library 존재
2. NPU initialize
3. test model load
4. test inference
5. inference latency 측정

Board NPU 동작 검증과 Side BSD YOLO 모델 정확도 검증은 분리한다.

## 9. GPIO / Vehicle I/O 진단

P0에서는 interface와 Mock만 제공한다. 실제 pin/line mapping이 확정된 이후 P1에서 다음을 연결한다.

- IGN
- Reverse
- Left Turn
- Right Turn
- Warning LED
- Buzzer
- Display control

GPIO 번호와 Linux 제어 API는 보드 회로도/BSP 확인 전에는 추정하지 않는다.

## 10. Display 진단

Display stack(DRM/framebuffer/Wayland/X11 등)이 확정되지 않았으므로 P0에서는 비대상이다. P1 이후 device 존재, resolution, test frame 출력 여부를 검사할 수 있도록 확장한다.

## 11. Preflight 판정

Board Diagnostic은 향후 Side BSD 실행 직전 다음과 같은 요약을 제공해야 한다.

```text
System                PASS
Python                PASS
Camera Left           PASS
Camera Right          PASS
Camera Rear           PASS
NPU Runtime           PASS
NPU Inference         PASS
GPIO                   SKIP
Display                SKIP

FINAL RESULT : PASS
```

P0에서는 실제 hardware 미확정 항목이 `SKIP`이어도 framework 자체의 최종 결과는 PASS 가능하다. 실제 Production Preflight에서 어떤 SKIP을 FAIL로 승격할지는 P1에서 정의한다.

## 12. 성능 진단 — P2 후보

실제 보드와 통합 애플리케이션 확보 후 다음 항목을 측정한다.

- Camera capture FPS
- AI inference FPS
- End-to-end FPS
- CPU usage
- Memory usage
- NPU inference latency
- Frame drop count
- Processing latency

성능 PASS/FAIL 기준은 실제 제품 성능 요구사항 확정 후 정의한다.

## 13. 로그 및 Report

최소 다음 출력을 지원한다.

- 콘솔 summary
- JSON report

권장 report 구조:

```text
reports/
└── YYYYMMDD_HHMMSS/
    ├── summary.txt
    └── report.json
```

P1에서 camera snapshot, V4L2/NPU 상세 로그를 추가할 수 있다.

## 14. CLI 요구사항

최소 다음 옵션을 지원한다.

```bash
python -m tools.board_diag.board_check --mock
python -m tools.board_diag.board_check --mock-profile all_pass
python -m tools.board_diag.board_check --mock-profile camera_failure
python -m tools.board_diag.board_check --system
python -m tools.board_diag.board_check --python
python -m tools.board_diag.board_check --camera
python -m tools.board_diag.board_check --npu
python -m tools.board_diag.board_check --json-report <path>
```

## 15. Mock / Fault Injection 요구사항

단순 Always-PASS Mock을 금지한다. 최소 다음 profile을 제공한다.

- `all_pass`
- `camera_failure`
- `npu_failure`
- `low_memory`
- `low_storage`
- `multiple_failure`

Mock 결과도 Real Board 결과와 동일한 Result Model/Report 경로를 통과해야 한다.

## 16. 자동 테스트

`tests/side_bsd/`에서 다음을 검증한다.

- Result 상태 및 final 판정
- Mock profile별 기대 결과
- JSON report serialization
- CLI 정상/오류 경로
- Real Board adapter가 hardware 부재 시 crash하지 않고 SKIP/UNKNOWN 처리하는지

기존 GitHub Actions의 `python -m unittest discover -s tests -t . -v`에 자동 포함되도록 작성한다.

## 17. Exit Code

권장:

- `0`: 최종 PASS
- `1`: 하나 이상의 FAIL
- `2`: 진단 도구 자체 실행 오류

WARN/SKIP 존재만으로 exit code 1을 반환하지 않는다. Production Preflight 정책은 P1에서 별도 확정한다.

## 18. Configuration 원칙

Board-specific 값은 진단 코드에 직접 하드코딩하지 않는다. P0에서는 표준 라이브러리만으로 읽을 수 있는 JSON configuration을 기본 지원하고, 필요 시 P1에서 YAML 지원을 추가한다.

예:

```json
{
  "camera": {
    "expected_roles": ["left", "right", "rear"],
    "devices": {
      "left": "auto",
      "right": "auto",
      "rear": "auto"
    }
  },
  "npu": {
    "runtime": "auto"
  },
  "gpio": {}
}
```

## 19. Production 코드 보호

- Side BSD Production algorithm 수정 금지
- risk logic 수정 금지
- calibration logic 수정 금지
- Mock code를 Production 실행 경로에 삽입하지 않음
- board-specific dependency를 Production core에 직접 추가하지 않음

진단툴 구현 과정에서 Production 변경 필요성이 발견되면 별도 GAP으로 보고하고 사용자 승인 후 처리한다.

## 20. 비대상

본 Board Diagnostic의 직접 합격 판정 대상이 아니다.

- BSD 위험영역 정확도
- 거리 추정 정확도
- TTC 정확도
- Optical Flow 정확도
- Tracking 정확도
- ISO 17387 성능 적합성
- 실제 도로 검증
- 카메라 calibration 품질

위 항목은 알고리즘 Verification / Vehicle Validation에서 별도 관리한다.

## 21. 단계별 완료 기준

### P0 — 보드 입고 전

- Framework/CLI/Report 동작
- System/Python 실제 PC·CI 진단 동작
- Camera/NPU/GPIO adapter 존재
- Mock fault injection 동작
- Unit test PASS
- 기존 Production 코드 무변경

### P1 — 보드 입고 후 Bring-up

- Linux BSP 확인
- Camera device/V4L2 topology 확인
- 3채널 role mapping
- 실제 frame capture
- RKNN Runtime/NPU test
- GPIO/Vehicle I/O mapping
- Display 방식 확인
- 실제 board profile 작성

### P2 — 통합 성능

- 3CH 동시 capture
- NPU inference
- 전체 pipeline FPS/latency
- frame drop
- CPU/RAM
- 장시간 실행
- crash/fault recovery

## 22. 현재 결론

현재 보드가 없는 상태에서는 **P0 Diagnostic Framework + Mock 검증까지 구현**한다. 실제 hardware-dependent 합격 기준과 device mapping은 추정하지 않고 P1로 이관한다.
