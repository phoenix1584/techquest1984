// Example program
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

void QSort(std::vector<int>& data,int left, int right,long long& count){
				if(left < right){
								count += (right-left - 1 ); 	
								int part = Partition(data,left,right);
								QSort(data,left,part-1,count);
								QSort(data,part+1,right,count);
				}
}

int RPartition(std::vector<int>& data,int left, int right){
				int pivot = data[right];
				int i = left - 1;
				int tmp;
				for(int j = left ; j < right ; ++j){
								if(data[j] <= pivot){
												i++;
												tmp = data[i];
												data[i] = data[j];
												data[j] = tmp;
								}    
				}
				data[right] = data[i+1];
				data[i+1] = pivot;
				return i+1; // New Partition boundary.
}

void RQSort(std::vector<int>& data,int left, int right,long long& count){
				if(left < right){
								count += (right-left - 1); 	
								int part = RPartition(data,left,right);
								RQSort(data,left,part-1,count);
								RQSort(data,part+1,right,count);
				}
}
int Median(const int& left, const int& mid, const int& right){
				return std::max(std::min(left,mid), std::min(std::max(left,mid),right));
} 

int MPartition(std::vector<int>& data,int left, int right){
				int pivot = Median(data[left],data[((right-left)/2)],data[right]);
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

void MQSort(std::vector<int>& data,int left, int right,long long& count){
				if(left < right){
								count += (right-left - 1 ); 	
								int part = MPartition(data,left,right);
								MQSort(data,left,part-1,count);
								MQSort(data,part+1,right,count);
				}
}

int main()
{
				//std::vector<int> data = {3,8,2,5,1,4,7,6};
				//for(const auto& x : data)
				//  std::cout << x << ",";
				{
								std::vector<int> data;
								std::ifstream ifs("qdata.txt");
								int x;
								while(ifs>>x){
												data.push_back(x);
								}
								std::cout << data.size() << "\n";
								long long inv_count = 0;
								QSort(data,0,data.size()-1,inv_count);
								std::cout << "\nCount of comparisons First Pivot : " << inv_count << "\n";
				}

				{
								std::vector<int> data;
								std::ifstream ifs("qdata.txt");
								int x;
								while(ifs>>x){
												data.push_back(x);
								}
								std::cout << data.size() << "\n";
								long long inv_count = 0;
								RQSort(data,0,data.size()-1,inv_count);
								std::cout << "\nCount of comparisons Last Pivot : " << inv_count << "\n";
				}
				{
								std::vector<int> data;
								std::ifstream ifs("qdata.txt");
								int x;
								while(ifs>>x){
												data.push_back(x);
								}
								std::cout << data.size() << "\n";
								long long inv_count = 0;
								MQSort(data,0,data.size()-1,inv_count);
								std::cout << "\nCount of comparisons Median Pivot : " << inv_count << "\n";
				}
}
