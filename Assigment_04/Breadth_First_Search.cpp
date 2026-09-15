#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/*Breadth_First_Search algorithm to go to all graph nodes in level order*/

void bfs(int start, const vector<vector<int>>& graph, vector<bool>& visited){
    queue<int> q; //Queue of nodes

    visited[start]=true; //Mark as visited the first node

    q.push(start); //Save the first node in queue
    
    while(!q.empty()){ // While queue is not empty
      int node=q.front(); // Save the node from the front of queue
      q.pop(); // Takes out the first element from queue
      cout<< node << " "; //Print the node
      for(int neighbor : graph[node]){ // Inspects node's neighbors
        if (!visited[neighbor]){ // Checks if neighbor was visit
            visited[neighbor]=true; // Marks neighbor as visited
            q.push(neighbor); // Enqueues neighbor
        }
      }
    }

    //Check if there are disconnected nodes
    for(int i=0; i< visited.size(); i++){ 
        if(!visited[i]){
            bfs(i,graph,visited);
            break;
        }
    }
}

/*Main function for testing bfs function*/

int main(){

    vector<vector<int>> graph = {
    {1, 2},   // Node 0
    {0, 3},   // Node 1
    {0},      // Node 2
    {1},      // Node 3
    {}        // Node 4
    };
    
    vector<bool> visited(graph.size(), false); 

    cout<<"Testing BFS: \n"<<endl;
    bfs(0,graph,visited);

    return 0;
}