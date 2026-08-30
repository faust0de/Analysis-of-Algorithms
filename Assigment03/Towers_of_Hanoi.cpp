#include<iostream>
#include<list>
#include<chrono>
#include<iomanip> //We use iomanip for formatting the output to a fixed number of decimal places
using namespace std;

class Stack{
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

/*
1. Implement the Towers of Hanoi as a recursive algorithm and as an iterative algorithm. Compare
the computational complexity of the algorithm with the mathematical analysis and with the
empirical analysis techniques discussed during the lecture. Mathematical analysis was already
done in class. Check for consistency between both methods
*/

long long TowersOfHanoi_recursive(Stack &source, Stack &auxiliary, Stack &destination, int n){
    if (n == 1) {
        destination.push(source.top());
        source.pop();
        return 1; // Base case M(1)=1
    }
    long long moves = 0;
    moves += TowersOfHanoi_recursive(source, destination, auxiliary, n - 1); // Moves the top n-1 disks from source to auxiliary
    destination.push(source.top());
    source.pop();
    moves++; // Moves the nth disk from source to destination
    moves += TowersOfHanoi_recursive(auxiliary, source, destination, n - 1); // Moves the n-1 disks from auxiliary to destination
    return moves;
}

void legalMove(Stack &a, Stack &b){
    // If stack a is empty, move from b to a
    if (a.empty()) {
        a.push(b.top());
        b.pop();
    }
    // If stack b is empty, move from a to b
    else if (b.empty()) {
        b.push(a.top());
        a.pop();
    }
    // If the top disk of a is smaller, move from a to b
    else if (a.top() < b.top()) {
        b.push(a.top());
        a.pop();
    }
    // Otherwise, move from b to a
    else {
        a.push(b.top());
        b.pop();
    }
}


long long TowersOfHanoi_iterative(Stack &source, Stack &auxiliary, Stack &destination, int n){
    long long moves = 0;
    long long totalMoves = (1LL << n) - 1; // M(n) = 2^n - 1

    for (long long i = 1; i <= totalMoves; i++) {

        // For an odd number of disks
        if (n % 2 == 1) {

            if (i % 3 == 1) {
                // Move between source and destination
                legalMove(source, destination);
            }
            else if (i % 3 == 2) {
                // Move between source and auxiliary
                legalMove(source, auxiliary);
            }
            else {
                // Move between auxiliary and destination
                legalMove(auxiliary, destination);
            }
        }

        // For an even number of disks
        else {

            if (i % 3 == 1) {
                // Move between source and auxiliary
                legalMove(source, auxiliary);
            }
            else if (i % 3 == 2) {
                // Move between source and destination
                legalMove(source, destination);
            }
            else {
                // Move between auxiliary and destination
                legalMove(auxiliary, destination);
            }
        }

        moves++;
    }

    return moves;
}

int main(){
    /*
        Empirical analysis section:
        We test n from 5 to 22 to observe the exponential growth of the number of moves.
        The recurrence is M(n) = 2M(n-1) + 1, whose closed form is M(n) = 2^n - 1.
        The measured data is compared with this theoretical result to verify whether the empirical
        results are consistent with the mathematical analysis of the algorithm.
    */

    const int repetitions = 5;
    const int start_n = 5;
    const int end_n = 22;

    cout << fixed << setprecision(6);
    cout << "Empirical Analysis of Towers of Hanoi" << endl;
    cout << "Theoretical model: M(n) = 2^n - 1" << endl;
    cout << "n\tTheoretical Moves\tRecursive Moves\tIterative Moves\tAverage Recursive Time (ms)\tAverage Iterative Time (ms)" << endl;

    bool recursiveMatches = true;
    bool iterativeMatches = true;

    for (int n = start_n; n <= end_n; n++) {
        long long theoreticalMoves = (1LL << n) - 1;

        double recursiveTotalTime = 0.0;
        double iterativeTotalTime = 0.0;
        long long recursiveMoves = 0;
        long long iterativeMoves = 0;

        for (int r = 0; r < repetitions; r++) {
            // Fresh stacks are created for every repetition so the algorithm always starts from a valid initial state.
            Stack source, auxiliary, destination;

            // Initialize only the source stack with n disks; this is not included in the measured execution time.
            for (int i = n; i >= 1; i--) {
                source.push(i);
            }

            auto start = std::chrono::high_resolution_clock::now();
            recursiveMoves = TowersOfHanoi_recursive(source, auxiliary, destination, n);
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedRecursive = std::chrono::duration<double, std::milli>(end - start);
            recursiveTotalTime += elapsedRecursive.count();

            // The stacks are re-created for each repetition, so no prior state is reused.
            Stack source2, auxiliary2, destination2;
            for (int i = n; i >= 1; i--) {
                source2.push(i);
            }

            start = std::chrono::high_resolution_clock::now();
            iterativeMoves = TowersOfHanoi_iterative(source2, auxiliary2, destination2, n);
            end = std::chrono::high_resolution_clock::now();
            auto elapsedIterative = std::chrono::duration<double, std::milli>(end - start);
            iterativeTotalTime += elapsedIterative.count();
        }

        double avgRecursiveTime = recursiveTotalTime / repetitions;
        double avgIterativeTime = iterativeTotalTime / repetitions;

        // Check consistency with the theoretical model: M(n) = 2^n - 1
        if (recursiveMoves != theoreticalMoves) {
            recursiveMatches = false;
        }
        if (iterativeMoves != theoreticalMoves) {
            iterativeMatches = false;
        }

        cout << n << "\t" << theoreticalMoves << "\t\t" << recursiveMoves << "\t\t" << iterativeMoves
             << "\t\t" << avgRecursiveTime << "\t\t" << avgIterativeTime << endl;
    }

    cout << endl;
    cout << "Comparison with mathematical analysis:" << endl;
    cout << "Recursive implementation matches theoretical model: " << (recursiveMatches ? "YES" : "NO") << endl;
    cout << "Iterative implementation matches theoretical model: " << (iterativeMatches ? "YES" : "NO") << endl;
    cout << "Expected complexity: Theta(2^n)" << endl;

    return 0;
}
