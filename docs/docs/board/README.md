# RK3576 AI_Camera 보드 입고부터 설치까지 — 통합 운영 매뉴얼

> 파일: docs/board/README.md (보드 절차의 유일한 운영 매뉴얼)
>
> 작성: 2026-10-06 / 상태: 개발·실보드 검증 진행 중
>
> 대상: 아무것도 설치하지 않은 Windows 개발 PC, IAC-RK3576-Kit, AI_Camera Side BSD
>
> 목표: **처음 사용하는 사람도 이 문서 순서대로 GUI 위주로 진행**한다. 과거 원본 시험 로그는 증거로 남기되 신규 절차 문서는 더 만들지 않는다.

## 0. 구분해야 할 세 가지

| 이름 | 역할 | 보드 변경 |
|---|---|---|
| AI_Camera Manager | 개발자용 Windows GUI, 보드 진단·파일 관리·환경 점검·빌드 | 기능에 따라 다름 |
| Factory Release | 개발자가 PC에서 만드는 오프라인 앱 배포 묶음 | 만들기만 해서는 변경 없음 |
| Factory GUI | 공장 작업자가 Release 폴더에서 실행하는 설치 GUI | **INSTALL을 누르면 보드 파일·시스템 설정 변경** |

**중요:** 신품 RK3576 보드의 /userdata/ai_camera 폴더는 **없어도 정상**이다. Factory Installer가 필요할 때 만든다. 그러나 기존 **개발 보드에 /userdata/ai_camera가 있으면 그 경로는 실험 자료가 있는 곳이므로 Factory GUI의 INSTALL을 누르지 않는다.**

Factory Installer는 보드의 Debian OS/BSP, NPU 커널 드라이버 및 /lib/librknnrt.so를 새로 설치하지 않는다. 이것들이 갖춰진 보드에 앱, venv, RKNN 모델, Native .so 및 runtime profile을 설치한다. 양산 전에는 공급사 Golden Image 기준을 정해야 한다.

## 1. 보드가 처음 도착했을 때

### 1-1. 준비물

1. IAC-RK3576-Kit 보드와 공급사가 지정한 전원 어댑터. 임의 전압 인가 금지.
2. Windows PC, 데이터 통신 가능한 USB Type-C 케이블(충전 전용 케이블 불가).
3. 회사 GitHub 저장소 WOOSEOK99/AI_Camera 접근 권한.
4. 개발 GUI를 실행할 Windows **.NET 8 SDK**. 다운로드: https://dotnet.microsoft.com/download/dotnet/8.0
5. ADB. Android Studio 전체는 필요하지 않다. 기본 SDK platform-tools가 이미 있거나, Google 공식 페이지 https://developer.android.com/tools/releases/platform-tools 에서 Windows zip만 받아도 된다.

보드 전원 연결 후 **J6 USB Type-C**로 PC와 연결한다. Windows **장치 관리자**에서 USB 장치를 확인한다. 기존 실보드에서는 rk3xxx, VID_2207&PID_0006으로 나타났다. 다른 보드는 표기가 달라질 수 있다.

현재 작업 중인 기존 보드에는 **LEFT/RIGHT/REAR 실제 카메라가 연결되어 있지 않다.** 카메라 sensor probe가 없는 경우 NOT CONNECTED로 기록해야 하며, 카메라 불량 판정을 내리면 안 된다.

### 1-2. PC에 저장소 내려받기

이미 D:\AI\AI_project가 있다면 **새로 복제하지 말고 이 단계를 건너뛴다.**

새 PC라면 Git for Windows를 설치하고 Windows 탐색기에서 D:\AI 폴더를 만든 후 Git Bash를 연다.

    cd /d/AI
    git clone https://github.com/WOOSEOK99/AI_Camera.git AI_project

GitHub 로그인/접근 오류는 저장소 권한 문제로 먼저 해결한다. 복제 후 Windows 파일 탐색기에 D:\AI\AI_project\tools\ai_camera_manager 폴더가 보이면 정상이다.

기존 PC에서 최신 코드를 받는 경우에만 PowerShell을 열고 다음을 실행한다.

    cd D:\AI\AI_project
    git status -sb
    git pull --ff-only

로컬 변경이 있으면 확인 후 커밋/정리한다. 근거 없이 git reset --hard나 stash pop을 하지 않는다.

## 2. AI_Camera Manager 시작 및 보드 연결

1. Windows 탐색기에서 **D:\AI\AI_project\tools\ai_camera_manager\Start_Manager.bat 더블클릭**.
2. 처음 실행은 C#/.NET 8 컴파일 때문에 시간이 걸릴 수 있다. 오류 메시지가 나오면 파일명·줄 번호를 기록한다.
3. 위쪽 **Connect / Refresh** 버튼을 누른다.
4. Connected: (장치 serial) 표시를 확인한다. 기존 보드에서 확인된 serial은 41e8925477df8a50이었으나 새 보드는 달라도 정상이다.
5. ADB가 2대 이상 연결됐다면 **한 대만 남기고 다시 연결**한다.

