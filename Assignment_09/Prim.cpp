#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <limits>
#include <utility>
using namespace std;

pair<vector<int>,vector<double>> Prim(const vector<vector<pair<int,int>>>& G, int start){
    int n=G.size();
    vector<double> key(n);
    vector<int> parent(n);
    vector<bool> inMST(n);

    for(int v=0; v<n; v++){
        key[v]=numeric_limits<double>::infinity();
        parent[v]=-1;
        inMST[v]=false;
    }
    key[start]=0;

    // Each queue entry contains (weight, vertex), smallest weight first.
    priority_queue<pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> Q;
    for(int v=0; v<n; v++){
        Q.push({key[v],v});
    }

    while(!Q.empty()){
        int u=Q.top().second;
        Q.pop();

        if(inMST[u]){
            continue;
        }
        inMST[u]=true;

        for(auto edge:G[u]){
            int v=edge.first;
            int w=edge.second;

            if(!inMST[v] && w<key[v]){
                key[v]=w;
                parent[v]=u;
                Q.push({key[v],v});
            }
        }
    }
    return {parent,key};
}

void addEdge(vector<vector<pair<int,int>>>& G, int u, int v, int w){
    // An undirected edge appears in both adjacency lists.
    G[u].push_back({v,w});
    G[v].push_back({u,w});
}

int main(){
    int n=5;
    vector<vector<pair<int,int>>> G(n);
    addEdge(G,0,1,2);
    addEdge(G,0,3,6);
    addEdge(G,1,2,3);
    addEdge(G,1,3,8);
    addEdge(G,1,4,5);
    addEdge(G,2,4,7);
    addEdge(G,3,4,9);

    cout<<"Adjacency lists (vertex, weight):\n"<<endl;
    for(int u=0; u<n; u++){
        cout<<u<<": ";
        for(auto edge:G[u]){
            cout<<"("<<edge.first<<", "<<edge.second<<") ";
        }
        cout<<endl;
    }

    int start=0;
    auto result=Prim(G,start);
    vector<int> parent=result.first;
    vector<double> key=result.second;
    double totalWeight=0;

    cout<<"\nMinimum Spanning Tree (start = "<<start<<"):\n"<<endl;
    cout<<"Edge\tWeight\n";
    for(int v=0; v<n; v++){
        if(parent[v]!=-1){
            cout<<parent[v]<<" - "<<v<<"\t"<<key[v]<<endl;
            totalWeight=totalWeight+key[v];
        }
    }
    cout<<"\nTotal weight: "<<totalWeight<<endl;
    return 0;
}
