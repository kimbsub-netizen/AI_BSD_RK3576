# Side BSD 보드 성능 BENCHMARK 판정기준

> Status: CURRENT
> Applies to: Side BSD Board Validation
> Updated: 2026-09-14

## 1. 목적

실제 Rockchip 보드에서 Side BSD의 YOLO 추론 성능과 좌/우 Side 처리 파이프라인 성능을 동일한 기준으로 측정하고 판정하기 위한 기준을 정의한다.

BENCHMARK 결과는 보드 성능 Validation 근거이며, 자동 unittest PASS나 PC 실행 성능을 대체하지 않는다.

## 2. 실행 설정

`src/apps/side_bsd/config/config_runtime.py`에서 다음과 같이 설정한다.

```python
SESSION_PROFILE = "BENCHMARK"
```

BENCHMARK profile은 preview, recording, annotation을 끄고 성능 모니터만 활성화하여 표시/저장 오버헤드를 최소화한다.

현재 Side 목표 설정은 다음 값을 기준으로 한다.

```python
INFERENCE_FPS_PER_SIDE = 30
PERF_MIN_FPS_RATIO = 0.95
PERF_MONITOR_WINDOW_SEC = 10.0
```

따라서 Side 1회 처리 시간 예산은 다음과 같다.

```text
1000 ms / 30 fps = 33.3 ms
```

최소 완료 FPS 기준은 다음과 같다.

```text
30 fps × 0.95 = 28.5 fps
```

설정값이 변경되면 33.3 ms와 28.5 fps를 고정값으로 재사용하지 말고 현재 설정값에서 다시 계산한다.

## 3. 로그 항목 정의

### `[YOLO BENCH]`

예:

```text
[YOLO BENCH 10.0s] | calls=300 | CALL avg=14.2ms p95=15.8ms | CORE avg=10.7ms p95=11.9ms samples=300
```

- `CALL`: `model.predict()` 전체 wall-clock 시간. 프레임이 `model.predict()`에 들어가서 결과가 반환될 때까지의 실제 호출 시간이다.
- `CORE`: active backend가 `result.speed["inference"]`로 제공하는 inference 시간이다.
- `CORE`는 현재 API에서 얻을 수 있는 backend inference 지표이며, raw NPU kernel profiler 측정값과 동일하다고 간주하지 않는다.
- `avg`: 측정 window 내 산술 평균.
- `p95`: 측정값의 95 percentile.

### `[PERF]`

예:

```text
[PERF 10.0s] | LEFT PIPELINE: avg=22.4ms, p95=27.5ms, completed=29.8fps, target=30.0fps, PASS | RIGHT PIPELINE: avg=22.9ms, p95=28.1ms, completed=29.7fps, target=30.0fps, PASS | LOOP: p95=33.0ms, fps=30.0 | PERF_RESULT: PASS
```

- `LEFT PIPELINE` / `RIGHT PIPELINE`: 각 Side의 `cam.infer()` 전체 처리시간. YOLO 호출뿐 아니라 Detection 변환, ByteTrack, ROI, 거리/TTC/경고 판단 등 Side 처리 경로를 포함한다.
- `completed`: 해당 window 동안 실제 완료된 Side inference FPS.
- `LOOP`: 메인 처리 loop의 p95 시간과 loop FPS.
- `PERF_RESULT`: 현재 runtime performance monitor의 종합 판정.

## 4. 판정 기준

현재 `INFERENCE_FPS_PER_SIDE = 30` 기준의 1차 진단 목표는 다음과 같다.

| 항목 | 기준 | 의미 |
|---|---:|---|
| YOLO CORE avg | <= 33.3 ms | backend inference 평균이 30 fps 예산 이내 |
| YOLO CALL avg | <= 33.3 ms | 실제 `model.predict()` 호출 평균이 예산 이내 |
| LEFT PIPELINE p95 | <= 33.3 ms | 좌측 Side 전체 처리의 95%가 예산 이내 |
| RIGHT PIPELINE p95 | <= 33.3 ms | 우측 Side 전체 처리의 95%가 예산 이내 |
| LEFT completed | >= 28.5 fps | 목표 30 fps의 95% 이상 완료 |
| RIGHT completed | >= 28.5 fps | 목표 30 fps의 95% 이상 완료 |

최종 Side 성능 PASS는 YOLO 평균시간 하나만으로 판정하지 않는다. 최소한 좌/우 `PIPELINE p95`와 좌/우 `completed fps`를 함께 만족해야 한다.

현재 코드의 `PERF_RESULT` 역시 Side 채널에 대해 다음 두 조건을 함께 사용한다.

```text
completed_fps >= target_fps × PERF_MIN_FPS_RATIO
p95_ms <= 1000 / target_fps
```

## 5. 해석 예

### PASS 예

```text
YOLO CORE avg = 18 ms
YOLO CALL avg = 21 ms
LEFT PIPELINE p95 = 27 ms, completed = 29.8 fps
RIGHT PIPELINE p95 = 28 ms, completed = 29.7 fps
```

YOLO와 Side 전체 처리 모두 30 fps 예산을 만족한다.

### FAIL 예

```text
YOLO CORE avg = 25 ms
YOLO CALL avg = 27 ms
LEFT PIPELINE p95 = 38 ms, completed = 24 fps
RIGHT PIPELINE p95 = 37 ms, completed = 24 fps
```

YOLO 자체 평균은 33.3 ms 이하이지만 Side 전체 pipeline이 목표를 만족하지 못하므로 최종 성능은 FAIL이다.

## 6. Board Validation 기록 시 보존할 항목

실제 보드 시험 결과에는 최소 다음을 같이 기록한다.

- 시험 baseline commit SHA
- 보드 모델 / SoC / NPU runtime 버전
- 사용 모델 및 모델 revision
- `OPERATION_MODE`, `SESSION_PROFILE`
- `INFERENCE_FPS_PER_SIDE`, `PERF_MIN_FPS_RATIO`, `PERF_MONITOR_WINDOW_SEC`
- 입력 해상도 및 `INPUT_RESIZE`
- `[YOLO BENCH]` 로그
- `[PERF]` 로그
- 시험 지속시간
- 발열/스로틀링 여부
- 판정 PASS/FAIL

짧은 초기 구간 하나만으로 최종 보드 성능을 확정하지 않는다. NPU warm-up 이후의 여러 window를 확보하고, 장시간 수행 시 thermal throttling 여부도 별도로 확인한다.

## 7. 범위 한계

본 BENCHMARK PASS는 실제 보드에서의 처리 성능 기준 충족을 의미한다.

다음을 자동으로 의미하지 않는다.

- 검출 정확도 PASS
- 거리/TTC 정확도 PASS
- 실제 차량 시나리오 Validation PASS
- ISO 17387 전체 적합성
- raw NPU kernel latency 인증

해당 항목은 별도의 Lab/Board/Vehicle Validation evidence로 관리한다.
