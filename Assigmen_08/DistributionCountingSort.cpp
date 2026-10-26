#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
using namespace std;

vector<int> DistributionCountingSort(vector<int> A, int l, int u){
    int n=A.size();
    vector<int> D(u-l+1);
    vector<int> S(n);

    for(int j=0; j<=u-l; j++){
        D[j]=0;
    }

    for(int i=0; i<n; i++){
        D[A[i]-l]=D[A[i]-l]+1;
    }

    for(int j=1; j<=u-l; j++){
        D[j]=D[j-1]+D[j];
    }

    for(int i=n-1; i>=0; i--){
        int j=A[i]-l;
        S[D[j]-1]=A[i];
        D[j]=D[j]-1;
    }
    return S;
}

vector<int> ComparisonCountingSort(vector<int> A){
    int n=A.size();
    vector<int> Count(n,0);
    vector<int> S(n);
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if(A[i]<A[j]){
                Count[j]++;
            }else{
                Count[i]++;
            }
        }
    }
    for(int i=0;i<n;i++){
        S[Count[i]]=A[i];
    }
    return S;
}

void printVector(const vector<int>& A){
    cout<<"[";
    for(int i=0; i<static_cast<int>(A.size()); i++){
        if(i>0){
            cout<<", ";
        }
        cout<<A[i];
    }
    cout<<"]\n";
}

int main(){
    vector<int> A={3,47,2,21,60,32,3,0,60};
    int l=0, u=100;

    cout<<"Distribution Counting Sort test\n"<<endl;
    cout<<"Original array: ";
    printVector(A);
    vector<int> S=DistributionCountingSort(A,l,u);
    cout<<"Sorted array:   ";
    printVector(S);
    vector<int> expected=A;
    sort(expected.begin(),expected.end());
    if(S!=expected){
        cerr<<"Distribution Counting Sort test failed."<<endl;
        return 1;
    }
    cout<<"Test passed.\n"<<endl;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(l,u);
    vector<int> sizes={1000,2000,4000,8000};
    int iterations=5;

    cout<<"Average execution time (µs), "<<iterations<<" random arrays per size\n";
    cout<<"Range: ["<<l<<", "<<u<<"], k = "<<u-l+1<<"\n"<<endl;
    cout<<"| "<<left<<setw(8)<<"n"
        <<" | "<<setw(16)<<"DCS (µs)"
        <<" | "<<setw(16)<<"CCS (µs)"<<" |\n";
    cout<<"|----------|-----------------|-----------------|\n";

    for(int n:sizes){
        double dcsTime=0, ccsTime=0;
        for(int iteration=0; iteration<iterations; iteration++){
            vector<int> B(n);
            for(int i=0; i<n; i++){
                B[i]=dist(gen);
            }

            auto start=chrono::steady_clock::now();
            vector<int> D=DistributionCountingSort(B,l,u);
            auto end=chrono::steady_clock::now();
            dcsTime+=chrono::duration<double,micro>(end-start).count();

            start=chrono::steady_clock::now();
            vector<int> C=ComparisonCountingSort(B);
            end=chrono::steady_clock::now();
            ccsTime+=chrono::duration<double,micro>(end-start).count();

            expected=B;
            sort(expected.begin(),expected.end());
            if(D!=expected || C!=expected){
                cerr<<"Sorting failed for n="<<n<<endl;
                return 1;
            }
        }
        cout<<"| "<<left<<setw(8)<<n
            <<" | "<<right<<fixed<<setprecision(2)<<setw(15)<<dcsTime/iterations
            <<" | "<<setw(15)<<ccsTime/iterations<<" |\n";
    }

    return 0;
}
