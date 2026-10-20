#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
using namespace std;

void Heapify(vector<int>& A, int n, int i){
    int largest=i;
    int left=2*i+1;
    int right=2*i+2;

    if(left<n && A[left]>A[largest]){
        largest=left;
    }
    if(right<n && A[right]>A[largest]){
        largest=right;
    }

    if(largest!=i){
        swap(A[i],A[largest]);
        Heapify(A,n,largest);
    }
}

vector<int>& BuildHeap(vector<int>& A){
    int n=A.size();
    // Start at the last parent: leaves already satisfy the heap property.
    // Bottom-up construction takes Theta(n) time overall.
    for(int i=n/2-1; i>=0; i--){
        Heapify(A,n,i);
    }
    return A;
}

vector<int>& randomVector(vector<int>& A, mt19937& gen){
    int n=A.size();
    uniform_int_distribution<int> dist(0,100);
    for(int i=0; i<n; i++){
        A[i]=dist(gen);
    }
    return A;
}

void printVector(const vector<int>& A){
    int n=A.size();
    cout<<"[";
    for(int i=0; i<n; i++){
        if(i>0){
            cout<<", ";
        }
        cout<<A[i];
    }
    cout<<"]\n"<<endl;
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
    uniform_int_distribution<int> sizeDist(6,15);

    for(int iteration=0; iteration<iterations; iteration++){
        int n=sizeDist(gen);
        vector<int> A(n);
        randomVector(A,gen);

        cout<<"\nIteration "<<iteration+1<<" (n="<<n<<")\n"<<endl;
        cout<<"Random array:\t";
        printVector(A);

        auto start=chrono::steady_clock::now();
        BuildHeap(A);
        auto end=chrono::steady_clock::now();
        auto duration=chrono::duration<double,micro>(end-start);

        cout<<"Max-Heap:\t";
        printVector(A);
        cout<<fixed<<setprecision(2);
        cout<<"The duration is (µs):\t"<<duration.count()<<endl;
    }
    return 0;
}
