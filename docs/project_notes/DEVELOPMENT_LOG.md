# RK3576 AI BSD 개발 작업 로그

> 이 문서는 RK3576 AI BSD 프로젝트의 개발 과정과 확인 결과를 기록합니다.
> 현재 진행 위치는 DEVELOPMENT_CHECKLIST.md에서 관리합니다.

---

## 2026-10-02

### Git / 프로젝트 환경

- Git 저장소 확인
- GitHub Remote 연결 확인
- Git Push 확인
- DEVELOPMENT_CHECKLIST.md 생성
- DEVELOPMENT_CHECKLIST.md GitHub Push 완료

### 현재 프로젝트 경로

```text
/mnt/d/NAMSUNG/AI_BSD_RK3576
```

### GitHub 저장소

```text
https://github.com/kimbsub-netizen/AI_BSD_RK3576
```

### 체크리스트

현재 개발 진행 상태는 다음 파일을 기준으로 관리한다.

```text
docs/project_notes/DEVELOPMENT_CHECKLIST.md
```

### 작업 기록 규칙

- 작업 날짜
- 작업 내용
- 실행 명령
- 결과
- 오류 / 문제
- 해결 방법
- Git Commit
- 다음 작업

---

## 이후 작업 기록

각 개발 작업이 완료될 때마다 이 문서에 작업 결과를 추가한다.

---

## 2026-10-02 - Git 인증 자동화 및 프로젝트 정리 완료

- Git Push 성공 확인 (Commit: 413c9ee)
- GCM 임시 파일 `gcm-diagnose.log`, `gcm-linux-x64-2.9.1.deb` 삭제
- `git status --short` 출력 없음 확인

## 2026-10-06 - RK3576 SDK 압축 해제 및 구조 확인

- `~/rockchip/Rk3576` SDK 디렉터리 존재 확인
- RK3576 SDK 주요 디렉터리 및 파일 구조 확인
- 확인 항목: `kernel-6.1`, `buildroot`, `debian`, `device`, `external`, `u-boot`, `rkbin`, `tools`, `ubuntu22.04`, `rtos`
- `DEVELOPMENT_CHECKLIST.md` 28번 완료 처리

- Git Commit: 미실행
- 다음 작업: Cross Compiler 설치 및 동작 확인

## 2026-10-06 - Cross Compiler 설치 및 동작 확인

- Cross Compiler 설치 경로 확인
- `aarch64-none-linux-gnu-gcc` 실행 파일 존재 확인
- Arm GNU Toolchain 12.2.Rel1 확인
- GCC 버전 `12.2.1` 확인
- Cross Compiler 정상 동작 확인
- `DEVELOPMENT_CHECKLIST.md` 29번, 30번 완료 처리

- Git Commit: 미실행
- 다음 작업: Kernel 소스 확인

## 2026-10-06 - Kernel 소스 확인

- `~/rockchip/Rk3576/kernel` 심볼릭 링크가 `kernel-6.1`을 가리키는 것을 확인
- `~/rockchip/Rk3576/kernel-6.1` 디렉터리 존재 확인
- Linux Kernel 소스의 `Makefile` 및 `Kconfig` 존재 확인
- Kernel 소스 구조 정상 확인
- `DEVELOPMENT_CHECKLIST.md` 31번 완료 처리

- Git Commit: 미실행
- 다음 작업: Buildroot 환경 확인

## 2026-10-06 - Buildroot 환경 확인

- `~/rockchip/Rk3576/buildroot` 디렉터리 존재 확인
- Buildroot `Makefile` 존재 확인
- Buildroot `Config.in` 존재 확인
- Buildroot 기본 소스 구조 확인
- `DEVELOPMENT_CHECKLIST.md` 32번 완료 처리

- Git Commit: 미실행
- 다음 작업: Debian 환경 확인

## 2026-10-06 - Debian 환경 확인

