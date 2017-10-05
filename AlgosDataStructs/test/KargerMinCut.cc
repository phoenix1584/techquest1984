#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>
struct Vertex;

struct Edge {
    int v1;
    int v2;
};

struct Vertex {
  int label;
  // 'squashed' is a list of any vertices squashed into this one during the
  // random contraction
  std::vector<int> squashed;
  std::vector<int> edges;
};

struct Graph {
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    int vertexCount;
};

Graph loadGraph(std::ifstream &infile) {
  Graph g;
  g.vertexCount = 0;

  std::string line;

  while (std::getline(infile, line)) {
    std::stringstream ss(line);
    // make a new Vertex for this line
    Vertex v;
    int vertexNum;
    ss >> vertexNum;
    v.label = vertexNum;
    int edgeNum;
    while (ss >> edgeNum) {
      v.edges.push_back(edgeNum);
      // only record the edge once, so when edgeNum > vertexNum
      if (edgeNum > vertexNum) {
        Edge e;
        e.v1 = vertexNum;
        e.v2 = edgeNum;
        g.edges.push_back(e);
      }
    }
    g.vertices.push_back(v);
    g.vertexCount++;
  }

  return g;
}

void printGraph(Graph g) {
  // first print vertices:
  for (std::vector<Vertex>::iterator it = g.vertices.begin();
       it != g.vertices.end(); it++) {
    std::cout << "Vertex " << it->label << ": ";
    for (std::vector<int>::iterator j = it->edges.begin();
         j != it->edges.end(); j++) {
      std::cout << *j << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "EDGES: ";
  for (std::vector<Edge>::iterator i = g.edges.begin();
       i != g.edges.end(); i++) {
    std::cout << i->v1 << "<=>" << i->v2 << " ";
  }
  std::cout << std::endl;

  std::cout << "Number of vertices " << g.vertices.size() << std::endl;
  std::cout << "Number of edges " << g.edges.size() << std::endl;
}

void randomContraction(Graph &g) {
  // pick a random edge
  Edge e = g.edges[rand() % g.edges.size()];
  //std::cout << "Random edge: " << e.v1 << "<=>" << e.v2 << std::endl;
  // now contract the vertices joined by this edge.  we'll merge in the edges
  // from v2 into v1 and remove v2 from the graph.
  Vertex v1 = g.vertices[e.v1-1];
  Vertex v2 = g.vertices[e.v2-1];

  // build a new list of edges based on the contracted graph
  std::vector<Edge> newEdges;
  for (std::vector<Edge>::iterator i = g.edges.begin(); i < g.edges.end(); i++) {
    Edge newEdge = *i;
    if (newEdge.v1 == v2.label) {
      // re assign edge to point to v1
      newEdge.v1 = v1.label;
    }
    if (newEdge.v2 == v2.label) {
      newEdge.v2 = v1.label;
    }
    // only add it to the new list if it's not a self-loop
    //std::cout << "Reassigning edge " << i->v1 << "<=>" << i->v2 <<" to point to "
    //          << newEdge.v1 << "<=>" << newEdge.v2 << "... ";
    if (newEdge.v1 != newEdge.v2) {
    //  std::cout << " Keeping." << std::endl;
      newEdges.push_back(newEdge);
    } else {
    //  std::cout << " SELF-LOOP." << std::endl;
    }
  }
  g.edges = newEdges;

  // null out the vertex to signify that it's been squashed
  //std::cout << "Squash " << (g.vertices.begin() + e.v2-1)->label <<
  //             " into " << (g.vertices.begin() + e.v1-1)->label << std::endl;
  g.vertices[e.v1-1].squashed.push_back(g.vertices[e.v2-1].label);
  g.vertices[e.v1-1].squashed.insert(g.vertices[e.v1-1].squashed.begin(), v2.squashed.begin(), v2.squashed.end());
  g.vertices[e.v2-1].label = 0;
  g.vertexCount--;
}

// compute the MinCut by making repeated attempts at the random contraction algorithm
int kargerMinCut(Graph &g) {

  // randomContraction has >= P(1/n^2) chance of finding the min cut.  Run it a sufficient
  // number of times so that we have only a 1/n chance of failure.
  int attempts = g.vertices.size() * g.vertices.size() * log(g.vertices.size());
  Graph bestCutGraph;
  int bestCutCount = g.edges.size();

  std::cout << "Making " << attempts << " attempts" << std::endl;

  for (int i = 0; i < attempts; i++) {
    // make a copy of g for contracting
    Graph g2 = g;
    // seed the RNG
    srand(time(0));
    std::cout << "\r" << i;
    // use random contraction to reduce the graph to a cut.
    while (g2.vertexCount > 2) {
      //std::cout << "Vertex count: " << g2.vertexCount << std::endl;
      randomContraction(g2);
    }

    if (g2.edges.size() < bestCutCount) {
      // this is our best so far
      bestCutGraph = g2;
      bestCutCount = g2.edges.size();
      std::cout << std::endl << "New best... " << bestCutCount << std::endl;
    }
  }

  // return the best cut we found
  g = bestCutGraph;
  return bestCutCount;
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: q3 <inputfile>" << std::endl;
    return 1;
  }

  std::ifstream infile(argv[1]);

  if (!infile) {
    std::cout << "Could not read input file: " << std::endl;
    return 1;
  }

  Graph g = loadGraph(infile);

  // attempt to find the min cut
  int minCut = kargerMinCut(g);

  std::cout << "Min cut: " << minCut << std::endl;
  // pick the first edge to tell us which two vertices we're left with
  Vertex v1 = g.vertices[g.edges[0].v1-1];
  Vertex v2 = g.vertices[g.edges[0].v2-1];
  std::cout << "Group 1: " << v1.label;
  for (auto& v : v1.squashed) {
    std::cout << " " << v;
  }
  std::cout << std::endl;
  std::cout << "Group 2: " << v2.label;
  for (auto& v : v2.squashed) {
    std::cout << " " << v;
  }
  std::cout << std::endl;

  return 0;
}
