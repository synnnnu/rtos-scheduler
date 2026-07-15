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

int main() {

    return 0;
}