ADB not found이면 Android SDK 플랫폼 도구가 설치됐는지 확인한다. 보통 C:\Users\(사용자)\AppData\Local\Android\Sdk\platform-tools 아래에 adb.exe가 있다. Factory Release/sidebsd_bringup의 adb 폴더를 가진 경우도 Manager가 찾는다. 수동 연결 확인이 필요한 경우 **PowerShell에서만**:

    & "$env:LOCALAPPDATA\Android\Sdk\platform-tools\adb.exe" devices

마지막 열이 device면 정상이다. unauthorized/offline 상태라면 USB 권한·보드 펌웨어/케이블을 먼저 확인한다. Windows 장치 관리자에서 알 수 없는 장치면 드라이버 설치가 필요할 수 있다.

### Manager 메뉴 한눈에 보기

| 탭 | 초보자가 누를 버튼 | 용도 |
|---|---|---|
| Hardware Diagnostic | Run All Read-Only Checks | OS/CPU/RAM/스토리지/NPU/GPU/VPU/MPP/RGA/네트워크/온도/GPIO/I2C/카메라 현황 |
| Environment Audit | Scan Board / Compare Factory | 개발 보드와 Factory requirements 비교, 읽기 전용 |
| Environment Audit | Save JSON + CSV | 검사 원본/비교표를 Windows PC에 저장 |
| Dual File Manager | PC → Board / Board → PC | 양쪽 폴더 트리 탐색, 파일 복사 |
| Dual File Manager | Delete PC / Delete Board | **삭제 전 확인**. 조심해서 사용 |
| Dual Terminals | 각 명령창 Execute command | PC PowerShell 또는 보드 adb shell **명령 1회씩 실행** |
| PC Setup / WSL | WSL status / Install / Ubuntu / Linux packages / Native prerequisites | WSL 준비 및 SDK·컴파일러 사전 확인 |
| Build / Release | Build librknn_detector.so (WSL) | ARM64 Native 추론 라이브러리 빌드 |
| Build / Release | Verify / Stage Native .so | 결과 ELF 검사 후 Factory vendor 디렉터리로 복사 |
| Build / Release | Build Factory Release | 오프라인 공장 설치 패키지 생성(보드 설치 아님) |

**주의:** Dual Terminals는 현재 영구 인터랙티브 셸이 아니라 *일회성 명령 실행기*다. cd 명령을 입력해도 다음 명령의 작업 디렉터리로 이어지지 않는다. 보드 shell은 root일 수 있으며 파일 관리자 보호 범위가 적용되지 않으므로 검증되지 않은 rm/apt/시스템 명령을 실행하지 않는다.

### 2-1. 보드 하드웨어 검사

USB 연결 완료 → **Hardware Diagnostic → Run All Read-Only Checks**. 결과를 각 항목별로 본다.

기존 보드 실측 기준:
- SoC RK3576 / aarch64, Debian 12, Kernel 6.1.141, Python 3.11.2
- RKNN Runtime 2.3.2, RKNPU Driver 0.9.8
- NPU, MPP 인코더와 병렬 부하 시험 수행
- 현재 실제 카메라 미연결: 카메라 항목은 검사 보류

새 보드에서 값이 다르면 **BSP 차이로 기록**하고 원인을 확인한다. 이 버튼의 결과가 모두 출력된다고 해서 공식 출하 합격은 아니다. 없거나 실패한 항목은 '물리적으로 미연결 / 기능 미설치 / 동작 오류'를 구분한다.

### 2-2. 개발 보드 소프트웨어 환경 점검

**Environment Audit → Scan Board / Compare Factory → Save JSON + CSV** 순서.

2026-10-06 기존 보드에서 조사 완료:
- Factory 기존 시험용 venv와 개발용 venv는 각각 동일한 Python 29개 패키지.
- MATCH 15, REVIEW 27, INVENTORY 1,467, INFO 2, NOT INSTALLED 1.
- 필수 OS/Factory pinned 버전의 MISSING 및 VERSION DIFF는 표시되지 않음.
- GStreamer, MPP, Python, RKNN 주요 도구 존재. ffmpeg CLI는 PATH에 없음.
- TEST용 장치의 active release(/userdata/ai_camera/current) Native .so가 없다는 것은 새 Factory INSTALL 실패를 의미하지 않는다.
- **개발 보드의 apt 1,467개를 Factory Installer에 전부 설치하는 것은 잘못**. Golden Image 기준과 비교해야 한다.
- Factory pinned 최상위 패키지: rknn-toolkit-lite2, numpy, opencv-python, supervision, psutil, ruamel.yaml, packaging. 설치기는 오프라인 wheelhouse에서 나머지 전이 의존성까지 설치한다.
- **중요 보완 후보:** Side BSD 거리 보정 모듈은 scipy.optimize.curve_fit을 import한다. 개발·Factory 테스트 venv에는 scipy==1.16.3이 있지만 Factory 최상위 requirements에서 직접 pin하지 않았다. 필수 직접 의존성인지 확인 및 버전 고정 여부 검토 필요.
- UART HardwareIO는 pyserial이 없을 때 Mock. 실제 차량 I/O가 준비되면 pyserial·UART 핀/장치/속도를 검증 후 Factory requirements에 추가한다.

