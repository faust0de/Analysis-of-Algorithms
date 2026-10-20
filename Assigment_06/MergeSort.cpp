#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;

vector<int>& Merge(vector<int>& B, vector<int>& C, vector<int>& A){
    int i=0, j=0, k=0;
    int p= B.size(), q=C.size();
    while(i<p && j<q){
        if(B[i]<=C[j]){
            A[k]=B[i];
            i++;
        }else{
            A[k]=C[j];
            j++;
        }
        k++;
    }
    if (i==p){
        copy(C.begin()+j,C.end(),A.begin()+k);
    }else{
        copy(B.begin()+i,B.end(),A.begin()+k);
    }
    return A;
}

vector<int> MergeSort(vector<int> A){
    int n=A.size();
    int m=n/2;
    if (n>1){
        vector<int> B(m);
        copy(A.begin(),A.begin()+m,B.begin());
        vector<int> C(n-m);
        copy(A.begin()+m,A.end(),C.begin());

        B=MergeSort(B);
        C=MergeSort(C);

        Merge(B,C,A);
    }
    return A;
}

vector<int>& randomVector(vector<int>& A){
    int n= A.size();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,100);
    for(int i=0; i<n; i++){
        A[i]=dist(gen);
    }
    return A;
}

int main(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1,1000);
    
    int iteraions;
    cout<<"Write the number of iterations\n"<<endl;
    cin>>iteraions;
    
    for(int i=0; i<iteraions;i++){
        int n= dist(rd);

        vector<int> A(n);
        randomVector(A);

        cout<<"Unsorted Array:\t";
        for(int i=0;i<n;i++){
            cout<<A[i]<<"\n"<<endl;
        }

        auto start=chrono::steady_clock::now();
        A=MergeSort(A);
        auto end=chrono::steady_clock::now();
        auto duration= chrono::duration_cast<chrono::microseconds>(end-start);

        cout<<"Sorted Array:\t";
        for(int i=0; i<n;i++){
            cout<<A[i]<<"\n"<<endl;
        }

        cout<<"The duration is:\t"<<duration.count()<<"\n"<<endl;
    }
    return 0;
}