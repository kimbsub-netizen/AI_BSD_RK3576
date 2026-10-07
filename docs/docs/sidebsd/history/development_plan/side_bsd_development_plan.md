# 측면 사각지대 감지 시스템 (Side BSD) 개발 계획서

## 1. 프로젝트 개요
* **프로젝트명:** 딥러닝 비전 기반 측면 사각지대 감지 시스템 (Vision-based Side Blind Spot Detection)
* **개발 목적:** 대형 상용차(화물차, 버스, 지게차 등)의 측면/후면 사각지대에서 발생하는 충돌 사고를 예방하기 위한 Standalone(독립형) AI 안전 장비 구축.
* **타겟 하드웨어:** Edge AI NPU 보드 (초기 개발: PC 기반 Python -> 최종 양산: Rockchip RK3576 등 Edge 장비)
* **주요 특징:** 차량 CAN 통신(속도/조향) 연동 없이 오직 **비전(카메라 영상)만으로** 절대 공간을 방어하고 상대 속도를 추정하는 Cost-effective 알고리즘 구현.

---

## 2. 핵심 개발 목표 (Key Objectives)
1. **단일 NPU 다중 카메라 처리:** 1대의 엣지 보드에서 좌측/우측 2개 이상의 카메라 영상을 병렬 추론하여 비용 최적화.
2. **커스텀 ROI 기반 공간 방어:** 차체 반사광과 인접 차선 너머의 객체를 무시하는 정밀 다각형(Polygon) 형태의 DANGER/WARNING 구역 설정 기능.
3. **O2O 캘리브레이션 파이프라인:** 비선형 곡선 피팅(Curve Fitting)을 통해 2D 픽셀을 3D 절대 거리(Meter)로 정밀 변환, 현장 설치 및 튜닝의 자동화/간소화.
4. **스마트 오작동 필터링 알고리즘:** 객체 추적(ByteTrack)을 통해 마주 오는 차량 및 서행 시 주차된 차량에 대한 불필요한 알람 억제.

---

## 3. 기술 스택 및 아키텍처
* **객체 인식 (Detection):** YOLOv8 (차량, 사람, 오토바이 등 타임-크리티컬 객체 중심)
* **객체 추적 (Tracking):** ByteTrack 알고리즘 (가려짐 및 부분 인식 보완)
* **영상 처리 (Vision):** OpenCV (디워핑, ROI 렌더링, 스트림 병렬 처리)
* **개발 언어:** Python 3.x
* **최종 배포 포맷:** RKNN (Rockchip NPU 모델 퀀타이제이션 변환) + Headless 배포

---

## 4. 단계별 개발 일정 및 마일스톤 (WBS)

### Phase 1: 핵심 알고리즘 및 PoC 구현 (완료)
* YOLOv8 모델 도입 및 차량 클래스(`VEHICLE_CLASS_IDS`) 필터링 로직 구현
* 좌/우 독립형 스레드 또는 라운드 로빈 카메라 스케줄링 구조(`SideCameraScheduler`) 설계
* 다각형 ROI(`DANGER_ZONE`, `WARNING_ZONE`) 및 `cv2.pointPolygonTest` 기반 포함 판정 로직 개발

### Phase 2: 연구소용 오프라인 세팅 툴(Toolchain) 확보 (완료)
* `roi_tuner_v2.py`: 카메라 장착 화각에 맞춘 정밀 ROI 픽셀 좌표 추출용 GUI 툴 구현
* `distance_calibrator_tool.py`: 1, 2, 4, 6, 10m 실측 기반 비선형 거리 산출(보간 방정식) 툴 개발
* `multi_camera_recorder.py`: 실차 현장 주행 시 다중 AHD 카메라 동시 녹화 툴(멀티스레딩) 개발

### Phase 3: ADAS 스마트 필터링 고도화 (완료)
* ByteTrack을 이용한 객체 ID 부여 부여 및 프레임 간 연속성 확보
* `VelocityTracker` 모듈 결합으로 화면 내 객체의 '상대 속도(m/s)' 도출
* Mute 기능 도입: `-5.0m/s` 등의 임계값(`IGNORE_AWAY_VELOCITY_MS`)을 두어 마주 오는 차 및 고속 주행 시 주차된 차량 무시 로직 적용

### Phase 4: 실차 주행 테스트 및 데이터 수집 (현재 단계)
* `multi_camera_recorder.py`를 활용한 차종별/화각별 원본(Raw) 운행 영상 수집
* 수집된 영상을 랩실(주피터 노트북)에서 재생하며 거리(3m/10m) 및 속도 임계값 황금 비율(Golden Ratio) 튜닝
* HUD 상단에 실시간 처리량(FPS) 및 추적 객체 파라미터 표출 확인

### Phase 5: Edge NPU 보드 포팅 및 양산화 (Next Step)
* 파이썬 의존성 최소화 및 `SHOW_VIDEO = False` (Headless 모드) 구동 안정성 검증
* YOLOv8 모델을 타겟 보드용 NPU 포맷(RKNN 등)으로 양자화(Quantization) 변환
* 부팅 시 자동 실행 데몬 연동 및 시리얼 통신을 통한 외부 물리(부저/LED) 알람 신호 연동
* 최종 양산 검수 및 매뉴얼 배포

---

## 5. 품질 검증 (QA) 방안
* **False Positive (오탐) 테스트:** 인접 차선 너머의 고속 주행 차량, 내 차체에 맺힌 반사광, 차선 분리대 등의 비정상 알람 발생 여부 체크.
* **부분 검출 한계 테스트:** 0~1m 초근접 환경에서 잘린 형태의 차량에 대한 ByteTrack 추적 유지력 점검.
* **프레임 방어(FPS) 체크:** NPU 1대로 2채널(좌/우) 인퍼런스 시 시스템 마지노선(총 15~20fps) 방어 여부 프로파일링.
* **장기 구동(Endurance):** 데몬 프로세스로 72시간 연속 구동 시 메모리 누수 발생 여부 확인.