결과 CSV/JSON은 사내 검사 증거로 보관한다. 패키지와 경로, 장치 serial이 포함되므로 외부 공유를 제한한다.

### 2-3. 폴더 열기와 파일 복사

**Dual File Manager**에서 왼쪽은 Windows PC, 오른쪽은 Linux 보드다.

1. 좌우 경로창에 원하는 폴더를 입력하거나 폴더 트리에서 선택.
2. 양쪽 목록 Refresh로 다시 표시.
3. PC 파일 선택 → **PC → Board** → 경로/덮어쓰기 확인 후 Yes.
4. 반대로 보드 파일 선택 → **Board → PC**.
5. Activity log에서 완료 또는 오류를 확인한다.

파일 관리자는 PC의 저장소 내부, 보드의 /userdata/ai_camera 내부 쓰기를 제한하도록 설계했다. **기존 개발 경로에서 파일을 삭제하는 행위는 복구되지 않을 수 있으므로 사용 전 확인한다.** 기존 수동 adb push/pull 반복 대신 GUI를 사용한다. 명령창은 예외 조치에만 사용한다.

## 3. Windows PC에 WSL 설치하기 — Native .so를 새로 빌드할 때만

보드 연결/진단/파일 전송/기존 Native .so를 사용한 Factory Release 생성에는 **WSL이 필수가 아니다**. C/C++ ARM64 Native 라이브러리 *새 빌드*를 할 때 필요하다.

### 3-1. PC Setup / WSL 탭에서 설치

1. **1. Check WSL status** 버튼 → 설치된 Ubuntu와 WSL 상태 확인.
2. Ubuntu가 이미 있다면 새로 설치하지 않는다.
3. 없다면 **2. Install WSL + Ubuntu** → 경고 확인 → Yes → Windows UAC 관리자 승인.
4. 인터넷 연결과 관리자 정책에 따라 설치가 중단될 수 있다. Windows 재시작을 요구하면 재부팅한다.
5. Manager 재실행 → **3. Open Ubuntu setup**. Ubuntu 첫 실행에 Linux 사용자 이름/비밀번호를 직접 정한다. 비밀번호 입력 중 화면에 문자가 나타나지 않는 것은 정상이다.
6. **4. Install Linux build packages** 버튼을 누른다. Ubuntu 별도 콘솔에서 sudo apt-get update와 sudo apt-get install -y cmake make build-essential 실행. Ubuntu 비밀번호를 요구할 수 있다.
7. **5. Check Native build prerequisites**로 컴파일러와 SDK 파일을 확인한다.

**WSL 설치 버튼은 설치 명령을 시작하는 기능이지 설치 성공을 보증하지 않는다.** 반드시 Check WSL status와 Native prerequisites를 재실행하여 확인한다.

Windows에서 GUI 버튼이 실패하면 **관리자 PowerShell에서만** 공식 표준 명령을 사용한다.

    wsl --install -d Ubuntu

설치 완료 후 재부팅 및 시작 메뉴의 Ubuntu 실행으로 계정을 만든다. 회사 PC는 IT 부서의 승인/권한이 필요할 수 있다. Microsoft 안내: https://learn.microsoft.com/windows/wsl/install

### 3-2. WSL 준비 이후에도 빠질 수 있는 것

**WSL 설치 ≠ RKNN SDK / ARM64 cross-compiler 설치**다. 현재 빌드 스크립트가 기대하는 검증 당시 경로는:

    Ubuntu: /home/ai/rknn_model_zoo/
             └─ 3rdparty/rknpu2/include/rknn_api.h
             └─ 3rdparty/rknpu2/Linux/aarch64/librknnrt.so

    Ubuntu: /home/ai/tools/cross_compiler/arm/
             └─ gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/
                ├─ aarch64-linux-gnu-gcc
                └─ aarch64-linux-gnu-g++

Ubuntu 사용자 이름이 ai가 아닌 경우 **위 경로를 그대로 가정하면 실패한다**. 검증된 회사 SDK/툴체인을 해당 경로로 준비하거나 WSL 환경변수 MODEL_ZOO_ROOT와 RKNN_CROSS_BIN을 실제 위치에 맞춰 설정한다. 현재 Build GUI는 별도의 SDK 경로 선택창이 없다. **임의 인터넷 링크에서 다른 RKNN SDK/크로스 컴파일러를 자동 다운로드해 덮어쓰지 않는다.**

