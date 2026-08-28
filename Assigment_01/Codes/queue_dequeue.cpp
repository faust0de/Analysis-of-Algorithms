#include <iostream>
#include <list>
using namespace std;

//6. Find the way C++ uses to queue and dequeue elements to and from a list to behave like a
//queue. Create an example that showcases the idea.

int main() {
    list<int> queue;

    // Enqueue elements into the queue
    queue.push_back(10);
    queue.push_back(20);
    queue.push_back(30);

    cout << "Queue after enqueuing elements: ";
    for (const auto& elem : queue) {
        cout << elem << " ";
    }
    cout << endl;

    // Dequeue elements from the queue
    while (!queue.empty()) {
        cout << "Dequeuing element: " << queue.front() << endl;
        queue.pop_front();
    }

    cout << "Queue after dequeuing all elements: ";
    for (const auto& elem : queue) {
        cout << elem << " ";
    }
    cout << endl;

    return 0;
}