- `~/rockchip/Rk3576/debian` 디렉터리 존재 확인
- Debian 빌드 스크립트 존재 확인
- `mk-rootfs.sh`, `mk-rootfs-bookworm.sh`, `mk-base-debian.sh`, `mk-image.sh` 확인
- `readme.md`에서 Debian Bookworm 및 `arm64` 환경 지원 확인
- 실제 RootFS 빌드는 수행하지 않음
- `DEVELOPMENT_CHECKLIST.md` 33번 완료 처리

- Git Commit: 미실행
- 다음 작업: SDK Build 환경 구성

## 2026-10-06 - SDK Build 환경 구성

- SDK `build.sh` 및 실제 빌드 스크립트 확인
- `check-sdk.sh` 존재 및 실행 권한 확인
- SDK 검사에서 `g++` 미설치 확인
- Ubuntu 22.04 WSL 환경에 `g++` 설치
- `check-sdk.sh` 재실행 결과 오류 없이 종료되어 SDK 환경 검사 통과
- `DEVELOPMENT_CHECKLIST.md` 34번 완료 처리

- Git Commit: 미실행
- 다음 작업: Kernel Build 테스트

## 2026-10-06 - Kernel Build 테스트

- 실제 RK3576 보드의 `/proc/device-tree/model`에서 `Rockchip RK3576 QiYang Board` 확인
- `rockchip_rk3576_qiyang_defconfig` 적용
- Kernel 6.1 / ARM64 / `rk3576-qiyang.dts` 구성으로 빌드
- 빌드 의존 패키지 보완: `bison`, `flex`, `lz4`, `python-is-python3`, `libssl-dev`, `libgmp-dev`, `libmpc-dev`, `libmpfr-dev`, `libncurses-dev`, `device-tree-compiler`
- `./build.sh kernel` 최종 성공
- Kernel `Image` 약 41MB 생성 확인
- `rk3576-qiyang.dtb` 약 288KB 생성 확인
- `output/firmware/boot.img` 생성 확인
- `DEVELOPMENT_CHECKLIST.md` 35번 완료 처리

- Git Commit: 미실행
- 다음 작업: RootFS Build 테스트

## 2026-10-06 - RootFS 빌드 시도 및 RKNN/NPU 개발 우선 전환

- `./build.sh help`에서 RootFS 빌드 옵션 확인 후 `./build.sh rootfs` 실행
- SDK 안내에 따라 live-build 소스를 내려받아 설치하고 `lb --version` 출력 `20230131` 확인
- 의존 패키지 설치: `debootstrap`, `qemu-user-static`, `binfmt-support`, `bzip2`
- binfmt 등록 중 읽기 전용 파일 시스템 경고 발생; 빌드 영향 여부는 미확인
- USTC 미러의 Bookworm Release 주소에서 HTTP/HTTPS 모두 `403 Forbidden` 확인
- `https://deb.debian.org/debian/dists/bookworm/Release`에서 `200 OK` 확인
- `debian/ubuntu-build-service/bookworm-desktop-arm64/configure`의 미러 주소 5곳을 `https://deb.debian.org`로 변경; 원본은 `.bak`으로 백업
- 미러 변경 후 RootFS 빌드를 다시 실행했으나 최종 결과는 아직 확인하지 않음
- 사용자 결정: 기존 보드 OS를 사용하여 3번 RKNN/NPU 개발을 우선 진행
- 2번의 미완료 항목인 RootFS Build 테스트, Firmware/Image Build 테스트, Build 결과 확인, 보드 Flash 테스트를 보류로 표시
- 기존 완료 항목은 유지; RootFS 빌드 성공으로 처리하지 않음
- 다음 작업: RKNN 환경 확인

## 2026-10-06 - RKNN 환경 확인

- ADB를 통해 RK3576 보드에서 `/usr/lib/librknnrt.so` 존재 확인
- `/usr/bin/rknn_server` 존재 확인
- `/sys/module/rknpu` 커널 모듈 경로 존재 확인
- `DEVELOPMENT_CHECKLIST.md`의 `RKNN 환경 확인` 완료 처리
- 다음 작업: RKNPU2 확인

## 2026-10-06 - RKNPU2 드라이버 확인