정확한 SDK/컴파일러 아카이브 공급·재현성 절차는 아직 표준화되지 않았다. 이 단계가 막히면 필요한 파일 출처를 회사가 확정해야 한다. Native Preflight 버튼에서 MISSING이 표시된 경로를 기록한다.

Ubuntu 터미널을 반드시 사용해야 할 때 파일 존재 확인 예:

    ls -l /home/ai/rknn_model_zoo/3rdparty/rknpu2/include/rknn_api.h
    ls -l /home/ai/rknn_model_zoo/3rdparty/rknpu2/Linux/aarch64/librknnrt.so

Windows 탐색기 주소창에 \\wsl$\Ubuntu\home 를 입력하면 Linux 홈 폴더를 볼 수 있다. Windows D: 프로젝트는 보통 Ubuntu에서 /mnt/d/AI/AI_project로 표시된다.

### 3-3. Native .so 빌드와 결과 사용

1. **PC Setup / WSL → 5. Check Native build prerequisites**에서 필수 항목 PASS.
2. **Build / Release → Build librknn_detector.so (WSL)**.
3. Activity log에 성공, AArch64 ELF 검증 완료 확인.
4. **Verify / Stage Native .so**를 눌러 추후 Factory Release에서 사용하는 파일로 복사.

예상 결과 위치(PC):

    D:\AI\AI_project\build\rknn_detector_rk3576\librknn_detector.so
    D:\AI\AI_project\tools\factory\vendor\native\librknn_detector.so

이 단계는 컴파일/형식 검증이다. 실제 보드에서 Native .so가 로드되고 RKNN이 제대로 추론되는 것은 별도 시험이다.


### 3-4. 보드 YOLOv8 + Native 최종 재확인

최신 Manager의 **Build / Release → Verify YOLOv8 + Native on Board** 버튼은 읽기 전용이다. 보드 파일/설정을 바꾸지 않고 다음을 한 번에 확인한다.

- 보드의 `yolov8n_rk3576_i8_test.rknn` 검색 및 SHA-256 비교
- 기준 SHA-256: `1B855C0ED7DD4CE143CCD711958BEB7459FC94804F5326CBFD262ECAF73075A7`
- `librknn_detector.so` 검색 및 AArch64 ELF 확인
- 배포된 `rknn_native.py`를 통해 YOLOv8 모델 로드
- 기존 보드 테스트 이미지가 있으면 해당 프레임으로 Native RKNN 1회 추론, 없으면 검은 640×640 smoke frame 사용
- 최종 `BOARD_YOLOV8_NATIVE_VERIFY: PASS` 출력 확인

이 PASS는 **모델 파일 동일성 + Native 런타임 동작 확인**이다. 실제 카메라의 검출 정확도, 거리/TTC, 실제 차량 경보까지 합격했다는 뜻은 아니다.

**2026-10-06 실보드 실행 결과: PASS**

- 보드 모델: `/userdata/ai_camera/deploy/models/rk3576/yolov8n_rk3576_i8_test.rknn`
- 모델 SHA-256: `1B855C0ED7DD4CE143CCD711958BEB7459FC94804F5326CBFD262ECAF73075A7` → GitHub 추적본과 일치
- Native .so: `/userdata/ai_camera/deploy/native/rk3576/librknn_detector.so`
- Native .so SHA-256: `4414B7D6066330E58BFA81CA4BE1B72BA1C8B482C0DB12F62EDC2F90633EF529`
- AArch64 ELF 확인: PASS
- 테스트 프레임: `/userdata/rknn_yolov8_demo_rk3576/model/left_side_frame.jpg`
- Native 추론 결과: detection 5개
- 최종 결과: `BOARD_YOLOV8_NATIVE_VERIFY: PASS`

따라서 **현재 개발 보드에서 GitHub에 등록된 YOLOv8n 모델 + 현재 Native `.so` 조합이 실제 RKNN 추론까지 정상 동작함을 재확인했다.**

## 4. 우리가 실보드에서 수행했던 작업 및 결과

초기에는 터미널에서 직접 하던 작업들이다. 이제 같은 목적에 대응하는 GUI를 우선 사용한다.

