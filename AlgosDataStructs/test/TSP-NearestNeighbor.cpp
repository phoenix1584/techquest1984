#include <bits/stdc++.h>

class City
{
public:
    double m_x;
    double m_y;

    City(double x, double y)
        :m_x(x),m_y(y)
    {}

    double Dist(City dst)
    {
        double dist = sqrt(pow((m_x-dst.m_x),2) + pow((m_y-dst.m_y),2)) ;
        return dist;
    }

    double SqrDist(City dst)
    {
        return (pow((m_x-dst.m_x),2) + pow((m_y-dst.m_y),2));
    }
};

double FindMinCity(std::vector<City>&  cities, std::vector<int>& visited, int city_id)
{
    double adj[cities.size()];
    adj[city_id] = DBL_MAX;
    for(int x =0 ; x < cities.size() ; x++){
        if(x != city_id){
            adj[x]=cities[city_id].SqrDist(cities[x]);
        }
    }
    int min_city = INT_MAX;
    double min_dist = DBL_MAX;
    for(int j = 0 ; j < cities.size() ; ++ j)
    {
        if(city_id != j && !(visited[j]))
        {
            if(adj[j] < min_dist)
            {
                //std::cout << j <<  "--" << adj[city_id][j] << "::";
                min_city = j;
                min_dist = adj[j];
            }

        }
    }
    return min_city;
}

int main()
{
    //std::ifstream ifs("TestCase.txt");
    std::ifstream ifs("nn_50.txt");
    if(!ifs)
    {
        std::cout << "File not found !\n";
        return 1;
    }

    int N;
    ifs >> N;
    std::vector<int> visited(N,0);
    std::vector<City> cities;
    std::vector<int> tour(N,-1);


    double x,y;
    int id;
    while(ifs >> id >> x >> y )
    {
        cities.push_back(City(x,y));
    }

    std::cout << "Starting the matrix population for city count : " << N << "\n";

    std::cout << "Starting the tour calculations .....\n";
    double min_tour_cost = 0;
    int min_city = 0;
    visited[0] = true;
    for(int i = 0 ; i <  N-1 ; i++)
    {
        //std::cout << "Iteration : " << i << ", SRC : " << min_city << ":";
        min_city = FindMinCity(cities,visited,min_city);
        visited[min_city] = 1;
        tour[i] = min_city;
       //std::cout << ",DST : "  << min_city;
    }

    double tour_dist = 0;
    int src = 0;
    //tour[N] = 0;
    // FIXME : Remember to recalculate the tour distances using Dist instead of SqrDist !!!!!
//    std::ofstream ofs("nn_tsp_results.txt");
//    for(int x = 0; x < N - 1; ++x)
//    {
//        ofs << (src + 1) << "->";
//        double dist = cities[src].Dist(cities[tour[x]]);
//        ofs << dist <<" -> ";
//        tour_dist +=  dist;
//        src = tour[x];
//        ofs  << (tour[x] + 1) << "\n";
//    }
//    double last_hop = cities[tour[N-2]].Dist(cities[0]);
//    tour_dist += last_hop;
//    ofs << tour[N-2] << "->" << last_hop << "-> " << "0\n";

    for(int x = 0; x < N - 1; ++x)
    {
        std::cout << (src + 1) << "->";
        double dist = cities[src].Dist(cities[tour[x]]);
        std::cout << dist <<" -> ";
        tour_dist +=  dist;
        src = tour[x];
        std::cout  << (tour[x] + 1) << "\n";
    }
    double last_hop = cities[tour[N-2]].Dist(cities[0]);
    tour_dist += last_hop;
    std::cout << tour[N-2] + 1 << "->" << (double)last_hop << "-> " << "1\n";

    long long floor_min_path = floor(tour_dist);
    std::cout << "\nMin Tour : " <<  (long long)(floor(tour_dist)) << "\n";
    printf("Min tour : %lld", (long long)(floor(tour_dist)));
    return 0;
}
