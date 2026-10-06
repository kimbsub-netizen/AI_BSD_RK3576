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
