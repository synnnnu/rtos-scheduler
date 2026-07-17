#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Task 하나를 표현하는 구조체
struct Task {
    string name;    // 디버깅용 이름
    void (*func)(); // 이 Task가 실행할 함수 (함수 포인터)
    int priority;   // 숫자가 낮을수록 우선순위가 높다고 가정
    bool isDone;    // 실행 완료 여부
};

// 모든 Task를 담아둘 리스트
vector<Task> taskList;

// Task를 등록하는 함수
void registerTask(string name, void (*func)(), int priority) {
    Task t;
    t.name = name;
    t.func = func;
    t.priority = priority;
    t.isDone = false;
    taskList.push_back(t);

    cout << "[등록] " << name << " (우선순위: " << priority << ")" << endl;
}

// 테스트용 더미 함수들 (실제 Task가 할 일이라고 가정)
void taskA() { cout << ">> Task A 실행 중..." << endl; }
void taskB() { cout << ">> Task B 실행 중..." << endl; }
void taskC() { cout << ">> Task C 실행 중..." << endl; }
void taskUrgent() { cout << ">> Task Urgent: 화재 감지! 즉시 대응 중..." << endl; }

// 아직 안 끝난 Task가 하나라도 있는지 확인
bool allDone() {
    for (const Task& t : taskList) {
        if (!t.isDone) return false;
    }
    return true;
}

// 등록된 Task를 순서대로 하나씩 실행하는 함수
void runScheduler() {
    cout << "\n--- 스케줄러 실행 시작 ---\n" << endl;

    while (!allDone()) {
        // 매번 아직 안 끝난 것들 중 우선순위(숫자 작은 것)가 가장 높은 걸 찾음
        int bestIdx = -1;

        for(int i = 0; i < taskList.size(); i++) {
            if(!taskList[i].isDone) {
                if (bestIdx == -1 || taskList[i].priority < taskList[bestIdx].priority) {
                    bestIdx = i;
                }
            }
        }

        cout << "[실행] " << taskList[bestIdx].name << endl;
        taskList[bestIdx].func();
        taskList[bestIdx].isDone = true;

        // EmergencyStop 처리 직후, 그보다 더 급한 상황이 새로 발생했다고 가정
        if (taskList[bestIdx].name == "EmergencyStop") {
            cout << "\n[!] 실행 도중 긴급 이벤트 발생 - 새 Task 등록\n" << endl;
            registerTask("FireDetected", taskUrgent, -1);
        }
    }

    cout << "\n--- 모든 Task 실행 완료 ---" << endl;
}

int main() {

    registerTask("SeonsorRead", taskA, 2);
    registerTask("EmergencyStop", taskB, 0);
    registerTask("LogWriter", taskC, 5);    

    // 이제 sort()를 미리 호출할 필요가 없음 — 스케줄러가 매번 알아서 최고 우선순위를 찾음
    runScheduler();
    
    return 0;
}