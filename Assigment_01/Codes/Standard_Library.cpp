#include <iostream>
using namespace std;
#include <stack>
#include <queue>

//8. Explore which alternatives does C++ Standard Library provide to deal with stacks and queues.Provide examples

//Stack example using std::stack
void stackExample() {
    stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    while (!s.empty()) {
        cout << s.top() << "\n";
        s.pop();
    }
}

//Queue example using std::queue
void queueExample() {
    queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    while (!q.empty()) {
        cout << q.front() << "\n";
        q.pop();
    }
}

int main() {
    cout << "Stack example using std::stack:\n";
    stackExample();

    cout << "\nQueue example using std::queue:\n";
    queueExample();

    return 0;
}