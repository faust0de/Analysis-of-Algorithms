#include <iostream>
using namespace std;

//1. Code Euclid’s Algorithms from the textbook.

/*
Euclid’s algorithm for computing gcd(m, n)

Step 1 If n = 0, return the value of m as the answer and stop; otherwise,
proceed to Step 2.

Step 2 Divide m by n and assign the value of the remainder to r.

Step 3 Assign the value of n to m and the value of r to n. Go to Step 1.
*/

int gcd(int m,int n){
    if(n==0){
        return m;
    }
    int r = m % n;
    return gcd(n,r);
}

int main(){
    int m,n;
    cout << "Enter two numbers to find their GCD: " << endl;
    cin >> m >> n;
    cout << "The GCD of " << m << " and " << n << " is: " << gcd(m,n) << endl;
    return 0;
}
