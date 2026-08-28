#include <iostream>
using namespace std;
#include <list>

//7. Create a class that encapsulates the stack behavior describe in point 6 (Ex: MyQueue)

class MyQueue {
    private:
        list<int> data;

    public:
        void enqueue(int value) {
            data.push_back(value);
        }

        void dequeue() {
            if (!data.empty()) {
                data.pop_front();
            }
        }

        int front() {
            if (!data.empty()) {
                return data.front();
            }
            return -1; // or throw an exception
        }

        bool empty() {
            return data.empty();
        }
};

int main() {
    MyQueue queue;

    // Enqueue elements into the queue
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    cout << "Queue after enqueuing elements:\n";
    
    while (!queue.empty()) {
        cout << queue.front() << "\n";
        queue.dequeue();
    }
    cout << endl;

    return 0;
}