| 초기 실제 작업 | 확인 결과 / 제한 | 현재 권장 방법 |
|---|---|---|
| Windows USB 장치 인식 및 ADB 접속 | rk3xxx / ADB 정상 | Windows 장치 관리자 + Manager Connect |
| Debian/CPU/RAM/eMMC/네트워크 확인 | RK3576 / Debian 12 / ARM64 검증 | Hardware Diagnostic |
| Python 3.11, RKNN Runtime, NPU driver 검사 | Python 3.11.2, RKNN Runtime 2.3.2, Driver 0.9.8 | Hardware Diagnostic + Environment Audit |
| NPU RKNN 샘플 inference | 정상 실행 확인; 2026-10-06 YOLOv8 + Native 실구동도 PASS | NPU 진단 + Build / Release의 Board YOLOv8 Native Verify |
| MPP H.264/H.265 단일·3채널 synthetic 인코더와 NPU 병행 | 해당 조건에서 동작 확인, 카메라 capture와 다름 | 현재 자동 벤치마크 버튼 없음. 과거 근거 로그 보존 |
| GC05A2 / V4L2 / media graph / I2C / GPIO 진단 | 조사 수행. 실제 카메라 물리 미연결 | Hardware Diagnostic. 실센서 연결 전 probe 실패를 불량이라 판단 금지 |
| RKNN YOLOv8/YOLO11 및 zero-copy 비교 | 구조/성능 시험 수행 | 과거 측정 근거 유지; 자동 성능 비교 버튼은 추후 |
| ARM64 Native .so 빌드 | 좌우 독립 Native context 사용 | PC Setup / WSL → Build / Release |
| PC 모델/코드/.so 보드 복사 | ADB push/pull 활용 | Dual File Manager |
| CPU/NPU/DMC 주파수 및 affinity 설정 | 성능 profile 스크립트 작성 | Factory runtime profile, Hardware Diagnostic read-back |
| Factory offline bundle 생성 | 빌드 PASS / GUI READY 확인 | Build / Release → Build Factory Release |
| Factory Installer 구버전 시험 경로 설치 | /userdata/ai_camera_factory_test에 과거 1회 PASS | 최신 신규 설치/업데이트는 **신규 보드 대기** |

### 성능 실측 핵심 (실카메라 결과와 구별)

- YOLO 640×640 INT8 **Native dual RKNN**: 약 47.23 pair FPS.
- **SideCamera.infer()** 좌우 병렬: 약 35.20 pair FPS.
- **Side-only overlap** (후방 녹화영상 디코드 제외): 약 30.82 FPS.
- **Full app**에서 후방 H.264 디코드 제외: 약 31~33 FPS.
- **1080p REAR H.264 녹화영상 CPU decode 포함**: 약 19~22.7 FPS. 720p 변경 시 약 26~29 FPS.
- Rockchip MPP hardware decode는 별도 시험에서 정상 빠르게 동작했으나 현재 Python OpenCV venv는 GStreamer 비활성.
- LEFT/RIGHT 임시 영상 calibration RMSE = 1.1764m 각각 (현재 threshold 0.75m 초과); REAR = 0.3915m.
- 640×640을 성능 때문에 512×512로 낮추지 않는다. RGA 전처리 시도는 결과 동등성/성능 문제로 되돌렸다.
- 녹화영상 후방 디코더 병목 때문에 생산 구조에 성급히 MPP sidecar를 추가하지 않는다. **최종 3채널 입력 성능은 실제 카메라 연결 후 다시 검증**한다.

### 현재 YOLO 모델 기준 (2026-10-06)

RK3576 개발은 **YOLO11로 시작했지만 성능 검증 과정에서 YOLOv8n 640×640 INT8로 전환**했다.
현재 GitHub에도 실제 YOLOv8n RKNN 파일이 등록되어 있으며, 앞으로 RK3576 INT8의 기준 모델은 YOLOv8n이다.

현재 파일:

`deploy/models/rk3576/yolov8n_rk3576_i8_test.rknn`

SHA-256:

`1B855C0ED7DD4CE143CCD711958BEB7459FC94804F5326CBFD262ECAF73075A7`

GitHub 등록 commit: `0c311c7`

| 선택값 `AI_CAMERA_MODEL` | 모델 파일 | 용도 |
|---|---|---|
| `rk3576_i8_test` | `yolov8n_rk3576_i8_test.rknn` | **현재 RK3576 INT8 기본 의미** |
| `rk3576_yolov8_i8_test` | 위와 동일 | 명시적 YOLOv8 선택 |
| `rk3576_yolo11_i8_test` | `yolo11n_rk3576_i8_test.rknn` | 과거 시험 재현용 |
| `rk3576_yolo11_fp` | `yolo11n_rk3576_fp.rknn` | 과거 FP 시험 재현용 |
| `pc` | `src/model/yolo11n.pt` | Windows 기존 개발 경로; RK3576 현재 기준과 구분 |

Factory Release의 기본 selector도 `rk3576_yolov8_i8_test`로 변경했다.
Factory manifest에는 이제 **선택한 모델 경로와 모델 SHA-256**을 같이 기록한다.

