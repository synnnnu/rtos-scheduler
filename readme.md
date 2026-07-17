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
- [ ] 4단계: 타이머 기반 자동 스케줄링 (tick 개념 도입)
- [ ] 5단계: ucontext.h 기반 실제 context switch 구현

<br>

## 사용기술

- C++ (STL: vector, algorithm)
- 함수 포인터를 이용한 Task 등록 구조

<br>

## 구현 세부 사항

- registerTask(): Task를 구조체로 만들어 리스트에 등록
- runScheduler(): 매 실행 전마다 아직 안 끝난 Task 중 최고 우선순위를 다시 탐색하는 방식
- 실행 도중 새로운 Task가 등록되어도 즉시 우선순위에 반영됨을 확인

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
