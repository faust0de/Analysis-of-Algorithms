#include <iostream>
using namespace std;
#include <cmath>

//3. Code the Middle-school procedure algorithm from the textbook (You will need to code the Sieve of Eratosthenes algorithm too)

/*
Middle-school procedure for computing gcd(m, n)
Step 1 Find the prime factors of m.
Step 2 Find the prime factors of n.
Step 3 Identify all the common factors in the two prime expansions found in
Step 1 and Step 2. (If p is a common factor occurring pm and pn times
in m and n, respectively, it should be repeated min{pm, pn} times.)
Step 4 Compute the product of all the common factors and return it as the
greatest common divisor of the numbers given.
*/

/*
ALGORITHM Sieve(n)
//Implements the sieve of Eratosthenes
//Input: A positive integer n> 1
//Output: Array L of all prime numbers less than or equal to n
1.1 What Is an Algorithm? 33
for p ← 2 to n do A[p] ← p
for p ← 2 to √n
do //see note before pseudocode
if A[p] = 0 //p hasn’t been eliminated on previous passes
j ← p ∗ p
while j ≤ n do
A[j ]← 0 //mark element as eliminated
j ← j + p
//copy the remaining elements of A to array L of the primes
i ← 0
for p ← 2 to n do
if A[p] = 0
L[i]← A[p]
i ← i + 1
return L
*/

int Sieve(int n, int primes[], int &count) {
    bool is_prime[n + 1];
    for (int i = 0; i <= n; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    for (int p = 2; p * p <= n; p++) {
        if (is_prime[p]) {
            for (int j = p * p; j <= n; j += p) {
                is_prime[j] = false;
            }
        }
    }

    count = 0;
    for (int p = 2; p <= n; p++) {
        if (is_prime[p]) {
            primes[count++] = p;
        }
    }
    return count;
}

int MiddleSchoolGCD(int m, int n) {
    int primes[100]; // Assuming we won't have more than 100 primes for simplicity
    int count_m, count_n;
    
    Sieve(max(m, n), primes, count_m); // Get primes up to max(m, n)
    
    int gcd = 1;
    for (int i = 0; i < count_m; i++) {
        int p = primes[i];
        if (p > m || p > n) break;

        int pm = 0, pn = 0;
        while (m % p == 0) {
            m /= p;
            pm++;
        }
        while (n % p == 0) {
            n /= p;
            pn++;
        }
        gcd *= pow(p, min(pm, pn));
    }
    return gcd;
}

int main() {
    int m, n;
    cout << "Enter two numbers to find their GCD using the Middle-school procedure algorithm: " << endl;
    cin >> m >> n;
    cout << "The GCD of " << m << " and " << n << " is: " << MiddleSchoolGCD(m, n) << endl;
    return 0;
}