Native `librknn_detector.so`는 특정 모델 파일 자체가 아니라 Rockchip의
640×640 / 80-class / 9-output DFL tensor contract를 처리하는 추론 엔진이다.
프로젝트의 Native 구현은 호환되는 YOLOv8/YOLO11 Model Zoo 출력 레이아웃을 지원하도록 작성되어 있다.
과거 Python 파일명 `rknn_yolo11.py`는 import 호환 때문에 유지하지만 내부 설명과 클래스는 공통 Model Zoo detector로 정리한다.

**완료된 확인:** GitHub 추적 YOLOv8 모델과 개발 보드 모델의 SHA-256 일치, AArch64 Native `.so`, 실제 테스트 프레임 Native RKNN 추론까지 2026-10-06 PASS.

**아직 남은 확인:** 파일명이 `_test`이므로 실제 카메라 입력, 최종 데이터셋 기준 검출 정확도/오검출/미검출, 거리/TTC 및 실제 차량 경보까지 검증한 뒤에만 양산 최종 모델로 승격한다.

## 5. Factory Release 만들기와 실제 신규 보드 설치

### 5-1. 개발 PC에서 Factory Release 생성

1. Git의 변경 파일을 확인하고 승인된 커밋 기준으로 정리한다. 빌더는 변경 중인 작업트리를 기본적으로 거부한다.
2. ARM64/cp311 오프라인 wheelhouse와 get-pip.py, adb.exe 및 DLL, ARM64 librknn_detector.so 준비.
3. Manager **Build / Release → Build Factory Release**. 이 버튼은 기존 tools/factory/build_factory_release.ps1을 실행한다.
4. Activity log의 FACTORY RELEASE BUILD: PASS 확인. 예상 위치:

    D:\AI\AI_project\dist\factory\AI_Camera_Factory_Release\
      Factory_GUI.bat
      Factory_Install.bat
      RELEASE_INFO.txt
      adb\adb.exe
      payload\factory_manifest.json
      payload\ai_camera_sidebsd.tar.gz
      payload\wheelhouse\*.whl

5. 위 **폴더 전체**를 공장 PC에 전달한다. 공장 작업자 PC에는 Python/Git/WSL/개발 소스가 없어도 된다.

Manager 빌드 버튼에 문제가 있으면 예비 진입점 tools\factory\Build_Factory_Release.bat을 파일 탐색기에서 실행한다. **릴리스 빌드 성공은 실제 보드 설치 성공이 아니다.**

### 5-2. 신품 RK3576 설치 예정 절차 — 아직 최신 버전 실보드 검증 전

1. 공급사 승인 Debian/BSP 이미지가 준비된 RK3576 보드 전원을 켠다.
2. Windows 공장 PC에 **한 대만** USB/ADB 연결.
3. Factory Release 폴더의 **Factory_GUI.bat** 더블클릭.
4. 연결 장치 serial, Release ID, model, runtime profile, READY 확인 후 INSTALL.
5. USB/전원을 빼지 않는다. 최종 GUI PASS/FAIL과 로그를 확인한다.
6. 앱 폴더 /userdata/ai_camera는 최초에 **없어도** Installer가 incoming/releases/persistent 등의 하위 폴더를 자동 생성한다.
7. 검증 대상으로는 offline wheelhouse venv 설치/pip check, SHA-256, RKNN 모델 load/init/inference, Native .so 실제 동작, runtime profile, current release 정보를 확인한다.

설계된 폴더:

    /userdata/ai_camera/
      venv/
      releases/<release-id>/
      current  -> 활성 릴리스
      previous -> 이전 릴리스
      persistent/logs/
      persistent/reports/
      persistent/video/in/
      persistent/video/out/
      FACTORY_INSTALL_INFO.json

**경고:** 기존 개발 보드에서 Factory GUI INSTALL 버튼을 누르면 이 기본 경로를 변경한다. 개발 보드를 보존하려면 승인된 시험에서만 명시적으로 -AppRoot /userdata/ai_camera_factory_test를 지정하는 CLI 시험을 이용한다. 다만 이 경우에도 설치 스크립트의 CPU/NPU/DMC 및 systemd runtime profile은 **보드 전역**으로 바뀔 수 있다.

Factory Test의 RKNNLite dummy inference PASS는 Native backend의 완전 검증이나 카메라/차량 안전성 승인 PASS가 아니다.

### 5-3. 신규 보드 업데이트(A→B) 및 재부팅 시험 — 아직 미수행

**신규 보드가 없으므로 현재 보류.** 서로 다른 Release ID A, B를 준비해 아래 순서로 확인한다.

