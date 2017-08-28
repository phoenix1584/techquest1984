// Example program
#include <iostream>
#include <string>
#include <vector>

void Partition(std::vector<int>& data,unsigned int left, unsigned int right){
    if(left < right){
     int pivot = data[left];
     int i = left + 1;
     int tmp;
     for(int j = left + 1 ; j < right ; ++j){
        if(data[j] < pivot){
            tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
            i++;
        }    
     }
     data[left] = data[i-1];
     data[i-1] = pivot;
     Partition(data,i,right);
    }
}

int main()
{
  std::vector<int> data = {3,8,2,5,1,4,7,6};
  for(const auto& x : data)
    std::cout << x << ",";
  Partition(data,0,data.size());
  std::cout << "\nPost Partition\n";
  for(const auto& x : data)
    std::cout << x << ",";
}
