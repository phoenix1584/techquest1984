#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <unordered_set>

int main()
{
    std::ifstream ifs("data.txt");
    if(ifs){
        long long num;
        std::unordered_set<long long> data;
        while(ifs >> num){
            data.insert(num);
        }
        std::cout << "Data Size : " << data.size() << "\n";

        int count = 0;
        for(int sum = -10000 ; sum <= 10000 ; ++sum){
           for(const auto& x : data){
                const auto y = (sum-x);
                if( x != y){
                    if(data.end() != data.find(y)){
                        ++count;
                        break;
                    }
                }
            }
        }
        std::cout << " Final Count : " << count << "\n";
    }else{
        std::cout << "ERROR: Data file not found.\n";
    }
    return 0;
}
