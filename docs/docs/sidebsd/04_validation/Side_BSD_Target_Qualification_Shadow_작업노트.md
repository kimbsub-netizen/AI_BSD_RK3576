# Side BSD Target Qualification Shadow 작업노트

> 상태: WORKING NOTE  
> 목적: 마주오는 차량과 실제 후방 접근 차량의 Track History 기반 분류 가능성 검증  
> 적용 범위: Forward Side BSD / Side Camera  
> 주의: 현재 단계에서는 실제 WARNING/DANGER 경고를 억제하지 않는다.

---

## 1. 왜 이 기능을 검토하는가

현재 Forward Side BSD는 ROI와 Distance를 독립적인 안전 판단층으로 사용하고, TTC는 WARNING을 DANGER로 올리는 escalation 용도로만 사용한다.

이 구조는 실제 위험차량의 미경보를 줄이는 데 유리하지만, 반대편에서 마주오는 차량이 Side Camera 영상에 갑자기 나타나 ROI에 들어올 경우 불필요한 WARNING/DANGER 후보가 될 수 있다.

과거에는 순간 상대속도나 Fast-Path 계열 파라미터로 이를 구분하려 했으나, 현재 요구사항에서는 velocity/TTC로 기존 기본경고를 SAFE로 downgrade하지 않는다.

따라서 새로운 방향은 다음과 같다.

```text
"속도가 몇 m/s인가?" 보다는
"이 차량이 ROI에 들어오기 전부터 추적되던 차량인가?"를 본다.
```

---

## 2. 핵심 가설

### 2.1 뒤에서 접근하는 실제 Side BSD 대상 차량

일반적으로 다음 패턴이 예상된다.

```text
Side Camera 화면에 먼저 등장
→ ROI 밖에서 tracker ID 유지
→ WARNING / DANGER ROI로 진입
→ PRETRACKED
```

즉 ROI에 들어오기 전부터 Track History가 존재한다.

### 2.2 반대편에서 마주오는 차량

카메라 설치 각도와 FOV에 따라 다음 패턴이 예상된다.

```text
이전 프레임에는 없거나 추적 이력 없음
→ 사이드미러/전방 쪽에서 ROI 내부에 갑자기 등장
→ 짧은 시간 안에 화면을 통과하며 멀어지는 방향으로 이동
→ NEW_INSIDE_RECEDING 후보
```

중요한 점은 이것이 아직 "확정 규칙"이 아니라 실제 영상으로 확인해야 하는 가설이라는 것이다.

---

## 3. Shadow 기능의 의미

Shadow 기능은 분류는 수행하지만 실제 경고에는 반영하지 않는 진단 기능이다.

예:

```text
기존 Side BSD 판단 : WARNING
Shadow 분류         : NEW_INSIDE_RECEDING
실제 사용자 출력   : WARNING 유지
```

즉 현재 단계에서 `NEW_INSIDE_RECEDING`이 나왔다고 경고를 SAFE로 바꾸거나 객체를 무시하면 안 된다.

Shadow 단계의 목적은 실제 영상/실차 데이터에서 분류가 믿을 만한지 확인하는 것이다.

---

## 4. 상태 정의

Target Qualification은 최소 다음 상태를 사용한다.

```text
OUTSIDE
PRETRACKED
NEW_INSIDE_PENDING
NEW_INSIDE_APPROACHING
NEW_INSIDE_RECEDING
NEW_INSIDE_AMBIGUOUS
```

### OUTSIDE
현재 Side BSD ROI 밖에 있는 차량.

### PRETRACKED
ROI 진입 전에 일정 프레임 이상 이미 추적된 차량.

정상적인 후방 접근 차량에서 주로 기대하는 상태다.

### NEW_INSIDE_PENDING
ROI 내부에서 처음 tracker가 생성되었고 아직 이동 방향을 판단하기 위한 프레임이 부족한 상태.

### NEW_INSIDE_APPROACHING
ROI 내부에서 처음 검출됐지만 이후 가까워지는 방향의 궤적이 확인된 상태.

실제 급접근 차량, 끼어드는 차량, 가림 이후 늦게 검출된 실제 위험차일 수 있으므로 단순 억제 대상이 되어서는 안 된다.

### NEW_INSIDE_RECEDING
ROI 내부에서 처음 등장하고 이후 멀어지는 방향의 짧은 궤적이 확인된 상태.

현재 가설상 "마주오는 차량 후보"다.

### NEW_INSIDE_AMBIGUOUS
짧은 Track History만으로 approaching/receding 판단이 명확하지 않은 상태.

---

## 5. 로그 해석에서 가장 중요한 기준

### 정상으로 기대하는 경우

```text
마주오는 차량
→ NEW_INSIDE_RECEDING
```

이 경우 의도한 분류에 가깝다.

```text
뒤에서 정상적으로 접근하는 차량
→ PRETRACKED
```

이 경우 정상이다.

```text
뒤에서 오는 차량이 가림/검출 누락 때문에 ROI 안에서 처음 검출
→ NEW_INSIDE_APPROACHING
```

이 경우도 정상 가능하다.

### 문제로 봐야 하는 경우

```text
뒤에서 접근하는 실제 Side BSD 대상 차량
→ NEW_INSIDE_RECEDING
```

이 결과가 나오면 오분류다.

이 상태에서는 `NEW_INSIDE_RECEDING`을 실제 경고 억제 조건으로 사용하면 안 된다.

---

## 6. 기억할 한 줄

> **마주오는 차가 `NEW_INSIDE_RECEDING`이면 정상 후보. 뒤에서 오는 차가 `NEW_INSIDE_RECEDING`이면 문제.**

