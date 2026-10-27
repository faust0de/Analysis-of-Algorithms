#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <limits>
#include <utility>
using namespace std;

// Assumes nonnegative edge weights and a valid source vertex.
pair<vector<double>,vector<int>> Dijkstra(const vector<vector<pair<int,int>>>& G, int source){
    int n=G.size();
    vector<double> dist(n);
    vector<int> parent(n);

    for(int v=0; v<n; v++){
        dist[v]=numeric_limits<double>::infinity();
        parent[v]=-1;
    }
    dist[source]=0;

    // Each entry contains (distance, vertex), smallest distance first.
    priority_queue<pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> Q;
    Q.push({0,source});

    while(!Q.empty()){
        double distance=Q.top().first;
        int u=Q.top().second;
        Q.pop();

        if(distance>dist[u]){
            continue;
        }

        for(auto edge:G[u]){
            int v=edge.first;
            int w=edge.second;

            if(dist[u]+w<dist[v]){
                dist[v]=dist[u]+w;
                parent[v]=u;
                Q.push({dist[v],v});
            }
        }
    }
    return {dist,parent};
}

void addEdge(vector<vector<pair<int,int>>>& G, int u, int v, int w){
    // An undirected edge appears in both adjacency lists.
    G[u].push_back({v,w});
    G[v].push_back({u,w});
}

int main(){
    int n=5;
    vector<vector<pair<int,int>>> G(n);
    addEdge(G,0,1,10);
    addEdge(G,0,2,3);
    addEdge(G,1,2,1);
    addEdge(G,1,3,2);
    addEdge(G,2,3,8);
    addEdge(G,2,4,2);
    addEdge(G,3,4,7);

    cout<<"Adjacency lists (vertex, weight):\n"<<endl;
    for(int u=0; u<n; u++){
        cout<<u<<": ";
        for(auto edge:G[u]){
            cout<<"("<<edge.first<<", "<<edge.second<<") ";
        }
        cout<<endl;
    }

    int source=0;
    auto result=Dijkstra(G,source);
    vector<double> dist=result.first;
    vector<int> parent=result.second;

    cout<<"\nShortest distances from source "<<source<<":\n"<<endl;
    cout<<"Vertex\tDistance\tParent\n";
    for(int v=0; v<n; v++){
        cout<<v<<"\t"<<dist[v]<<"\t\t"<<parent[v]<<endl;
    }
    return 0;
}
