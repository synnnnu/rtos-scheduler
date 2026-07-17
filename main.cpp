#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Task 하나를 표현하는 구조체
struct Task {
    string name;    // 디버깅용 이름
    void (*func)(); // 이 Task가 실행할 함수 (함수 포인터)
    int priority;   // 숫자가 낮을수록 우선순위가 높다고 가정
    int remainingTicks; // 완료까지 남은 tick 수
};

// 모든 Task를 담아둘 리스트
vector<Task> taskList;

// 라운드로빈용 포인터: 다음에 후보를 찾을 때 어디서부터 훑어볼지 기억해두는 변수
// 매번 리스트 처음부터 찾지 않고, 지난번 실행한 다음 자리부터 찾게 됨
int rrPointer = 0;  // 라운드로빈 탐색 시작 위치

// Task를 등록하는 함수
void registerTask(string name, void (*func)(), int priority, int ticks) {
    Task t;
    t.name = name;
    t.func = func;
    t.priority = priority;
    t.remainingTicks = ticks;
    taskList.push_back(t);

    cout << "[등록] " << name << " (우선순위: " << priority << ", 필요 tick: " << ticks << ")" << endl;
}

// 테스트용 더미 함수들 (실제 Task가 할 일이라고 가정)
void taskSensor()    { cout << ">> SensorRead 진행 중..." << endl; }
void taskEmergency() { cout << ">> EmergencyStop 진행 중..." << endl; }
void taskLog()       { cout << ">> LogWriter 진행 중..." << endl; }
void taskUrgent()    { cout << ">> FireDetected 진행 중..." << endl; }
void taskD()         { cout << ">> TaskD 진행 중..." << endl; }
void taskE()         { cout << ">> TaskE 진행 중..." << endl; }

// 모든 Task의 remainingTicks가 0이 됐는지(=다 끝났는지) 확인
bool allDone() {
    for (const Task& t : taskList) {
        if (t.remainingTicks > 0) return false;
    }
    return true;
}

// 이번 tick에 실행할 Task를 하나 골라서 인덱스를 반환하는 함수
// 매 tick마다: 아직 안 끝난 것 중 최고 우선순위 찾고, 동점이면 rrPointer부터 라운드로빈
int findNext() {
    // 아직 안 끝난 Task들 중 가장 급한 우선순위 값이 뭔지 찾기 
    int minPriority = 1e9; // 일단 아주 큰 값으로 시작
    for (const Task& t : taskList)
        if (t.remainingTicks > 0 && t.priority < minPriority) minPriority = t.priority;

    // 그 등급(minPriority)을 가진 후보들 중에서, 라운드로빈 순서로 하나 고르기
    int n = taskList.size();
    for(int i = 0; i < n; i++) {
        // rrPointer부터 시작해서 순서대로 검사 (한 바퀴 돌면 처음 자리로 돌아오도록 % n)
        // 예: rrPointer가 3이고 n이 5면 -> 3, 4, 0, 1, 2 순서로 검사
        int idx = (rrPointer + i) % n;
        // 조건: 안 끝났고(remainingTicks > 0), 아까 찾은 최고 우선순위와 같은 등급인지 체크
        if (taskList[idx].remainingTicks > 0 && taskList[idx].priority == minPriority)
            return idx; // rrPointer부터 훑다가 처음으로 걸리는 후보를 바로 선택
    }

    return -1;
}

// 등록된 Task를 순서대로 하나씩 실행하는 함수
void runScheduler() {
    cout << "\n--- 스케줄러 실행 시작 ---\n" << endl;
    int tick = 0;

    while (!allDone()) {
        tick++;
        int idx = findNext(); // 이번 tick에 누굴 실행할지 결정

        cout << "[tick " << tick << "] 실행: " << taskList[idx].name
             << " (남은 tick: " << taskList[idx].remainingTicks
             << " -> " << taskList[idx].remainingTicks - 1 << ")" << endl;

        // 딱 "한 tick 분량"만 실행 (전체를 다 끝내는 게 아니라 1만큼만 진행)
        taskList[idx].func();
        taskList[idx].remainingTicks--;

        // 라운드로빈 포인터 갱신
        // 다음번엔 방금 실행한 애 바로 다음 자리부터 찾으라고 기억해둠
        // 이게 없으면 매번 findNext()가 인덱스 0부터 찾아서, 앞쪽 Task만 계속 유리해짐
        rrPointer = (idx + 1) % taskList.size();

        // 시연용: EmergencyStop이 1 tick만 처리된 시점에 더 급한 이벤트 발생
        if (taskList[idx].name == "EmergencyStop" && taskList[idx].remainingTicks == 2) {
            cout << "\n[!] 실행 도중 긴급 이벤트 발생 - 새 Task 등록 (선점 예상)\n" << endl;
            registerTask("FireDetected", taskUrgent, -1, 2);
        }
    }

    cout << "\n--- 모든 Task 완료 (총 " << tick << " tick 소요) ---" << endl;
}

int main() {

    registerTask("SensorRead", taskSensor, 2, 2);
    registerTask("EmergencyStop", taskEmergency, 0, 3);
    registerTask("LogWriter", taskLog, 5, 2);
    registerTask("TaskD", taskD, 3, 2); // TaskD, TaskE는 우선순위(3)가 동일
    registerTask("TaskE", taskE, 3, 2);

    // 이제 sort()를 미리 호출할 필요가 없음 — 스케줄러가 매번 알아서 최고 우선순위를 찾음
    runScheduler();
    
    return 0;
}