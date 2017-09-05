// C++ Implementation of Kosaraju's algorithm
#include <iostream>
#include <list>
#include <stack>
//using namespace std;

class Graph
{
				int m_vertices; // No. of vertices
				std::list<int> *adj; // An array of adjacency lists

				// Fills Stack with vertices (in increasing order of finishing
				// times). The top element of stack has the maximum finishing 
				// time
				void fillOrder(int v, bool visited[], std::stack<int> &Stack);

				// A recursive function to print DFS starting from v
				void DFSUtil(int v, bool visited[]);
	public:
				Graph(int V);
				void addEdge(int v, int w);

				// The main function that finds and prints strongly connected
				// components
				void printSCCs();

				// Function that returns reverse (or transpose) of this graph
				Graph getTranspose();
};

Graph::Graph(int V)
				:m_vertices(V){
	adj = new std::list<int>[m_vertices];
}

// A recursive function to print DFS starting from v
void Graph::DFSUtil(int v, bool visited[])
{
	// Mark the current node as visited and print it
	visited[v] = true;
	std::cout << v << " ";

	// Recur for all the vertices adjacent to this vertex
	for (std::list<int>::iterator i = adj[v].begin(); i != adj[v].end(); ++i){
		if (!visited[*i])
			DFSUtil(*i, visited);
	}
}

Graph Graph::getTranspose()
{
	Graph g(m_vertices);
	for (int v = 0; v < m_vertices; v++)
	{
		// Recur for all the vertices adjacent to this vertex
		for(std::list<int>::iterator i = adj[v].begin(); i != adj[v].end(); ++i){
			g.adj[*i].push_back(v);
		}
	}
	return g;
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w); // Add w to v.s list.
}

void Graph::fillOrder(int v, bool visited[], std::stack<int> &Stack)
{
	// Mark the current node as visited and print it
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	for(std::list<int>::iterator i = adj[v].begin(); i != adj[v].end(); ++i)
		if(!visited[*i])
			fillOrder(*i, visited, Stack);

	// All vertices reachable from v are processed by now, push v 
	Stack.push(v);
}

// The main function that finds and prints all strongly connected 
// components
void Graph::printSCCs()
{
	std::stack<int> Stack;

	// Mark all the vertices as not visited (For first DFS)
	bool *visited = new bool[m_vertices];
	for(int i = 0; i < m_vertices; i++)
		visited[i] = false;

	// Fill vertices in stack according to their finishing times
	for(int i = 0; i < m_vertices; i++)
		if(visited[i] == false)
			fillOrder(i, visited, Stack);

	// Create a reversed graph
	Graph gr = getTranspose();

	// Mark all the vertices as not visited (For second DFS)
	for(int i = 0; i < m_vertices; i++)
		visited[i] = false;

	// Now process all vertices in order defined by Stack
	while (Stack.empty() == false)
	{
		// Pop a vertex from stack
		int v = Stack.top();
		Stack.pop();

		// Print Strongly connected component of the popped vertex
		if (visited[v] == false)
		{
			gr.DFSUtil(v, visited);
			std::cout << std::endl;
		}
	}
}

// Driver program to test above functions
int main()
{
	// Create a graph given in the above diagram
	Graph g(11);
	g.addEdge(1,4);
	g.addEdge(2,8);
	g.addEdge(3,6);
	g.addEdge(4,7);
	g.addEdge(5,2);
	g.addEdge(6,9);
	g.addEdge(7,1);
	g.addEdge(8,5);
	g.addEdge(8,6);
	g.addEdge(9,7);
	g.addEdge(9,3);


	std::cout << "Following are strongly connected components in "
		"given graph \n";
	g.printSCCs();

	return 0;
}