- 보드 `dmesg`에서 RKNPU 드라이버 `0.9.8` 초기화 로그 확인
- IOMMU 사용 및 성능 도메인 생성 로그 확인
- `/dev/dri`에서 `card0`, `card1`, `renderD128`, `renderD129` 장치 노드 확인
- 초기화 로그에 메모리 영역 요청 관련 경고가 있었으나 RKNPU 초기화 로그도 확인됨
- 실제 NPU 추론 실행은 아직 확인하지 않음
- `DEVELOPMENT_CHECKLIST.md`의 `RKNPU2 확인` 완료 처리
- 다음 작업: RKNN Model Zoo 확인

## 2026-10-06 - RKNN Model Zoo 확인

- `~/rockchip/rknn_model_zoo` 저장소에 README, LICENSE, `examples` 디렉터리 존재 확인
- README에서 지원 플랫폼 목록에 RK3576 기재 확인
- README의 객체 검출 목록에서 YOLO11 및 FP16/INT8 형식 기재 확인
- `DEVELOPMENT_CHECKLIST.md`의 `RKNN Model Zoo 확인` 완료 처리
- 다음 작업: RKNN 샘플 Build

## 2026-10-07 - RKNN MobileNet 샘플 Build

- RKNN Model Zoo의 `build-linux.sh`로 RK3576 / AArch64 대상 MobileNet 데모 빌드
- Arm GNU Toolchain 12.2.1을 사용해 `rknn_mobilenet_demo` 빌드 및 설치 성공
- 생성 파일을 `file` 명령으로 확인: ARM aarch64 ELF 64-bit 실행 파일
- `image_drawing.c` 및 `image_utils.c`에서 컴파일 경고가 있었으나 빌드는 완료됨
- 설치된 `model/` 폴더에 `.rknn` 모델은 없어 아직 보드 실행은 하지 않음
- `DEVELOPMENT_CHECKLIST.md`의 `RKNN 샘플 Build` 완료 처리
- 다음 작업: YOLO 샘플 Build

## 2026-10-07 - YOLO11 샘플 Build

- 실행 전 `install/rk3576_linux_aarch64`는 존재하고 `rknn_yolo11_demo` 출력 폴더는 없음을 확인
- `build-linux.sh`의 삭제 대상이 해당 데모 설치 폴더임을 확인
- 직접 실행 시 `Permission denied` 발생; 권한 변경 없이 `bash`로 실행하여 해결
- 실행 명령 (Model Zoo 루트에서):

```bash
GCC_COMPILER=/home/kim/rockchip/arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu bash ./build-linux.sh -t rk3576 -a aarch64 -d yolo11
```

- Arm GNU Toolchain 12.2.1 / Release 설정으로 빌드 및 설치 성공 (종료 코드 0)
- `file`로 일반 및 zero-copy 데모 모두 ARM aarch64 ELF 64-bit 실행 파일임을 확인
- 출력 경로: `~/rockchip/rknn_model_zoo/install/rk3576_linux_aarch64/rknn_yolo11_demo`
- `image_drawing.c`, `image_utils.c`, `postprocess.cc`에 컴파일 경고 발생; 빌드 완료와 구분하여 기록
- 설치된 `model/yolo11.rknn` 존재 확인; 모델 출처, 변환 설정 및 RK3576 호환성은 아직 미확인
- ONNX → RKNN 변환, 보드 추론 및 FPS 측정은 수행하지 않음
- 체크리스트의 `YOLO 샘플 Build`만 완료 처리
- SDK, 모델 및 빌드 산출물은 프로젝트 Git에 추가하지 않음
- 다음 작업: 기존 YOLO11 모델과 변환 환경 확인

## 2026-10-07 - YOLO11 모델 변환, RK3576 NPU 추론 및 FPS 측정

- 기존 `examples/yolo11/model/yolo11n.onnx`와 `~/rockchip/yolo11_env` 환경 확인; 기존 파일을 재다운로드하지 않음
- 로컬 README의 Rockchip 최적화 YOLO11 모델 및 9개 출력 구조와 실제 데모 입력·출력 일치 확인
- RKNN Toolkit2 2.3.2 / ONNX 1.16.1 사용
- Model Zoo의 `examples/yolo11/python`에서 아래 명령 실행:

