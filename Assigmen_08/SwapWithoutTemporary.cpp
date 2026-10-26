#include<iostream>
using namespace std;

void SwapWithoutTemporary(int u, int v){
    cout<<"u= "<<u<<"\tv= "<<v;
    u=u+v;
    v=u-v;
    u=u-v;
    cout<<"\nu= "<<u<<"\tv= "<<v;
}

int main(){
    int u,v;
    cout<<"Write u value\n";
    cin>>u;
    cout<<"Write v value\n";
    cin>>v;
    SwapWithoutTemporary(u,v);
    return 0;
}