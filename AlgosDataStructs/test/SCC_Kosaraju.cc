// C++ Implementation of Kosaraju's algorithm
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

// #FIXME : Could bools be replaced by bitset ?? Will they save space ??

class Graph
{
				int m_vertices; // No. of vertices
				std::list<int> *adj; // An array of adjacency lists
				std::vector<int> m_scc;				

				// Fills Stack with vertices (in increasing order of finishing
				// times). The top element of stack has the maximum finishing 
				// time
				void fillOrder(int v, bool visited[], std::stack<int> &Stack);

				// A recursive function to print DFS starting from v
				void DFSUtil(int v, bool visited[],int& v_count);
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
void Graph::DFSUtil(int v, bool visited[],int& v_count)
{
	// Mark the current node as visited and print it
	visited[v] = true;
	//std::cout << v << " ";
  ++v_count;

	// Recurse for all the vertices adjacent to this vertex
	for (auto& i : adj[v]){
		if (!visited[i])
			DFSUtil(i, visited,v_count);
	}
}

Graph Graph::getTranspose()
{
	Graph g(m_vertices);
	for (int v = 0; v < m_vertices; v++){
		// Recurse for all the vertices adjacent to this vertex
		for(auto i : adj[v]){
			g.adj[i].push_back(v);
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

	// Recurse for all the vertices adjacent to this vertex
	for(auto i : adj[v]){
		if(!visited[i])
			fillOrder(i, visited, Stack);
	}

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
		int v_count = 0;
		// Print Strongly connected component of the popped vertex
		if (visited[v] == false)
		{
			gr.DFSUtil(v, visited,v_count);
			m_scc.push_back(v_count);
			//std::cout << std::endl;
		}
	}
  m_scc.erase(m_scc.end()-1);
	std::cout << "SCC Counts descending order. Total : " << m_scc.size() << "\n";
  std::sort(m_scc.rbegin(),m_scc.rend());
  for(auto i : m_scc)
		std::cout << i << "\n";
}

// Driver program to test above functions
int main(int argc, char * argv[])
{
	if(argc != 3){
		std::cout << "Usage : ./SCC_Kosaraju <input_file_name> <Number of vertices> \n"; 
		return 1;
	}
	std::ifstream ifs(argv[1]);
	int v,w;
	Graph g(std::stoi(std::string(argv[2])));
  if(ifs){
		while(ifs >> v >> w){
				g.addEdge(v,w);	
		}
	}
  
	// TEST DATA FOR DEBUGGING : Answer : 3,3,3 
	//Graph g(10);
	//g.addEdge(1,4);
	//g.addEdge(2,8);
	//g.addEdge(3,6);
	//g.addEdge(4,7);
	//g.addEdge(5,2);
	//g.addEdge(6,9);
	//g.addEdge(7,1);
	//g.addEdge(8,5);
	//g.addEdge(8,6);
	//g.addEdge(9,7);
	//g.addEdge(9,3);

	std::cout << "Following are strongly connected components in given graph \n";
	g.printSCCs();
	return 0;
}
