#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <stdexcept>
using namespace std;

vector<vector<double>>& ForwardElimination(vector<vector<double>>& A){
    int n=A.size();
    for(int i=0; i<n; i++){
        if(A[i].size()!=static_cast<size_t>(n+1)){
            throw invalid_argument("The augmented matrix must have n rows and n+1 columns.");
        }
    }

    for(int i=0; i<n-1; i++){
        // This algorithm assumes nonzero pivots and does not swap rows.
        if(A[i][i]==0){
            throw runtime_error("Zero pivot: forward elimination requires a row exchange.");
        }
        for(int j=i+1; j<n; j++){
            double m=A[j][i]/A[i][i];

            for(int k=i; k<n+1; k++){
                A[j][k]=A[j][k]-m*A[i][k];
            }
        }
    }
    return A;
}

void printMatrix(const vector<vector<double>>& A){
    int n=A.size();
    cout<<fixed<<setprecision(2);
    for(int i=0; i<n; i++){
        cout<<"| ";
        for(int j=0; j<n+1; j++){
            if(j==n){
                cout<<" |";
            }
            cout<<setw(10)<<A[i][j];
        }
        cout<<" |\n";
    }
    cout<<endl;
}

int main(){
    vector<vector<double>> A={
        {2,-4,1,6},
        {3,-1,1,11},
        {1,1,-1,-3}
    };

    cout<<"Augmented matrix:\n"<<endl;
    printMatrix(A);

    auto start=chrono::steady_clock::now();
    try{
        ForwardElimination(A);
    }catch(const exception& error){
        cerr<<error.what()<<endl;
        return 1;
    }
    auto end=chrono::steady_clock::now();
    auto duration=chrono::duration_cast<chrono::microseconds>(end-start);

    cout<<"Upper triangular matrix:\n"<<endl;
    printMatrix(A);
    cout<<"The duration is (µs):\t"<<duration.count()<<endl;
    return 0;
}
