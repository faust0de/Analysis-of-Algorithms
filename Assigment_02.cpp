#include <iostream>
using namespace std;

/*
The Fibonacci sequence is generated using the following recurrence:
T(n) = T(n-2) + T(n-1)
*/

//Write a C++ program that generates the sequence using iteration

int fibonacci_iteration(int n) {
    if (n <= 1) {
        return n;
    }
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

//Write a C++ program that generates the sequence using recursion

int fibonacci_recursion(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci_recursion(n - 1) + fibonacci_recursion(n - 2);
}

int main() {
    int n;
    
    cout << "Write any number to generate the Fibonacci sequence: " << endl;
    
    cin >> n;
    
    cout << "The Fibonacci number at position " << n << " is: " << fibonacci_iteration(n) << endl;
    
    cout << "The Fibonacci number at position " << n << " is: " << fibonacci_recursion(n) << endl;
    
    return 0;
}
