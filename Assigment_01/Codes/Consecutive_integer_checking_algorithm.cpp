#include <iostream>
using namespace std;

//2. Code Consecutive integer checking algorithm from the textbook

/*
Consecutive integer checking algorithm for computing gcd(m, n)
Step 1 Assign the value of min{m, n} to t.
Step 2 Divide m by t. If the remainder of this division is 0, go to Step 3;
otherwise, go to Step 4.
Step 3 Divide n by t. If the remainder of this division is 0, return the value of
t as the answer and stop; otherwise, proceed to Step 4.
Step 4 Decrease the value of t by 1. Go to Step 2.
*/

int gcd_consecutive(int m, int n) {
    int t = min(m, n);
    while (t > 0) {
        if (m % t == 0 && n % t == 0) {
            return t;
        }
        t--;
    }
    return 1; // If no common divisor found, return 1
}

int main() {
    int m, n;
    cout << "Enter two numbers to find their GCD using the consecutive integer checking algorithm: " << endl;
    cin >> m >> n;
    cout << "The GCD of " << m << " and " << n << " is: " << gcd_consecutive(m, n) << endl;
    return 0;
}
