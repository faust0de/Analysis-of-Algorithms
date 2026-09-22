#include <iostream>
#include <vector>
#include <random>

using namespace std;

vector<char> insertion_sort_algorithm(int n){
    random_device rd; // Generates seed
    mt19937 gen(rd()); // Random number generator
    uniform_int_distribution<int> dist('a','z'); // Range between a and z in ASCII

    vector<char> A; // Vector of chars

    for(int i = 0; i < n; i++){
        char letter = static_cast<char>(dist(gen)); // Generates a random letter
        A.push_back(letter); // Adds the letter to the vector
    }

    cout<<"Unsorted Vector \n"<<endl;
    for(char i:A){
        cout<<i<<endl;
    }

    for(int i = 1; i < n; i++){
        char key = A[i]; // Current letter to insert
        int j = i - 1;

        while(j >= 0 && A[j] > key){
            A[j + 1] = A[j]; // Shift bigger letter one position to the right
            j = j - 1;
        }

        A[j + 1] = key; // Insert key in its correct position
    }

    cout<<"Sorted Vector \n"<<endl;
    for(char i:A){
        cout<<i<<endl;
    }

    return A; // Returns sorted vector
}

int main(){
    int n;
    cout<<"Give a value for n \n"<<endl;
    cin>>n;
    vector<char> A= insertion_sort_algorithm(n);
    return 0;
}
