#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <random>
using namespace std;

vector<double> BackwardSubstitution(const vector<vector<double>>& A){
    int n=A.size();
    vector<double> x(n);

    for(int i=0; i<n; i++){
        if(A[i].size()!=static_cast<size_t>(n+1)){
            throw invalid_argument("The augmented matrix must have n rows and n+1 columns.");
        }
    }

    // The input must be upper triangular with nonzero diagonal elements.
    // The inner loop performs n(n-1)/2 multiplications: Theta(n^2).
    for(int i=n-1; i>=0; i--){
        if(A[i][i]==0){
            throw runtime_error("Zero diagonal element: backward substitution cannot divide by zero.");
        }

        double sum=0;
        for(int j=i+1; j<n; j++){
            sum=sum+A[i][j]*x[j];
        }

        x[i]=(A[i][n]-sum)/A[i][i];
    }
    return x;
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

vector<vector<double>>& randomMatrix(vector<vector<double>>& A, mt19937& gen){
    int n=A.size();
    uniform_int_distribution<int> dist(-10,10);
    uniform_int_distribution<int> diagonal(1,10);
    for(int i=0; i<n; i++){
        for(int j=0; j<n+1; j++){
            if(j<i){
                A[i][j]=0;
            }else if(j==i){
                A[i][j]=diagonal(gen);
            }else{
                A[i][j]=dist(gen);
            }
        }
    }
    return A;
}

int main(){
    int iterations;
    cout<<"Write the number of iterations\n"<<endl;
    cin>>iterations;
    if(!cin || iterations<=0){
        cerr<<"The number of iterations must be a positive integer."<<endl;
        return 1;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> sizeDist(2,6);
    vector<vector<double>> previous;

    for(int iteration=0; iteration<iterations; iteration++){
        int n=sizeDist(gen);
        vector<vector<double>> A(n,vector<double>(n+1));
        do{
            randomMatrix(A,gen);
        }while(A==previous);
        previous=A;

        cout<<"\nIteration "<<iteration+1<<" (n="<<n<<")\n"<<endl;
        cout<<"Upper triangular matrix:\n"<<endl;
        printMatrix(A);

        vector<double> x;
        auto start=chrono::steady_clock::now();
        try{
            x=BackwardSubstitution(A);
        }catch(const exception& error){
            cerr<<error.what()<<endl;
            return 1;
        }
        auto end=chrono::steady_clock::now();
        auto duration=chrono::duration<double,micro>(end-start);

        cout<<"Solution vector:\n"<<endl;
        for(int i=0; i<static_cast<int>(x.size()); i++){
            cout<<"x"<<i+1<<" = "<<x[i]<<endl;
        }
        cout<<"\nThe duration is (µs):\t"<<duration.count()<<endl;
    }
    return 0;
}
