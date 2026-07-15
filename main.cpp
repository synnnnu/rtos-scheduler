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

int main() {

    registerTask("SeonsorRead", taskA, 2);
    registerTask("EmergencyStop", taskB, 0);
    registerTask("LogWriter", taskC, 5);    

    cout << "\n--- 우선순위 순으로 정렬 ---\n" << endl;

    // 우선순위 기준으로 정렬 (숫자 작을수록 앞으로)
    sort(taskList.begin(), taskList.end(), [](const Task& a, const Task& b) {
        return a.priority < b.priority;
    });

    // 정렬된 순서대로 출력
    for (const Task& t : taskList) {
        cout << t.name << " (우선순위: " << t.priority << ")" << endl;
    }

    return 0;
}