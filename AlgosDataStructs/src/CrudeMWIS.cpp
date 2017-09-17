#include<new>
#include<cstdio>
#include<cctype>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include<sstream>
#include<iostream>
#include<strstream>

// Data Structure Includes

#include<map>
#include<set>
#include<list>
#include<stack>
#include<deque>
#include<queue>
#include<vector>
#include<bitset>
#include<string>
#include<iterator>
#include<algorithm>
#include <fstream>

// Standard Namespace Inclusion

using namespace std;

// Supporting Macros

#define SZ( C )                 ( ( int ) ( ( C ).size() ) )
#define ALL( C )                ( C ).begin() , ( C ).end()
#define TR( C , it )            for( typeof( ( C ).begin() ) it = ( C ).begin(); it != ( C ).end() ; ++it )
#define LN( STRING )            ( ( int ) ( STRING ).length() )
#define SPRESENT( C , x )       ( ( ( C ).find( x ) ) != ( C ).end() )
#define CPRESENT( C , x )       ( find( ALL( C ) , x ) != ( C ).end() )
#define PB                      push_back

// Typedefed Versions of Data Types

typedef vector< int > VI;
typedef vector< VI > VVI;
typedef vector< string > VS;
typedef pair< int ,int > PII;
typedef long long LL;
typedef unsigned long long ULL;

int main(){

    // this variable stores the number of vertices in the path graph
    int nVertices;
    std::ifstream ifs("mwis.txt");
    if(!ifs){
        std::cout << "File not found.\n";
        return 1;
    }
    ifs >> nVertices;

    // this array will store the weights of the respective vertices in the path graph
    int vertexWeight[nVertices + 1];

    cout <<"\nPlease enter the weights assigned to respective vertices in the path graph.\n" << endl;

    vertexWeight[0] = 0;

    for(int i = 1 ; i < nVertices + 1; ++i){
        ifs >> vertexWeight[i];
    }

    // the following code evaluates the maximum weight independent set of the graph using the
    // dynamic programming technique

    // this array will be used to perform the bottom-up computation
    int maxSetComputation[nVertices + 1];
    maxSetComputation[0] = 0;
    maxSetComputation[1] = vertexWeight[1];

    for(int i = 2 ; i < nVertices + 1; ++i){
        maxSetComputation[i] = max( maxSetComputation[i-1] , maxSetComputation[i-2] + vertexWeight[i] );
    }

    cout <<"\nThe weight of the maximum weight independent set of the graph is : " << maxSetComputation[nVertices] << endl << endl;

    // this is the reconstruction algorithm the outputs the vertices that constitute the max-weight independent set
    int i = nVertices;

    cout <<"The vertices that constitute the maximum weight independent set of the path graph are : ";
    std::set<int> vertices;
    while( i > 1 ){
        if( maxSetComputation[i-2] + vertexWeight[i] > maxSetComputation[i-1] ){
            //cout << i <<" , ";
            vertices.insert(i);
            i -=2;

            if( i == 1){
                //cout << 1 << " , ";
                vertices.insert(i);
                break;
            }
        }

        else{
            i--;
        }
    }
    std::vector<int> target = {1, 2, 3, 4, 17, 117, 517, 997};
    for(const auto& x : target){
        std::cout << ((vertices.end() != vertices.find(x)) ? 1 : 0);
    }
    std::cout << "\n";

    return 0;
}
