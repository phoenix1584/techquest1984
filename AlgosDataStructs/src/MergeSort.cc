// Example program
#include <iostream>
#include <vector>

void Merge(std::vector<int>& sub_data,unsigned int left_index, unsigned int middle_index, unsigned int right_index){
    
       const int l_size = middle_index - left_index + 1;
       const int r_size = right_index - middle_index;
       std::cout << "l_index : " << left_index << ", Right_index : " << right_index << ",Middle_index : " << middle_index << ", l_size : " << l_size << ", r_size " << r_size;
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
         
       std::cout << " Left Data : ";
       for(const auto& x : l_data)
            std::cout << x << ",";
       std::cout << " Right Data : ";
       for(const auto& x : r_data)
            std::cout << x << ",";
        std::cout << "\n";
       

       // Merging the data
       int i = 0, j =0 , k = left_index;
       while(i < l_size && j < r_size){
            sub_data[k++] = (l_data[i] <= r_data[j]) ? l_data[i++] : r_data[j++];
        }
        while (i < l_size)
            sub_data[k++] = l_data[i++];
        while (j < r_size)
            sub_data[k++] = r_data[j++];
}

void MSort(std::vector<int>& sub_data,unsigned int left_index, unsigned int right_index){
    if(left_index < right_index){
        int middle_index = left_index + (right_index-left_index) /2;
        MSort(sub_data,left_index,middle_index);
        MSort(sub_data,middle_index+1,right_index);
        Merge(sub_data,left_index,middle_index,right_index);
    }
    
};

int main()
{
    std::vector<int> data = {38,27,43,3,9,82,10};
    for(const auto& x : data)
        std::cout << x << ",";
    std::cout << "\nStarting the sort ...\n";
    MSort(data,0,data.size()-1);
    for(const auto& x : data)
        std::cout << x << ",";  
    std::cout<< "\nFinished" << std::endl;
}
