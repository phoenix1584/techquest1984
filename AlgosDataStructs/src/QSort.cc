// Example program
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

void Printer(std::vector<int>& data,std::string comment= "None"){
#ifndef DEBUG
	for(const auto& x : data)
		std::cout << x << ",";
	std::cout << "\t======== Comment : " << comment << "\n";
#endif
}

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
		count += (right-left + 1 ); 	
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
	Printer(data,std::to_string(pivot));
	return i+1; // New Partition boundary.
}

void RQSort(std::vector<int>& data,int left, int right,long long& count){
	if(left < right){
		count += (right-left); 	
		int part = RPartition(data,left,right);
		RQSort(data,left,part-1,count);
		RQSort(data,part+1,right,count);
	}
}

int MPartition(std::vector<int>& data,int left, int right){
	int mid = (left + (right-left)/2);
	int tmp;
        if((data[mid] > data[left] && data[mid] < data[right]) || 
	    (data[mid] < data[left] && data[mid] > data[right]) ){
		tmp=data[left];
		data[left]=data[mid];
		data[mid]=tmp;
	}
	else if((data[right] > data[left] && data[right] < data[mid]) || 
	    (data[right] < data[left] && data[right] > data[mid]) ){
		tmp=data[left];
		data[left]=data[right];
		data[right]=tmp;
	}
	int pivot = data[left];

	int i = left + 1;
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
		count += (right-left - 1); 	
		int part = MPartition(data,left,right);
		MQSort(data,left,part-1,count);
		MQSort(data,part+1,right,count);
	}
}

int main()
{
	//std::vector<int> data = {3,8,2,5,1,4,7,6};
	{
		std::vector<int> data;
		std::ifstream ifs("qdata.txt");
		int x;
		while(ifs>>x){
			data.push_back(x);
		}
		Printer(data,"Unsorted");
		long long inv_count = 0;
		QSort(data,0,data.size(),inv_count);
		Printer(data,"Sorted");
		std::cout << "Count of comparisons First Pivot : " << inv_count << "\n\n";
	}
	{
		std::vector<int> data;
		std::ifstream ifs("qdata.txt");
		int x;
		while(ifs>>x){
			data.push_back(x);
		}
		Printer(data,"Unsorted");
		long long inv_count = 0;
		RQSort(data,0,data.size()-1,inv_count);
		Printer(data,"Sorted");
		std::cout << "Count of comparisons Last Pivot : " << inv_count << "\n\n";
	}
	{
		std::vector<int> data;
		std::ifstream ifs("qdata.txt");
		int x;
		while(ifs>>x){
			data.push_back(x);
		}
		Printer(data,"Unsorted");
		long long inv_count = 0;
		MQSort(data,0,data.size(),inv_count);
		Printer(data,"Sorted");
		std::cout << "Count of comparisons Median Pivot : " << inv_count << "\n\n";
	}
}
