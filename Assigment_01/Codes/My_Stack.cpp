#include <iostream>
#include <list>
using namespace std;

//5. Create a class that encapsulates the stack behavior describe in point 4 (Ex: MyStack)

class MyStack{
    private:
        list<int> data;

    public:
        void push(int value) {
            data.push_back(value);
        }

        void pop() {
            if (!data.empty()) {
                data.pop_back();
            }
        }

        int top() {
            if (!data.empty()) {
                return data.back();
            }
            return -1; // or throw an exception
        }

        bool empty() {
            return data.empty();
        }
};

int main() {
    MyStack stack;

    // Push elements onto the stack
    stack.push(10);
    stack.push(20);
    stack.push(30);

    cout << "Stack after pushing elements: ";
    while (!stack.empty()) {
        cout << stack.top() << " ";
        stack.pop();
    }
    cout << endl;

    return 0;
}