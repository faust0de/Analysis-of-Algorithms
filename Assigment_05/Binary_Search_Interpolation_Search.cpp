#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

vector<int> random_sorted_vector(){
    int n=10000000;
    vector <int> A;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,n);
    
    for(int i=0;i<n;i++){
        int random_number= dist(gen);
        A.push_back(random_number);
    }

    sort(A.begin(),A.end());

    return A;
}

int random_index_generator(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0,9999999);
    int index= dist(gen);
    return index;
}

int Binary_Search(vector<int>& A,int key){
    int low=0;
    int high= A.size()-1;
    int iterations=0;
    while(low<=high){
        iterations++;
        int mid= (low + high)/2;
        if (A[mid]==key){
            return mid, iterations;
        }else if (A[mid]<key){
            low=mid + 1;
        }else if(A[mid]>key){
            high=mid- 1;
        }
    }
    return -1;
}

int Interpolation_Search(const vector<int>& A, int key){
    int low=0;
    int high=A.size()-1;
    int iterations=0;
    while(low<=high && key>=A[low] && key<= A[high]){
        iterations ++;
        if(A[low]==A[high]){
            if(A[low]==key){
                return low, iterations;
            }else{
                return -1;
            }
        }
        long long numerator = static_cast<long long>(key - A[low]) * (high - low);
        int pos= low+numerator/(A[high]-A[low]);
        if(A[pos]==key){
            return pos, iterations;
        }else if(A[pos]<key){
            low= pos+1;
        }else{
            high=pos-1;
        }
    }
    cout<<"key not found \n"<<endl;
    return -1;
}

int main(){
    vector <int> A= random_sorted_vector();
    int key= A[random_index_generator()];
    
    auto start_binary = chrono::high_resolution_clock::now();

    int binary_result, binary_search_iterations = Binary_Search(A, key);

    auto end_binary = chrono::high_resolution_clock::now();
    
    auto binary_time = chrono::duration_cast<chrono::nanoseconds>(end_binary - start_binary);

    auto start_interpolation = chrono::high_resolution_clock::now();

    int interpolation_result, interpolation_iterations = Interpolation_Search(A, key);

    auto end_interpolation = chrono::high_resolution_clock::now();

    auto interpolation_time =chrono::duration_cast<chrono::nanoseconds>(end_interpolation - start_interpolation);

    cout << "Key: " << key << endl;

    cout << "Binary Search index: "
         << binary_result << endl;

    cout << "Binary Search time: "
         << binary_time.count()
         << " ns" << endl;
    
    cout <<"Binary Search iterations: "
         <<binary_search_iterations<< endl;

    cout << "Interpolation Search index: "
         << interpolation_result << endl;

    cout << "Interpolation Search time: "
         << interpolation_time.count()
         << " ns" << endl;

    cout <<"Interpolation Iterations: "
         <<interpolation_iterations<< endl;

    return 0;
}