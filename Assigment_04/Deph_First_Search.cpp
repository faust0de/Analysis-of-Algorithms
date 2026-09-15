#include <iostream>
#include <vector>

using namespace std;

void dfs(int node, const vector<vector<int>>& graph, vector<bool>& visited){
    visited[node]= true;
    cout << node << " ";
    for(int neighbor : graph[node]){
        if(!visited[neighbor]){
            dfs(neighbor, graph, visited);
        }
    }
}

int main(){
    vector<vector<int>> graph = {
        {1, 2},    // Neighbors of node 0
        {0, 3, 4}, // Neighbors of node 1
        {0},       // Neighbors of node 2
        {1},       // Neighbors of node 3
        {1}        // Neighbors of node 4
    };

    vector<bool> visited(graph.size(), false);
    cout << "DFS starting from node 0: ";
    dfs(0, graph, visited);
    return 0;
}