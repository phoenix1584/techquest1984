// Example program
#include <iostream>
#include <vector>
#include <fstream>

void Merge(std::vector<int>& sub_data,unsigned int left_index, unsigned int middle_index, unsigned int right_index, long long& inv_count){
    
       const int l_size = middle_index - left_index + 1;
       const int r_size = right_index - middle_index;
       std::vector<int> l_data,r_data;
       std::vector<int>::iterator itr = sub_data.begin()+left_index;
       for(int x = 0; x < l_size ; ++x){
            l_data.push_back(*itr); 
            ++itr;
        }
       itr = sub_data.begin()+ middle_index + 1;
       for(int x = 0; x < r_size ; ++x){
            r_data.push_back(*itr); 
            ++itr;
        }
         
       // Merging the data
       int i = 0, j =0 , k = left_index;
       while(i < l_size && j < r_size){
           if(l_data[i] <= r_data[j]){
            sub_data[k++] = l_data[i++];
           }else {
            sub_data[k++] = r_data[j++];
            inv_count += (l_size - i);
           }
        }
        while (i < l_size)
            sub_data[k++] = l_data[i++];
        while (j < r_size)
            sub_data[k++] = r_data[j++];
}

void MSort(std::vector<int>& sub_data,unsigned int left_index, unsigned int right_index, long long& inv_count){
    if(left_index < right_index){
        int middle_index = left_index + (right_index-left_index) /2;
        MSort(sub_data,left_index,middle_index,inv_count);
        MSort(sub_data,middle_index+1,right_index,inv_count);
        Merge(sub_data,left_index,middle_index,right_index,inv_count);
    }
    
};

int main()
{
   // Test Data for sanity.
   //std::vector<int> data = {38,27,43,3,9,82,10};
   //for(const auto& x : data)
   //    std::cout << x << ",";
   //std::cout << "\nStarting the sort ...\n";
   
   std::vector<int> data;
   std::ifstream ifs("IntegerArray.txt");
   int x;
   while(ifs>>x){
      data.push_back(x);
   }
    std::cout << data.size();
    long long inv_count = 0;
    MSort(data,0,data.size()-1,inv_count);
    //for(const auto& x : data)
    //    std::cout << x << ",";  
    std::cout<< "\nFinished : Inversions " << inv_count << std::endl;
}