1. 앱 폴더가 없는 보드에 A 최초 설치, 해당 릴리스와 영속 데이터의 상태 기록.
2. B로 업데이트 후 current=B, previous=A 확인.
3. persistent/logs, reports, video 데이터가 삭제되지 않았는지 확인.
4. RKNN 모델과 Native .so 로드, 앱 의존성, 설치 traceability 및 기록된 해시 확인.
5. CPU0~3 2016 MHz, CPU4~7 2208 MHz, NPU 950 MHz, DMC 2112 MHz read-back.
6. 재부팅 후 ai-camera-rk3576-runtime.service 및 값 유지 확인.
7. 같은 Release ID 재설치, SHA mismatch, 저장공간 부족, USB 분리, 전원 오류 시 FAIL/복구 및 이전 정상 릴리스 보호 여부 확인.
8. Factory GUI PASS/FAIL/진행률과 공장 PC 환경(개발툴 없는 PC) 재현성 확인.

위 단계가 모두 검증되기 전에는 Factory UPDATE VALIDATED라고 표시하지 않는다. 이전 릴리스 링크 변경과 별도로 venv/runtime 전역 설정이 바뀔 수 있으므로 링크 rollback만으로 전체 상태 복구가 완료된다고 가정하지 않는다.

## 6. 문제 해결 — 먼저 GUI, 안 될 때만 명령어

| 증상 | GUI로 먼저 할 일 | 그래도 안 될 때 |
|---|---|---|
| Manager가 시작되지 않음 | Windows .NET 8 SDK 확인 | 실행 창의 C# 파일명/줄 번호 기록, git pull 뒤 재실행 |
| ADB not found | SDK platform-tools/Factory ADB 폴더 확인 | PowerShell에서 adb.exe devices |
| Board disconnected/offline | Connect / Refresh, USB 데이터 케이블·J6·보드 전원 | 장치 관리자와 드라이버 확인 |
| NPU 라이브러리 없음 | Hardware Diagnostic / Environment Audit | BSP의 /lib/librknnrt.so 공급 여부 확인 (pip로 해결 안 됨) |
| 카메라가 없음 | Hardware Diagnostic Cameras | 카메라 물리 연결 전이라면 정상 보류 |
| WSL 설치 실패 | PC Setup / WSL → Check WSL status | 관리자 PowerShell: wsl --install -d Ubuntu |
| Ubuntu 이름/비밀번호 요구 | Open Ubuntu setup | 첫 실행 사용자 생성, 비밀번호 입력은 보이지 않음 |
| Native prerequisite MISSING | Check Native build prerequisites | 검증 SDK/툴체인 경로 확인, 무작위 다운로드 금지 |
| Factory dirty Git 상태 | git status 확인 후 변경 검토/커밋 | 승인 없이 git reset --hard 또는 -AllowDirty 사용 금지 |
| Factory wheelhouse 없음 | tools/factory/vendor 및 기존 테스트 bundle 확인 | ARM64 cp311 전체 오프라인 wheelhouse 준비 |
| RKNN_ERR_MODEL_INVALID 경고 | 실제 반환 텐서/종료 코드 확인 | static-shape dynamic-range 경고만으로 FAIL 아님 |
| Calibration RMSE 초과 | 조건/Calibration 확인 | 최종 실카메라 측정 후 재보정 |
| 개발용 파일이 날아갈 우려 | Factory GUI INSTALL 사용 금지 | 격리 시험용 -AppRoot 및 전역 runtime 변경 검토 |

## 7. 기타: 전체 한 일과 앞으로 할 일 (2026-10-06 기준)

### 7-1. 완료 (완료 범위가 명시된 항목만)

- [x] 보드 수령/부팅, Windows USB/ADB 연결, RK3576 Debian/ARM64 기본 환경 확인
- [x] CPU/RAM/eMMC, RKNPU/RKNN runtime/MPP 기본 시험 및 synthetic encoder/NPU 병렬 부하
- [x] YOLO11/Yolov8 RKNN·native zero-copy/CPU 배치 성능 실험
- [x] 640×640 Native dual context, SideCamera 병렬화, ego-motion/capture overlap 구현
- [x] RGA preprocessing 시도 후 동등성/성능 문제로 revert
- [x] PC Python 3.11 회귀시험 28개 PASS (시험 당시 해당 소스 범위)
- [x] recorded-video REAR H.264 software decode 병목 및 MPP decode 동작 확인
- [x] Factory Installer 구버전 **시험 전용 경로** 설치 PASS (최신판과 구분)
- [x] Windows Factory Release builder 생성 PASS / Factory GUI 보드 연결·READY 확인
- [x] Factory runtime performance profile 코드 작성(신규 bundle 보드 재검증은 아래)
- [x] AI_Camera Manager Windows C#/.NET GUI 구현 및 기본 실행 확인
- [x] Hardware Diagnostic / Dual File Manager / Dual Terminals / Native build GUI 코드 추가
- [x] Environment Audit GUI 구현, 개발 보드 실검사 성공, JSON·CSV 내보내기 기능 추가
- [x] PC Setup / WSL 설치 시작·상태/선행조건 점검 UI 코드 추가 (실제 버튼 실행 검증은 아래)

