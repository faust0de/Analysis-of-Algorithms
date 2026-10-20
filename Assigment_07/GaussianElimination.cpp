#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <stdexcept>
using namespace std;

void validateMatrix(const vector<vector<double>>& A){
    int n=A.size();
    for(int i=0; i<n; i++){
        if(A[i].size()!=static_cast<size_t>(n)){
            throw invalid_argument("The coefficient matrix must be square.");
        }
    }
}

// Gaussian elimination without pivoting: A=L*U. Time: Theta(n^3).
void LUDecomposition(const vector<vector<double>>& A,
                     vector<vector<double>>& L, vector<vector<double>>& U){
    validateMatrix(A);
    int n=A.size();
    L=vector<vector<double>>(n,vector<double>(n,0));
    U=A;

    for(int i=0; i<n; i++){
        L[i][i]=1;
    }

    for(int i=0; i<n; i++){
        if(U[i][i]==0){
            throw runtime_error("Zero pivot: LU decomposition without pivoting cannot solve this system.");
        }
        for(int j=i+1; j<n; j++){
            double m=U[j][i]/U[i][i];
            L[j][i]=m;

            for(int k=i; k<n; k++){
                U[j][k]=U[j][k]-m*U[i][k];
            }
        }
    }
}

// Solves L*y=b in Theta(n^2) time.
vector<double> ForwardSubstitution(const vector<vector<double>>& L, const vector<double>& b){
    validateMatrix(L);
    int n=L.size();
    if(b.size()!=static_cast<size_t>(n)){
        throw invalid_argument("The vector size must match the matrix size.");
    }
    vector<double> y(n);

    for(int i=0; i<n; i++){
        if(L[i][i]==0){
            throw runtime_error("Zero diagonal element in forward substitution.");
        }
        double sum=0;
        for(int j=0; j<i; j++){
            sum=sum+L[i][j]*y[j];
        }
        y[i]=(b[i]-sum)/L[i][i];
    }
    return y;
}

// Solves U*x=y in Theta(n^2) time.
vector<double> BackwardSubstitution(const vector<vector<double>>& U, const vector<double>& y){
    validateMatrix(U);
    int n=U.size();
    if(y.size()!=static_cast<size_t>(n)){
        throw invalid_argument("The vector size must match the matrix size.");
    }
    vector<double> x(n);

    for(int i=n-1; i>=0; i--){
        if(U[i][i]==0){
            throw runtime_error("Zero diagonal element in backward substitution.");
        }
        double sum=0;
        for(int j=i+1; j<n; j++){
            sum=sum+U[i][j]*x[j];
        }
        x[i]=(y[i]-sum)/U[i][i];
    }
    return x;
}

vector<double> SolveSystemLU(const vector<vector<double>>& A, const vector<double>& b){
    vector<vector<double>> L,U;
    LUDecomposition(A,L,U);
    vector<double> y=ForwardSubstitution(L,b);
    return BackwardSubstitution(U,y);
}

void printMatrix(const vector<vector<double>>& A){
    cout<<fixed<<setprecision(2);
    for(int i=0; i<static_cast<int>(A.size()); i++){
        cout<<"| ";
        for(int j=0; j<static_cast<int>(A[i].size()); j++){
            cout<<setw(10)<<A[i][j];
        }
        cout<<" |\n";
    }
    cout<<endl;
}

void printVector(const vector<double>& x){
    cout<<fixed<<setprecision(2)<<"[";
    for(int i=0; i<static_cast<int>(x.size()); i++){
        if(i>0){
            cout<<", ";
        }
        cout<<x[i];
    }
    cout<<"]\n"<<endl;
}

int main(){
    vector<vector<double>> A={
        {2,-4,1},
        {3,-1,1},
        {1,1,-1}
    };
    vector<double> b={6,11,-3};

    cout<<"Coefficient matrix A:\n"<<endl;
    printMatrix(A);
    cout<<"Vector b:\n"<<endl;
    printVector(b);

    vector<vector<double>> L,U;
    vector<double> y,x;
    auto start=chrono::steady_clock::now();
    try{
        LUDecomposition(A,L,U);
        y=ForwardSubstitution(L,b);
        x=BackwardSubstitution(U,y);
    }catch(const exception& error){
        cerr<<error.what()<<endl;
        return 1;
    }
    auto end=chrono::steady_clock::now();
    auto duration=chrono::duration<double,micro>(end-start);

    cout<<"Lower triangular matrix L:\n"<<endl;
    printMatrix(L);
    cout<<"Upper triangular matrix U:\n"<<endl;
    printMatrix(U);
    cout<<"Intermediate vector y:\n"<<endl;
    printVector(y);
    cout<<"Solution vector x:\n"<<endl;
    printVector(x);
    cout<<"The duration is (µs):\t"<<duration.count()<<endl;
    return 0;
}
