# Side BSD 보드 배포 패키지 가이드

Status: CURRENT
Document ID: SBSD-DEPLOY-001
Revision: v1.1
Applies to: Side BSD
Tool baseline: `main @ ec327e5`
Last verified: 2026-09-30
Approval: SELF-REVIEWED

## 1. 목적

`AI_Camera` 저장소 전체를 Rockchip 보드에 복사하지 않고, Side BSD 실행과 보드 Bring-up에 필요한 파일만 자동으로 묶어 배포하기 위한 절차를 정의한다.

현재 저장소에는 Side BSD, Forklift, ADAS와 공통 모듈이 함께 있으므로 보드별 패키지는 Manifest를 기준으로 생성한다.

## 2. 배포 프로파일

### `sidebsd_bringup`

보드 입고 직후 P1 Bring-up과 하드웨어 연동에 사용한다.

포함 범위:

- `run_side_bsd.py`
- `src/apps/side_bsd/`
- Side BSD가 실제 import하는 `src/modules/` 파일
- Side BSD가 실제 import하는 `src/shared/` 파일
- `tools/board_diag/`
- `deploy/models/rk3576/`의 FP·INT8 시험 모델과 `shared` RKNN 어댑터
- 로그/리포트/모델용 빈 runtime 디렉터리

제외 범위:

- Forklift app
- ADAS app
- `docs/`
- `tests/`
- Jupyter Notebook
- Windows `.bat`
- 과거 로그/개발 보조 파일

### `sidebsd_production`

P1 보드 연동과 실차 검증 이후 최소 runtime 배포를 위한 골격이다.

Bring-up 프로파일과 달리 `tools/board_diag/`를 포함하지 않는다. 현재는 실제 RKNN 모델, Camera device path, GPIO/Vehicle I/O가 확정되지 않았으므로 **구조만 준비된 상태**이며 즉시 양산 배포 가능한 패키지라는 의미는 아니다.

## 3. 패키지 생성

프로젝트 루트에서 실행한다.

```bash
python tools/deploy/build_package.py sidebsd_bringup
```

생성 위치:

```text
dist/sidebsd_bringup/
```

Production 골격 생성:

```bash
python tools/deploy/build_package.py sidebsd_production
```

생성 위치:

```text
dist/sidebsd_production/
```

`dist/`는 생성물이므로 Git에 커밋하지 않는다.

## 4. 패키지 검증

```bash
python tools/deploy/validate_package.py dist/sidebsd_bringup
```

검증 항목:

- entrypoint 존재
- Manifest include 파일/디렉터리 존재
- Forklift/ADAS/docs/tests 등 금지 경로 미포함
- `.bat`, `.ipynb`, `.pyc` 미포함
- 패키지 내부 Python 코드의 `apps`, `modules`, `shared` 로컬 import 누락 검사
- 기본 모델이 PC `.pt`이고 보드에서 `AI_CAMERA_MODEL`을 선택하지 않으면 WARN
- RKNN 모델/BSP runtime이 아직 TBD이면 WARN

WARN은 보드 입고 전 단계에서 예상되는 상태이며 구조 검증 실패와 구분한다.

## 5. 생성 패키지 구조 예

```text
dist/sidebsd_bringup/
├── DEPLOYMENT_MANIFEST.json
├── PACKAGE_INFO.json
├── run_side_bsd.py
├── deploy/models/rk3576/  # RKNN FP·INT8 시험 모델
├── src/
│   ├── apps/
│   │   └── side_bsd/
│   ├── modules/
│   ├── shared/
│   └── model/
├── tools/
│   └── board_diag/
├── logs/
├── reports/
└── video/
    ├── in/
    └── out/
```

Forklift/ADAS 소스는 Side BSD 패키지에 포함하지 않는다.

## 6. 모델과 Linux Runtime

현재 `.pt` 모델 파일은 Git에 포함되지 않는다. RK3576용 FP 모델과 INT8 변환 시험 모델은 `deploy/models/rk3576/`에 있고 `sidebsd_bringup`/`sidebsd_develop` 패키지에 포함된다. 보드에서 선택하는 명령은 다음과 같다.

```bash
AI_CAMERA_MODEL=rk3576_fp python3 run_side_bsd.py
```

`AI_CAMERA_MODEL` 기본값은 `pc`이다. INT8 시험 모델은 `rk3576_i8_test`로 선택할 수 있지만 제품용 calibration이 완료된 모델이 아니다. 최종 보정 모델은 `AI_CAMERA_MODEL_PATH=/절대경로/model.rknn`으로 지정한다. 모델별 상태와 복사 경로는 `deploy/models/rk3576/README.md`를 참조한다.

모델 파일의 패키지 포함과 구조 검증은 확인됐지만 RK3576 보드에서의 RKNNLite 추론, runtime/driver 호환성, 실제 경보 성능은 아직 확인되지 않았다.

또한 저장소 루트 `requirements.txt`는 PC 개발 환경용 패키지가 섞여 있으므로 Rockchip 보드에 그대로 설치하는 것을 표준 배포 절차로 사용하지 않는다. 보드용 Python/OpenCV/RKNN 의존성은 공급사 BSP와 실제 런타임을 확인한 후 별도로 확정한다.

## 7. 보드 입고 후 실제 순서

```text
Vendor Linux image 설치
→ 보드 부팅 / UART 확인
→ Network / SSH 확인
→ PC에서 sidebsd_bringup 패키지 생성·검증
→ 패키지를 보드로 전송
→ Board Diagnostic 실행
→ V4L2 / Camera 3CH 확인
→ RKNN/NPU 확인
→ GPIO/Vehicle I/O 확인
→ Side BSD config를 실제 보드값으로 확정
→ Board Diagnostic PASS
→ run_side_bsd.py 실행
```

보드로 전송하는 구체적인 `scp`/`rsync` 명령은 실제 사용자명, IP, 설치 경로가 정해진 뒤 확정한다. 비밀번호나 개인 SSH key는 저장소에 기록하지 않는다.

## 8. 실행 전 반드시 확인할 Side BSD 설정

현재 코드에는 PC 개발용 설정이 남아 있으므로 실제 보드에서 전체 Side BSD를 실행하기 전에 최소 다음을 확인한다.

- `OPERATION_MODE`: 실보드 정책에 맞게 확정
- LEFT/RIGHT/REAR 입력 경로: 실제 V4L2 device mapping으로 확정
- `DISPLAY_OUTPUT_MODE`: 실제 display 구성에 맞게 확정
- `AI_CAMERA_MODEL=rk3576_fp` 선택 및 RKNNLite/runtime/driver: 보드에서 검증
- HardwareIO UART/GPIO/CAN 연결: BSP/회로도 기준으로 확정

이 값들은 보드 입고 전 추정해서 Production 코드에 하드코딩하지 않는다.

## 9. 검증 범위의 한계

패키지 검증 PASS는 **필요한 파일 구조와 로컬 Python import가 패키지 내부에서 닫혀 있다는 뜻**이다.

다음을 의미하지 않는다.

- 카메라 3채널이 실제로 수신됨
- AHD Decoder/MIPI CSI가 정상임
- RKNN/NPU inference가 정상임
- GPIO/Vehicle I/O가 정상임
- Side BSD 실차 성능이 검증됨

이 항목들은 Board Diagnostic P1 및 실차 검증에서 별도로 확인한다.
