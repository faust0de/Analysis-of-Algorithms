#include<iostream>
#include <vector>
using namespace std;

vector<int> ComparisonCountingSort(vector<int> A){
    int n=A.size();
    vector<int> Count(n,0);
    vector<int> S(n);
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if (A[i]<A[j]){
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

int main(){
    vector<int> A={3,47,2,21,60,32};
    int n=A.size();
    for(int i=0;i<n;i++){
        cout<<A[i]<<"\n";
    }
    
    A=ComparisonCountingSort(A);

    for(int i=0;i<n;i++){
        cout<<A[i]<<"\n";
    }

    return 0;
}
