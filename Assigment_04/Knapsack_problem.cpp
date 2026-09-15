#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>
#include <iomanip>

using namespace std;


// Random number generator
random_device rd;
mt19937 gen(rd());

struct ExperimentResult {

    int n;
    int capacity;
    int best_weight;
    int best_value;
    double execution_time;
};

vector<int> random_weight_vectors(int n) {

    vector<int> v(n);

    // Generate random weights between 1 and 50
    uniform_int_distribution<int> weight_dist(1, 50);

    for (int i = 0; i < n; i++) {

        v[i] = weight_dist(gen);
    }

    return v;
}


vector<int> random_value_vectors(int n) {

    vector<int> v(n);

    // Generate random values between 1 and 100
    uniform_int_distribution<int> value_dist(1, 100);

    for (int i = 0; i < n; i++) {

        v[i] = value_dist(gen);
    }

    return v;
}


int random_capacity(vector<int> weights) {

    int total_weight = 0;

    for (int i = 0; i < weights.size(); i++) {

        total_weight += weights[i];
    }

    // Generate a random capacity between 1 and total_weight
    uniform_int_distribution<int> capacity_dist(1, total_weight);

    return capacity_dist(gen);
}

vector<int> exhaustive_search(const vector<int>& weights, const vector<int>& values, int capacity) {

    int n = weights.size();
    // Number of items

    uint64_t total = 1ULL << n;
    // Total number of possible subsets: 2^n

    int best_weight = 0;
    // Weight of the best feasible subset

    int best_value = 0;
    // Value of the best feasible subset


    for (uint64_t i = 0; i < total; i++) {
        // Iterate through every possible subset

        int weight = 0;
        // Weight of the current subset

        int value = 0;
        // Value of the current subset


        for (int j = 0; j < n; j++) {
            // Check every item

            if ((i & (1ULL << j)) != 0) {
                // If bit j is 1, item j belongs
                // to the current subset

                weight += weights[j];
                value += values[j];
            }
        }


        if (weight <= capacity && value > best_value) {
            // The subset must fit inside the knapsack
            // and have a better value than the best
            // solution found so far

            best_weight = weight;
            best_value = value;
        }
    }


    return {best_weight, best_value};
}


int main() {

    int n;

    cout << "Enter maximum problem size: ";
    cin >> n;

    // Vector used to store the results of each experiment
    vector<ExperimentResult> table;


    for (int i = 3; i <= n; i++) {

        // Generate a random knapsack problem
        vector<int> weights = random_weight_vectors(i);
        vector<int> values = random_value_vectors(i);
        int capacity = random_capacity(weights);


        // Start measuring execution time
        auto start = chrono::high_resolution_clock::now();


        // Solve the problem
        vector<int> result =
            exhaustive_search(weights, values, capacity);


        // Stop measuring execution time
        auto end = chrono::high_resolution_clock::now();


        // Calculate execution time in seconds
        chrono::duration<double> duration = end - start;


        // Store the results in the table
        table.push_back({
            i,                  // Number of items
            capacity,           // Knapsack capacity
            result[0],          // Best weight
            result[1],          // Best value
            duration.count()    // Execution time
        });


        // Optional information during execution
        cout << "Finished n = " << i
             << " | Time: "
             << duration.count()
             << " seconds"
             << endl;


        // Stop the experiment when execution reaches 30 seconds
        if (duration.count() >= 30.0) {

            cout << "\n30 second limit reached." << endl;

            break;
        }
    }


    // Print results table

    cout << "\n\nEXPERIMENTAL RESULTS\n";

    cout << "--------------------------------------------------------------------------\n";

    cout << left
         << setw(10) << "n"
         << setw(15) << "Capacity"
         << setw(15) << "Best Weight"
         << setw(15) << "Best Value"
         << setw(20) << "Time (seconds)"
         << endl;

    cout << "--------------------------------------------------------------------------\n";

    for (const ExperimentResult& row : table) {

        cout << left
             << setw(10) << row.n
             << setw(15) << row.capacity
             << setw(15) << row.best_weight
             << setw(15) << row.best_value
             << setw(20) << row.execution_time
             << endl;
    }

    cout << "--------------------------------------------------------------------------\n";


    return 0;
}