```bash
~/rockchip/yolo11_env/bin/python convert.py ../model/yolo11n.onnx rk3576 i8 ../model/yolo11n_rk3576_i8_verified.rknn
```

- COCO subset 20 이미지 목록을 사용한 INT8 양자화 및 RKNN export 성공 (종료 코드 0)
- 양자화 outlier 및 입출력 dtype 변경 경고 발생; 정확도 평가는 별도로 수행하지 않음
- 기존 `yolo11.rknn`을 보존하고 새 모델을 사용
- ADB 서버 시작 실패는 권한 허용 후 해결; 보드 `2d6b412d4cbf7110` 연결 확인
- 보드: Rockchip RK3576 QiYang Board, Linux 6.1.141 aarch64
- 새 배포 경로: `/userdata/ai_bsd_yolo11_verified`
- 보드 실행 명령 (배포 폴더에서):

```bash
LD_LIBRARY_PATH=./lib ./rknn_yolo11_demo model/yolo11n_rk3576_i8_verified.rknn model/bus.jpg
```

- 입력 NHWC 1×640×640×3, INT8 출력 9개 확인
- NPU 추론 성공: bus 0.944, person 0.898 / 0.835 / 0.831 / 0.452
- 생성된 `out.png`를 로컬로 가져와 버스·사람 검출 박스와 라벨을 직접 확인
- Runtime 2.3.2 (429f97ae6b), RKNPU 드라이버 0.9.8 확인
- 기존 초기화 메모리 경고 기록은 유지; 이번에는 실제 모델 초기화·추론·검출 성공을 확인함

### FPS 측정 방법 및 결과

- `tools/yolo11_benchmark.cc` 및 `tools/build_yolo11_benchmark.sh` 추가
- 기존 Model Zoo 일반 데모의 구현과 빌드 객체를 사용 (zero-copy 데모 측정 아님)
- 모델 로드·이미지 읽기 1회 후 준비 실행 10회, 100회씩 3회 측정
- 단조 시계로 전처리, 입력 전달, NPU 실행, 출력 회수, 후처리 및 버퍼 해제 시간을 측정
- 반복 로그 출력 억제; 카메라 입력, 이미지 디코딩, 모델 로딩, 박스 그리기, 파일 저장, 화면 출력 제외
- 매회 추론 반환 코드와 비어 있지 않은 검출 결과 확인; 총 300회 성공, 마지막 검출 수 매회차 5개

| 회차 | 프레임 수 | 전체 시간 (ms) | 평균 (ms/frame) | FPS |
| --- | --- | --- | --- | --- |
| 1 | 100 | 2322.605 | 23.226 | 43.055 |
| 2 | 100 | 2814.405 | 28.144 | 35.531 |
| 3 | 100 | 2827.427 | 28.274 | 35.368 |

- 합계 300프레임 / 7964.437ms = 약 37.67 FPS, 평균 약 26.55ms/frame
- 기본 보드 설정에서 같은 `bus.jpg` 반복 처리; 주파수·온도는 고정하거나 기록하지 않았으므로 회차별 차이 원인은 미확인
- 카메라 실시간 FPS나 순수 NPU 실행 시간으로 해석하지 않음; 장시간 안정성 및 정량 정확도 평가는 미실시
- 재현: WSL에서 `bash tools/build_yolo11_benchmark.sh` 실행 후 생성된 프로그램을 배포 폴더에 전송
- 보드 측정 명령:

```bash
LD_LIBRARY_PATH=./lib ./yolo11_benchmark model/yolo11n_rk3576_i8_verified.rknn model/bus.jpg
```

- `.yolo11_run/`, `*.onnx`, `*.rknn`을 Git 제외 규칙에 추가; 모델·SDK·바이너리·결과 이미지는 Git에 추가하지 않음
- 3번 RKNN / NPU의 나머지 항목을 실제 확인 결과에 따라 완료 처리; 7번 성능 최적화의 FPS 항목은 유지
- 다음 작업: 4번 Camera Interface 확인 및 카메라 입력 파이프라인 구성