### 7-2. 현재 보드/PC에서 앞으로 할 일

- [ ] **PC Setup / WSL** 신규 GUI 버튼 사용 시험, 관리자 설치·재부팅/Ubuntu 첫 실행 확인
- [ ] WSL RKNN SDK 및 ARM64 cross-compiler 경로 표준화/공급본 checksum 문서화
- [ ] Manager 각 탭의 실제 전송·보호·Native .so GUI 빌드 및 Factory Release 버튼 검증
- [x] 실제 YOLOv8n RKNN 파일 GitHub 등록 및 RK3576/Factory 기본 selector를 YOLOv8로 전환
- [x] 개발 보드 YOLOv8 모델 SHA-256 일치 및 현재 Native `.so` 실제 RKNN 추론 재검증 PASS (2026-10-06, left_side_frame, detection=5)
- [ ] Python scipy 직접 import에 대한 Factory 최상위 의존성 선언 여부 확인; pyserial은 실 UART 후
- [ ] BSP/Golden Image 필수 라이브러리 vs 개발 패키지 분리 확정
- [ ] Native RKNN context/worker lifecycle 종료 및 재시작 안정성 시험
- [ ] BENCHMARK dual context 계측, 시작/종료 반복 memory/스레드 누수 확인
- [ ] 영상 기반 예비 발열/전력 장시간 시험(실카메라 열 검증과 구별)
- [ ] TEST_ROI_DIRECT_ALERT_MODE, HardwareIO Mock 및 production config 최종 정책
- [ ] 공장 시험 전용 AppRoot를 GUI에서 지정하거나 별도 보드로 격리하는 절차 확정

### 7-3. 신규 보드가 입고되면 재개할 일

- [ ] **최신 Factory GUI**로 앱 폴더 없는 보드에 최초 INSTALL
- [ ] 서로 다른 릴리스 A→B 업데이트, current/previous, persistent 및 traceability 확인
- [ ] Native .so 실제 로드와 NPU 추론, 재부팅 후 CPU/NPU/DMC runtime profile
- [ ] 동일 버전 재설치·설치 실패/USB 끊김/rollback·Golden Image/EOL 절차
- [ ] 공장 Windows PC에서 완전 오프라인 반복 설치 재현성 시험

### 7-4. 실카메라/차량 장비가 준비되면 할 일

- [ ] LEFT/RIGHT/REAR 실제 sensor 연결, ISP/media graph/V4L2 채널 매핑
- [ ] 실제 3채널 FPS/latency/frame drop, 지속 부하/온도/전력
- [ ] 물리 거리 Calibration (LEFT/RIGHT 0.75m RMSE 요구치) 재검증
- [ ] 실제 UART/차량 방향지시등/후진 신호, LED/부저/HMI, 장애 복구
- [ ] 차량/도로 시나리오 FP/FN/TTC, 출하 기준, Golden Image·버전 승인

**절대 혼동하지 말 것:** 녹화영상 FPS와 실제 카메라 FPS는 다르다. 카메라 미연결은 불량이 아니다. WSL 설치는 SDK 설치가 아니다. Factory Release 빌드 성공은 Factory INSTALL 성공이 아니다. 앱 단독 import PASS는 실제 Native 추론/차량 검증 완료가 아니다.

## 8. 향후 문서 운영 원칙

- **보드 관련 모든 신규 작업 절차와 TODO는 이 파일 docs/board/README.md만 수정**한다. 날짜별 새 인수인계/보드 가이드를 docs 최상단에 만들지 않는다.
- 기존 기록은 검증 증거이므로 **임의로 삭제하지 않는다**. 새 작업자는 지금 읽는 매뉴얼로 시작한다. 별도 과거 원시 측정값이 필요한 경우에만 아래 증거 문서를 참고한다.
- 상세 실측 로그: docs/RK3576_Board_Bringup_Test_Log.md, docs/RK3576_YOLOv8_Performance_Validation_2026-10-06.md, docs/sidebsd/04_validation/RK3576_SideBSD_Native_Pipeline_Performance_2026-10-06.md
- 구현/인수인계 근거: docs/HANDOFF_RK3576_Native_Inference_2026-10-06.md, docs/RK3576_Factory_Deployment_Guide.md
- USB/ADB 이전 상세 안내: docs/datasheet/RK3576_Windows_ADB_Connection_Guide.md
- 도구 상세 기능: tools/ai_camera_manager/README.md; Factory 원시 스크립트 안내: tools/factory/README.md
- 새 검증은 실행날짜, 장치 serial, source commit, 모델/라이브러리 hash, 시험조건, 실제 결과, PASS/FAIL 또는 NOT_TESTED를 같이 기록한다.
