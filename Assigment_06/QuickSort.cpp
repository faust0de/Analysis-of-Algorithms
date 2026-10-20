#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
using namespace std;

int HoarePartition(vector<int>& A, int l, int r, long long& comparisons){
    int p=A[l];
    int i=l, j=r+1;

    do{
        do{
            i++;
        }while(i<=r && (++comparisons, A[i]<p));

        do{
            j--;
        }while((++comparisons, A[j]>p));

        // Avoid accessing A[r+1] when the pivot is the largest element.
        if(i<j){
            swap(A[i],A[j]);
        }
    }while(i<j);

    swap(A[l],A[j]);
    return j;
}

void QuickSort(vector<int>& A, int l, int r, long long& comparisons){
    if(l<r){
        int s=HoarePartition(A,l,r,comparisons);
        QuickSort(A,l,s-1,comparisons);
        QuickSort(A,s+1,r,comparisons);
    }
}

vector<int>& randomVector(vector<int>& A){
    int n=A.size();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,100);
    for(int i=0; i<n; i++){
        A[i]=dist(gen);
    }
    return A;
}

int main(){
    vector<int> sizes={1000,2000,4000,8000};
    vector<string> labels={"1,000","2,000","4,000","8,000"};

    cout<<"| "<<left<<setw(7)<<"n"
        <<" | "<<setw(21)<<"Execution time (µs)"
        <<" | "<<setw(12)<<"Comparisons"<<" |\n";
    cout<<"|---------|----------------------|--------------|\n";

    for(int i=0; i<static_cast<int>(sizes.size()); i++){
        int n=sizes[i];
        vector<int> A(n);
        randomVector(A);
        long long comparisons=0;

        // Count only comparisons between array elements and the pivot.
        auto start=chrono::steady_clock::now();
        QuickSort(A,0,n-1,comparisons);
        auto end=chrono::steady_clock::now();
        auto duration=chrono::duration_cast<chrono::microseconds>(end-start);

        if(!is_sorted(A.begin(),A.end())){
            cerr<<"Sorting failed for n="<<n<<endl;
            return 1;
        }

        cout<<"| "<<left<<setw(7)<<labels[i]
            <<" | "<<right<<setw(20)<<duration.count()
            <<" | "<<setw(12)<<comparisons<<" |\n";
    }
    return 0;
}
