// C++ program for Kruskal's algorithm to find Minimum
// Spanning Tree of a given connected, undirected and
// weighted graph
#include<bits/stdc++.h>
using namespace std;

// Creating shortcut for an integer pair
typedef  pair<int, int> iPair;

// Structure to represent a graph
struct Graph
{
    int V, E;
    vector< pair<int, iPair> > edges;

    // Constructor
    Graph(int V, int E)
    {
        this->V = V;
        this->E = E;
    }

    // Utility function to add an edge
    void addEdge(int u, int v, int w)
    {
        edges.push_back({w, {u, v}});
    }

    // Function to find MST using Kruskal's
    // MST algorithm
    int kruskalMST();
};

// To represent Disjoint Sets
struct DisjointSets
{
    int *parent, *rnk;
    int n;
    int m_cluster_count;

    // Constructor.
    DisjointSets(int n)
    {
        // Allocate memory
        this->n = n;
        m_cluster_count = n;
        parent = new int[n+1];
        rnk = new int[n+1];

        // Initially, all vertices are in
        // different sets and have rank 0.
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;

            //every element is parent of itself
            parent[i] = i;
        }
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

 /* Functions returns weight of the MST*/

int Graph::kruskalMST()
{
    int mst_wt = 0; // Initialize result

    // Sort edges in increasing order on basis of cost
    sort(edges.begin(), edges.end());

    // Create disjoint sets
    DisjointSets ds(V);

    // Iterate through all sorted edges
    vector< pair<int, iPair> >::iterator it;
    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // Check if the selected edge is creating
        // a cycle or not (Cycle is created if u
        // and v belong to same set)
        if (set_u != set_v)
        {
            // Current edge will be in the MST
            // so print it
            //cout << u << " - " << v << endl;

            // Update MST weight
            mst_wt += it->first;

            // Merge two sets
            ds.merge(set_u, set_v);
            --ds.m_cluster_count;
            if(ds.m_cluster_count == 3){
                return it->first;
            }
        }
    }
    return mst_wt;
}

// function to calculate Hamming distance
int hammingDist(const std::string& str1, const std::string& str2)
{
    int i = 0, count = 0;
    while (str1[i] != '\0')
    {
        if (str1[i] != str2[i])
            count++;
        i++;
    }
    return count;
}

void HammingNeighbors(std::string& str, int i, int changesLeft,std::vector<std::string>& hamm_nbors) {
        if (changesLeft == 0) {
                hamm_nbors.push_back(str);
                return;
        }
        if (i < 0) return;
        // flip current bit
        str[i] = str[i] == '0' ? '1' : '0';
        HammingNeighbors(str, i-1, changesLeft-1,hamm_nbors);
        // or don't flip it (flip it again to undo)
        str[i] = str[i] == '0' ? '1' : '0';
        HammingNeighbors(str, i-1, changesLeft,hamm_nbors);
}

int MinClusterCreate(std::unordered_map<std::string,int>& nodes_hash,int V_size,int Bit_Size,std::vector<std::string>& data){
    DisjointSets ds(V_size);
    int node_count = 0;
    for(const auto& x : data){
        std::vector<std::string> hamm_nbors;
        for(int i = 1 ; i < 3 ; ++i){
            std::string str = x;
            HammingNeighbors(str,Bit_Size,i,hamm_nbors);
        }
        for(const auto& h : hamm_nbors){
            auto idx = nodes_hash.find(h);
            if(nodes_hash.end() != idx){
                int u = node_count;
                int v = idx->second;
                int set_u = ds.find(u);
                int set_v = ds.find(v);

                // Check if the selected edge is creating
                // a cycle or not (Cycle is created if u
                // and v belong to same set)
                if (set_u != set_v)
                {
                    // Merge two sets
                    ds.merge(set_u, set_v);
                    --ds.m_cluster_count;
                }
            }
        }
        ++node_count;
    }
    return ds.m_cluster_count;
}

// Driver program to test above functions
int main()
{
    //std::ifstream ifs("clustering2-example-200-12-solution-4.txt");
    //std::ifstream ifs("clustering2-example-200000-24-solution-6247.txt");
    std::ifstream ifs("clustering_big.txt");
    if(!ifs){
        std::cout << "File Not found.\n";
        return 1;
    }
    int V, B;
    std::string init_data;
    std::getline(ifs,init_data);
    std::stringstream ss(init_data);
    ss >> V >> B;
    //Graph g(V, 0);

    std::cout << "Nodes : " << V << ", Bits " << B << "\n";
    std::string bit_data;
    std::unordered_map<std::string,int> input;
    std::vector<std::string> data;
    int node_id = 0;
    while(std::getline(ifs,bit_data)){
        bit_data.erase(std::remove_if(bit_data.begin(), bit_data.end(),[](char x){return std::isspace(x);}), bit_data.end());
        input.insert(std::make_pair(bit_data,node_id));
        data.push_back(bit_data);
        bit_data.clear();
        ++node_id;
    }
    std::cout << input.size() << "," << data.size() << "\n";
    std::cout << MinClusterCreate(input,V,B,data);

    return 0;
}
