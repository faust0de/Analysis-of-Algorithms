#include <iostream>
using namespace std;

struct Slot{
    int key=0;
    bool empty=true;
};

int HashFunction(int key, int m){
    int index=key%m;
    // Keep the index valid for negative integer keys as well.
    if(index<0){
        index=index+m;
    }
    return index;
}

bool Insert(int key, Slot Table[], int m){
    if(m<=0){
        return false;
    }
    int index=HashFunction(key,m);

    for(int i=0; i<m; i++){
        if(Table[index].empty){
            Table[index].key=key;
            Table[index].empty=false;
            return true;
        }
        index=(index+1)%m;
    }
    return false;
}

int Search(int key, const Slot Table[], int m){
    if(m<=0){
        return -1;
    }
    int index=HashFunction(key,m);

    for(int i=0; i<m; i++){
        if(Table[index].empty){
            return -1;
        }
        if(Table[index].key==key){
            return index;
        }
        index=(index+1)%m;
    }
    return -1;
}

void printTable(const Slot Table[], int m){
    cout<<"\nIndex\tKey\n";
    for(int i=0; i<m; i++){
        cout<<i<<"\t";
        if(Table[i].empty){
            cout<<"Empty";
        }else{
            cout<<Table[i].key;
        }
        cout<<"\n";
    }
    cout<<endl;
}

int main(){
    const int m=7;
    Slot Table[m];
    int keys[]={10,17,24,6,13};
    int n=sizeof(keys)/sizeof(keys[0]);

    cout<<"Closed hashing with linear probing\n"<<endl;
    cout<<"Collision test (table size = "<<m<<"):\n";
    for(int i=0; i<n; i++){
        bool inserted=Insert(keys[i],Table,m);
        if(!inserted){
            return 1;
        }
        int index=HashFunction(keys[i],m);
        int position=Search(keys[i],Table,m);
        cout<<"Key "<<keys[i]<<": hash = "<<index;
        if(position!=index){
            cout<<" -> Collision";
        }
        cout<<", inserted at index "<<position<<endl;
    }
    printTable(Table,m);

    cout<<"Search test:\n";
    int queries[]={10,24,13,99};
    for(int key:queries){
        int position=Search(key,Table,m);
        cout<<"Search "<<key<<": ";
        if(position==-1){
            cout<<"Not found"<<endl;
        }else{
            cout<<"Found at index "<<position<<endl;
        }
    }
    return 0;
}
