/*
* The file contains an adjacency list representation of an undirected weighted graph with 200 vertices labeled 1 to 200. Each row consists of the node tuples that are adjacent to that particular vertex along with the length of that edge. For example, the 6th row has 6 as the first entry indicating that this row corresponds to the vertex labeled 6. The next entry of this row "141,8200" indicates that there is an edge between vertex 6 and vertex 141 that has length 8200. The rest of the pairs of this row indicate the other vertices adjacent to vertex 6 and the lengths of the corresponding edges.
* 
* Your task is to run Dijkstra's shortest-path algorithm on this graph, using 1 (the first vertex) as the source vertex, and to compute the shortest-path distances between 1 and every other vertex of the graph. If there is no path between a vertex v and vertex 1, we'll define the shortest-path distance between 1 and v to be 1000000.
* 
* You should report the shortest-path distances to the following ten vertices, in order: 7,37,59,82,99,115,133,165,188,197. You should encode the distances as a comma-separated string of integers. So if you find that all ten of these vertices except 115 are at distance 1000 away from vertex 1 and 115 is 2000 distance away, then your answer should be 1000,1000,1000,1000,1000,2000,1000,1000,1000,1000. Remember the order of reporting DOES MATTER, and the string should be in the same order in which the above ten vertices are given. The string should not contain any spaces. Please type your answer in the space provided.
* 
* IMPLEMENTATION NOTES: This graph is small enough that the straightforward O(mn) time implementation of Dijkstra's algorithm should work fine. OPTIONAL: For those of you seeking an additional challenge, try implementing the heap-based version. Note this requires a heap that supports deletions, and you'll probably need to maintain some kind of mapping between vertices and their positions in the heap.
*/

// Program to find Dijkstra's shortest path using STL set
#include<bits/stdc++.h>
using namespace std;
# define INF 1000000
 
// This class represents a directed graph using 
// adjacency list representation
class Graph
{
    int V;    // No. of vertices
 
    // In a weighted graph, we need to store vertex 
    // and weight pair for every edge
    list< pair<int, int> > *adj;
 
public:
    Graph(int V);  // Constructor
 
    // function to add an edge to graph
    void addEdge(int u, int v, int w);
 
    // prints shortest path from s
    void shortestPath(int s);
};
 
// Allocates memory for adjacency list
Graph::Graph(int V)
{
    this->V = V;
    adj = new list< pair<int, int> >[V];
}
 
void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}
 
// Prints shortest paths from src to all other vertices
void Graph::shortestPath(int src)
{
    // Create a set to store vertices that are being
    // prerocessed
    set< pair<int, int> > setds;
 
    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<int> dist(V, INF);
 
    // Insert source itself in Set and initialize its
    // distance as 0.
    setds.insert(make_pair(0, src));
    dist[src] = 0;
 
    /* Looping till all shortest distance are finalized
       then setds will become empty */
    while (!setds.empty())
    {
        // The first vertex in Set is the minimum distance
        // vertex, extract it from set.
        pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());
 
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = tmp.second;
 
        // 'i' is used to get all adjacent vertices of a vertex
        list< pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = (*i).first;
            int weight = (*i).second;
 
            //  If there is shorter path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                /*  If distance of v is not INF then it must be in
                    our set, so removing it and inserting again
                    with updated less distance.  
                    Note : We extract only those vertices from Set
                    for which distance is finalized. So for them, 
                    we would never reach here.  */
                if (dist[v] != INF)
                    setds.erase(setds.find(make_pair(dist[v], v)));
 
                // Updating distance of v
                dist[v] = dist[u] + weight;
                setds.insert(make_pair(dist[v], v));
            }
        }
    }
 
    // Print shortest distances stored in dist[]
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; ++i){
        printf("%d \t\t %d\n", i, dist[i]);
    }
}
 
// Driver program to test methods of graph class
int main()
{
    // create the graph given in above fugure
    int V = 201;
    Graph g(V);
 
    // TEST DATA
    //  making above shown graph
    //g.addEdge(0, 1, 4);
    //g.addEdge(0, 7, 8);
    //g.addEdge(1, 2, 8);
    //g.addEdge(1, 7, 11);
    //g.addEdge(2, 3, 7);
    //g.addEdge(2, 8, 2);
    //g.addEdge(2, 5, 4);
    //g.addEdge(3, 4, 9);
    //g.addEdge(3, 5, 14);
    //g.addEdge(4, 5, 10);
    //g.addEdge(5, 6, 2);
    //g.addEdge(6, 7, 1);
    //g.addEdge(6, 8, 6);
    //g.addEdge(7, 8, 7);
 
    //g.shortestPath(0);
 
    std::ifstream ifs("dijkstraData.txt");
    if(ifs){
        std::string data;
        while(std::getline(ifs,data)){
            std::istringstream ss(data);
            std::string item;
            std::vector<std::string> elem;
            while(ss >> item){
                elem.push_back(std::move(item));
            }
            int v_id = std::stoi(elem[0]);
            for(std::vector<std::string>::iterator itr = elem.begin() + 1 ; itr != elem.end() ; ++itr){
                std::vector<int> edge;
                std::istringstream ss(*itr);
                while (std::getline(ss,item,',')){
                    edge.push_back(std::stoi(item));    
                }
                g.addEdge(v_id, edge[0], edge[1]);
            }
        }
        g.shortestPath(1);
    }else{
        std::cout << "File not found.\n";
    }
    return 0;
}
