// Example program
#include <iostream>
#include <vector>

void Merge(std::vector<int>& sub_data,unsigned int left_index, unsigned int middle_index, unsigned int right_index){
       const int l_size = middle_index - left_index + 1;
       const int r_size = right_index - middle_index;
       std::vector<int> l_data(l_size);
       std::vector<int> r_data(r_size);
       std::copy(sub_data.begin()+left_index,sub_data.begin()+left_index+l_size,l_data.begin());
       std::copy(sub_data.begin()+middle_index+1,sub_data.begin()+middle_index + 1 + r_size,l_data.begin());
       
       // Merging the data
       int i = 0, j =0 , k = left_index;
       while(i < l_size && j < r_size){
            if(l_data[i] < r_data[j]){
                sub_data[k] = l_data[i];
                i++;
            }else{
                sub_data[k] = r_data[j];
            }
            k++;
        }
}

void MSort(std::vector<int>& sub_data,unsigned int left_index, unsigned int right_index){
    if(left_index < right_index){
        int middle_index = (left_index + right_index)/2;
        MSort(sub_data,left_index,middle_index);
        MSort(sub_data,middle_index+1,right_index);
        Merge(sub_data,left_index,middle_index,right_index);
    }
    
};

int main()
{
    std::vector<int> data = {10,5,6,1,4};
    for(const auto& x : data)
        std::cout << x << ",";
    std::cout << "\nStarting the sort ...\n";
    MSort(data,0,data.size());
    for(const auto& x : data)
        std::cout << x << ",";
}
