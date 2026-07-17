# Mini RTOS Task Scheduler

C++로 RTOS의 핵심 개념인 우선순위 기반 Task 스케줄링을 간단히 구현해보는 학습 프로젝트입니다.

<br>

## 프로젝트 목적

RTOS 스케줄러의 핵심 기능인 우선순위에 따라 실행 순서를 결정하는 로직을 직접 구현하며 시스템 프로그래밍 기초를 다지는 것을 목표로 합니다.

<br>

## 진행 상황

- [x] 1단계: Task 구조체 설계 + Task 등록 기능 + 우선순위 기준 정렬
- [x] 2단계: 등록된 Task를 한 번 정렬 후 그 순서대로 실행
- [x] 3단계: 정렬 1회 방식 → 매번 재판단하는 스케줄러 루프로 구조 변경 (매 실행 전 아직 안 끝난 Task 중 최고 우선순위를 다시 탐색). 실행 도중 새 Task 등록 같은 동적 상황에 대응하기 위한 사전 작업
- [x] 4단계: 타이머(tick) 기반 자동 스케줄링 도입. TIME_SLICE(타임 슬라이스) 개념 적용, 동일 우선순위 Task 간 Round-robin, 더 급한 Task 등장 시 즉시 선점(preemption) 및 재개(resume) 구현
- [ ] 5단계: ucontext.h 기반 실제 context switch 구현

<br>

## 사용기술

- C++ (STL: vector, algorithm)
- 함수 포인터를 이용한 Task 등록 구조

<br>

## 구현 세부 사항

- registerTask(): Task를 구조체로 만들어 리스트에 등록
- remainingTicks: 각 Task가 완료까지 필요한 tick 수. 매 tick 1씩 감소, 0이 되면 완료
- rrPointer: 동일 우선순위 Task들 사이에서 공평하게 순서를 배분하는 라운드로빈 포인터
- minPriorityAmongReady(): 아직 안 끝난 Task 중 가장 급한(가장 낮은) 우선순위 값을 찾는 함수
- findNext(): 최고 우선순위를 찾고, 동점이면 rrPointer 기준 라운드로빈으로 후보 선택
- TIME_SLICE: 한 번 뽑힌 Task가 최대 몇 tick 연속 실행될지 정하는 상수
- runScheduler(): 매 tick마다 더 급한 Task가 새로 나타났는지 재확인하여 TIME_SLICE가 남아있어도 즉시 선점(preemption)되도록 구현. 선점된 Task는 나중에 남은 tick부터 이어서 실행됨

<br>

## 실행 예시

### 1단계: Task 구조체 설계 + Task 등록 기능 + 우선순위 기준 정렬

```
[등록] SensorRead (우선순위: 2)
[등록] EmergencyStop (우선순위: 0)
[등록] LogWriter (우선순위: 5)

--- 우선순위 순으로 정렬 ---

EmergencyStop (우선순위: 0)
SensorRead (우선순위: 2)
LogWriter (우선순위: 5)
```

<br>

### 2단계: 등록된 Task를 한 번 정렬 후 그 순서대로 실행

```
[등록] SensorRead (우선순위: 2)
[등록] EmergencyStop (우선순위: 0)
[등록] LogWriter (우선순위: 5)

--- 스케줄러 실행 시작 ---

[실행] EmergencyStop
>> Task B: 비상 정지 처리 중...
[실행] SensorRead
>> Task A: 센서 값을 읽는 중...
[실행] LogWriter
>> Task C: 로그를 기록하는 중...

--- 모든 Task 실행 완료 ---
```

<br>

### 3단계: 정렬 1회 방식 → 매번 재판단하는 스케줄러 루프로 구조 변경

```
[등록] SensorRead (우선순위: 2)
[등록] EmergencyStop (우선순위: 0)
[등록] LogWriter (우선순위: 5)

--- 스케줄러 실행 시작 ---

[실행] EmergencyStop
>> Task B: 비상 정지 처리 중...

[!] 실행 도중 긴급 이벤트 발생 - 새 Task 등록

[등록] FireDetected (우선순위: -1)
[실행] FireDetected
>> Task Urgent: 화재 감지! 즉시 대응 중...
[실행] SensorRead
>> Task A: 센서 값을 읽는 중...
[실행] LogWriter
>> Task C: 로그를 기록하는 중...

--- 모든 Task 실행 완료 ---
```

<br>

### 4단계: 타이머(tick) + TIME_SLICE + 즉시 선점/재개 + 동일 우선순위 Round-robin

```
[등록] SensorRead (우선순위: 2, 필요 tick: 2)
[등록] EmergencyStop (우선순위: 0, 필요 tick: 3)
[등록] LogWriter (우선순위: 5, 필요 tick: 2)
[등록] TaskD (우선순위: 3, 필요 tick: 2)
[등록] TaskE (우선순위: 3, 필요 tick: 2)

--- 스케줄러 실행 시작 ---

[tick 1] 실행: EmergencyStop (남은 tick: 3 -> 2)

[!] 실행 도중 긴급 이벤트 발생 - 새 Task 등록

[등록] FireDetected (우선순위: -1, 필요 tick: 2)
[선점!] 더 급한 Task 발견 - 슬라이스 조기 종료

[tick 2] 실행: FireDetected (남은 tick: 2 -> 1)
[tick 3] 실행: FireDetected (남은 tick: 1 -> 0)
[tick 4] 실행: EmergencyStop (남은 tick: 2 -> 1)
[tick 5] 실행: EmergencyStop (남은 tick: 1 -> 0)
[tick 6] 실행: SensorRead (남은 tick: 2 -> 1)
[tick 7] 실행: SensorRead (남은 tick: 1 -> 0)
[tick 8] 실행: TaskD (남은 tick: 2 -> 1)
[tick 9] 실행: TaskD (남은 tick: 1 -> 0)
[tick 10] 실행: TaskE (남은 tick: 2 -> 1)
[tick 11] 실행: TaskE (남은 tick: 1 -> 0)
[tick 12] 실행: LogWriter (남은 tick: 2 -> 1)
[tick 13] 실행: LogWriter (남은 tick: 1 -> 0)

--- 모든 Task 완료 (총 13 tick 소요) ---
```

핵심 동작

- EmergencyStop이 TIME_SLICE를 다 안 썼는데도, 더 급한 FireDetected가 등록되자마자 바로 끼어듦
- FireDetected가 다 끝나자 EmergencyStop이 처음부터가 아니라 남은 tick(1개)부터 이어서 실행됨
- Round-robin: 우선순위가 동일한 TaskD, TaskE 중 TaskD가 먼저 선택됨 (rrPointer 기준 공평한 순서 배분)
