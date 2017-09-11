#include<iostream>
#include<fstream>
#include<queue>

// TODO : Implement heap from scratch !!

int main(){
    std::priority_queue<int,std::vector<int>,std::less<int> > q_low;
    std::priority_queue<int,std::vector<int>,std::greater<int> >q_high;
    std::ifstream ifs("heap_data.txt");
    int x;
    int sum_median = 0, median = 0;
    int k = 0;
    while(ifs >> x){
        ++k;
        if(q_low.size() == q_high.size()){
            (x < median) ? q_low.push(x) : q_high.push(x);
        }else{
            if(q_low.size() > q_high.size()){
                if(x < median){
                    q_high.push(q_low.top());
                    q_low.pop();
                    q_low.push(x);
                }else{
                    q_high.push(x); 
                } 
            }
            if(q_low.size() < q_high.size()){
                if(x < median){
                    q_low.push(x);
                }else{
                    q_low.push(q_high.top());
                    q_high.pop();
                    q_high.push(x);
                } 
            }
        }
        if(q_low.size() == q_high.size()){
            median = q_low.top();
        }else{
            if(q_low.size() > q_high.size()){
                median = q_low.top();
            }else{
                median = q_high.top();
            }
        }
        sum_median += median;
    }
    std::cout << (sum_median%10000) << "\n";
}
