# RK3576 AI BSD 개발 체크리스트

## 0. 개발 환경 / Git

- [x] RK3576 SDK 및 개발 파일 확보
- [x] Windows + WSL 개발 환경 구성
- [x] Git 저장소 생성
- [x] Git Remote 연결
- [x] Git Push 확인
- [x] Git 인증 자동화
- [x] 프로젝트 디렉터리 정리

## 1. RK3576 보드 기본 동작

- [x] RKDevTool 설치
- [x] RK USB Driver 설치
- [x] RKDevTool에서 LOADER Device 인식
- [x] 보드 전원 및 USB 연결 확인
- [~] HDMI 모니터 연결
- [ ] 기본 Firmware 다운로드
- [x] 보드 정상 부팅 확인
- [x] ADB 연결 확인
- [ ] Serial Console 연결
- [ ] SSH 연결 확인

## 2. RK3576 Linux / SDK

- [x] RK3576 SDK 압축 해제 및 구조 확인
- [x] Cross Compiler 설치
- [x] Cross Compiler 동작 확인
- [x] Kernel 소스 확인
- [x] Buildroot 환경 확인
- [x] Debian 환경 확인
- [x] SDK Build 환경 구성
- [x] Kernel Build 테스트
- [ ] RootFS Build 테스트 — 보류 (기존 보드 OS로 RKNN/NPU 개발 우선)
- [ ] Firmware/Image Build 테스트 — 보류 (기존 보드 OS로 RKNN/NPU 개발 우선)
- [ ] Build 결과 확인 — 보류 (기존 보드 OS로 RKNN/NPU 개발 우선)
- [ ] 보드 Flash 테스트 — 보류 (기존 보드 OS로 RKNN/NPU 개발 우선)

## 3. RKNN / NPU

- [x] RKNN 환경 확인
- [x] RKNPU2 확인
- [x] RKNN Model Zoo 확인
- [x] RKNN 샘플 Build
- [x] YOLO 샘플 Build
- [ ] YOLO 모델 준비
- [ ] ONNX → RKNN 변환
- [ ] RK3576 NPU에서 YOLO 실행
- [ ] NPU 실행 결과 확인
- [ ] FPS 측정

## 4. Camera

- [ ] Camera Interface 확인
- [ ] Camera 연결
- [ ] Camera 영상 입력 확인
- [ ] 영상 출력 확인
- [ ] 해상도 설정
- [ ] FPS 설정
- [ ] Camera 장시간 동작 확인

## 5. ROI / YOLO

- [ ] ROI 개념 및 좌표 정의
- [ ] ROI 영역 설정
- [ ] ROI 영상 출력
- [ ] ROI 내 YOLO 실행
- [ ] Object Detection 좌표 확인
- [ ] Object Class 확인
- [ ] Bounding Box 처리
- [ ] ROI + YOLO 통합

## 6. AI BSD 기능

- [ ] BSD Detection 조건 정의
- [ ] Detection Target 정의
- [ ] Left Camera 처리
- [ ] Right Camera 처리
- [ ] 위험 영역 ROI 정의
- [ ] Object 위치 판단
- [ ] Object 접근 판단
- [ ] 위험 상태 판단
- [ ] Warning Logic 구현
- [ ] 실시간 처리 확인

## 7. 성능 최적화

- [ ] NPU 사용률 확인
- [ ] CPU 사용률 확인
- [ ] Memory 사용량 확인
- [ ] FPS 측정
- [ ] Processing Latency 측정
- [ ] YOLO 모델 최적화
- [ ] ROI 처리 최적화
- [ ] Camera Pipeline 최적화
- [ ] 장시간 안정성 테스트

## 8. 최종 프로그램

- [ ] 프로그램 구조 정리
- [ ] Configuration 파일 구성
- [ ] Error Handling
- [ ] Logging
- [ ] Boot 시 자동 실행
- [ ] Final Build
- [ ] Standalone Board 실행
- [ ] 최종 기능 테스트
- [ ] 최종 성능 테스트
- [ ] Git Repository 정리
- [ ] Final Commit / Push
