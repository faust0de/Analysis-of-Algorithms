#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
using namespace std;

/*
ALGORITHM Random(n, m, seed, a, b)
//Generates a sequence of n pseudorandom numbers according to the linear
// congruential method
//Input: A positive integer n and positive integer parameters m, seed, a, b
//Output: A sequence r1,...,rn of n pseudorandom integers uniformly
// distributed among integer values between 0 and m − 1
//Note: Pseudorandom numbers between 0 and 1 can be obtained
// by treating the integers generated as digits after the decimal point
r0 ← seed
for i ← 1 to n do
ri ← (a ∗ ri−1 + b) mod m
*/

// This function implements the linear congruential generator.
// It computes the sequence in O(n) time because each iteration performs a constant amount of work.
// It does not store all generated values; it only keeps the current value and returns the final one.
long long generateRandomSequence(long long n, long long m, long long seed, long long a, long long b) {
    long long current = seed;

    for (long long i = 1; i <= n; i++) {
        current = (a * current + b) % m;
    }

    return current;
}

int main() {
    /*
        Empirical analysis of the linear congruential method:
        Theoretical analysis: each iteration executes a constant amount of work,
        so the total runtime is O(n), i.e., linear in the number of generated values.
        To compare theory with practice, we test several values of n and measure the
        average execution time over 5 repetitions for each value.
    */

    const int repetitions = 5;
    const long long m = 2147483647LL; // Large modulus for a typical LCG
    const long long seed = 123456789LL;
    const long long a = 1103515245LL;
    const long long b = 12345LL;

    cout << fixed << setprecision(6);
    cout << "Empirical Analysis of the Linear Congruential Method" << endl;
    cout << "n\tBasic Operations\tAverage Time (ms)\tLast Generated Value" << endl;

    vector<long long> values = {
        10000,
        20000,
        40000,
        80000,
        160000,
        320000,
        640000,
        1280000
    };

    for (long long n : values) {
        double totalTime = 0.0;
        long long basicOperations = n;
        long long lastGeneratedValue = 0;

        for (int r = 0; r < repetitions; r++) {
            auto start = chrono::high_resolution_clock::now();
            lastGeneratedValue = generateRandomSequence(n, m, seed, a, b);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;
            totalTime += elapsed.count();
        }

        double averageTime = totalTime / repetitions;
        cout << n << "\t" << basicOperations << "\t\t" << averageTime << "\t\t" << lastGeneratedValue << endl;
    }

    cout << endl;
    return 0;
}