---

## 7. Tracker ID (`tid`) 의미

로그 예:

```text
[QUAL] LEFT tid=31 class=NEW_INSIDE_RECEDING
```

여기서 `tid=31`은 실제 차량 번호판 번호가 아니다.

`tid`는 ByteTrack 같은 객체 추적기가 영상 세션 안에서 동일 차량을 프레임 간 이어서 보기 위해 임시로 부여한 Tracker ID다.

예:

```text
Frame 100 → 차량 #31
Frame 101 → 차량 #31
Frame 102 → 차량 #31
```

가림이나 재검출 때문에 같은 실제 차량도 나중에 다른 `tid`를 받을 수 있다.

---

## 8. 영상 준비 시 주의사항

뒤 차량 영상은 차량이 이미 ROI 안에 들어온 시점부터 잘라서 사용하면 안 된다.

가능하면 다음 구간을 모두 포함해야 한다.

```text
ROI 밖에서 차량이 보이기 시작
→ 추적 유지
→ ROI 진입
→ 통과
```

권장:

```text
ROI 진입 전 약 1~2초 이상 포함
```

마주오는 차량 영상도 다음 흐름이 연속으로 포함되는 것이 좋다.

```text
차량 없음
→ 사이드미러/전방 쪽에서 등장
→ ROI 통과
→ 사라짐
```

두 시나리오 모두 동일한 Side Camera 영상으로 비교해도 문제없다. 오히려 같은 카메라/렌즈/FOV/장착각도 조건에서 비교하는 것이 의미가 있다.

---

## 9. 초기 판단 방식

현재 Side Camera 거리 구조에서는 일반적으로 foot point의 normalized Y가 커질수록 화면 아래쪽, 즉 가까운 영역으로 해석된다.

개념적으로:

```text
delta_y = current_y_norm - entry_y_norm
```

```text
delta_y > +threshold
→ 가까워지는 방향
→ NEW_INSIDE_APPROACHING 후보


delta_y < -threshold
→ 멀어지는 방향
→ NEW_INSIDE_RECEDING 후보
```

단, threshold 값은 실제 녹화영상/실차 데이터로 조정해야 하며 현재 단계에서 제품 성능 기준으로 확정하면 안 된다.

---

## 10. 안전 원칙

다음과 같은 단순 규칙은 사용하면 안 된다.

```text
새 tracker = 마주오는 차량
새 tracker = 무시
NEW_INSIDE_RECEDING = 무조건 SAFE
```

실제 위험차도 다음 이유로 ROI 안에서 처음 잡힐 수 있다.

- 차량 가림
- YOLO 일시 검출 누락
- ByteTrack ID 재생성
- 급차선변경
- 카메라 FOV에 늦게 등장

따라서 실제 경고 억제 단계로 넘어가려면 여러 조건과 실제 데이터 검증이 필요하다.

특히 same-side Turn Signal이 활성화된 상황에서는 더 보수적인 fail-safe 정책을 유지해야 한다.

---

## 11. 기존 Forward Side BSD 정책과의 관계

Target Qualification Shadow는 아래 기존 정책을 변경하지 않는다.

```text
DANGER ROI + Distance 독립 판단
0~10m DANGER
10~30m WARNING
TTC <= 2.5s : WARNING → DANGER escalation only
same-side Turn Signal : WARNING → DANGER escalation
Fault/Unavailable fail-safe
```

Reverse Assist에는 적용하지 않는다.

---

## 12. 실제 검증에서 확인할 항목

최소 다음 시나리오를 반복 수집한다.

| 시나리오 | 기대 Shadow 결과 | 실제 경고 |
|---|---|---|
| 뒤에서 정상 접근 | PRETRACKED | 기존 Side BSD 그대로 |
| 뒤 차량이 ROI 안에서 늦게 검출 | NEW_INSIDE_APPROACHING 또는 AMBIGUOUS | 기존 Side BSD 그대로 |
| 마주오는 차량이 ROI에 갑자기 등장 | NEW_INSIDE_RECEDING 기대 | 기존 Side BSD 그대로 |
| 마주오는 차량인데 PRETRACKED | 원인 분석 필요 | 기존 Side BSD 그대로 |
| 뒤에서 오는 차량인데 NEW_INSIDE_RECEDING | 위험한 오분류, 반드시 분석 | 기존 Side BSD 그대로 |

---

## 13. Production 경고 억제로 승격하기 위한 조건

다음이 실제 데이터에서 충분히 확인되기 전에는 경고 억제를 적용하지 않는다.

1. 마주오는 차량의 `NEW_INSIDE_RECEDING` 분류율이 안정적일 것.
2. 뒤에서 접근하는 실제 위험차량이 `NEW_INSIDE_RECEDING`으로 잘못 분류되는 사례가 사실상 없을 것.
3. 급차선변경, 가림 후 등장, tracker ID 재생성 시나리오에서 미경보 위험이 없을 것.
4. 좌/우 카메라 각각에서 검증할 것.
5. 카메라 장착각도/FOV/Flip 변경 시 재검증할 것.
6. Turn Signal 활성 상태에서 fail-safe가 유지될 것.

검증 후에도 실제 억제 기능은 별도 요구사항 변경 및 회귀시험 대상으로 다룬다.

---

## 14. 작업 중 메모

실영상 검증 중 발견한 내용을 아래에 계속 추가한다.

### LEFT

- TBD

### RIGHT

- TBD

### 오분류 사례

- TBD

### Threshold 조정 기록

- TBD

### 결론

- TBD
