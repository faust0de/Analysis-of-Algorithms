#include <iostream>
using namespace std;
#include <list>

//4. Find the way C++ uses to push and pop elements to and from a list to behave like a stack.
//Create an example that showcases the idea.

int main() {
    list<int> stack;

    // Push elements onto the stack
    stack.push_back(10);
    stack.push_back(20);
    stack.push_back(30);

    cout << "Stack after pushing elements: ";
    for (const auto& elem : stack) {
        cout << elem << " ";
    }
    cout << endl;

    // Pop elements from the stack
    while (!stack.empty()) {
        cout << "Popping element: " << stack.back() << endl;
        stack.pop_back();
    }

    cout << "Stack after popping all elements: ";
    for (const auto& elem : stack) {
        cout << elem << " ";
    }
    cout << endl;

    return 0;
}