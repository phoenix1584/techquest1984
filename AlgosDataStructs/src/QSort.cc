// Example program
#include <iostream>
#include <string>
#include <vector>

int Partition(std::vector<int>& data,int left, int right){
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
    return i-1; // New Partition boundary.
}

void QSort(std::vector<int>& data,int left, int right){
    if(left < right){
        int part = Partition(data,left,right);
        QSort(data,left,part-1);
        QSort(data,part+1,right);
    }
}

int main()
{
  std::vector<int> data = {3,8,2,5,1,4,7,6};
  for(const auto& x : data)
    std::cout << x << ",";
  QSort(data,0,data.size()-1);
  std::cout << "\nPost Partition\n";
  for(const auto& x : data)
    std::cout << x << ",";
  std::cout << "\n";
}
