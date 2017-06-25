#include <iostream>
#include <fstream>

#include "queryparser.hh"
using namespace galaxy_trade;

/// TODO: Use boost option and boost log for cleaner application help and option handling.
int main(int argc,char* argv[]){
    if(argc != 2){
        std::cout << "Usage : galactus <query-file-path>\n";
        return 1;
    }
    try{
        std::ifstream f_queries(argv[1]);
        if(f_queries){
            std::string query;
                QueryParser q_parser;
                while(std::getline(f_queries,query)){
                    try{
                        std::string ret = q_parser.ProcessQuery(query) ;
                        if(ret.size()){
                            std::cout << ret << "\n";
                        }        
                    }catch(galaxy_trade::BaseException& e){
                        std::cout << e.getdisplaymessage() << "\n";
                    }
                }
        }
    }catch(std::exception& e){
        std::cout << "std::exception : " << e.what() << "\n";
    }

    return 0;
}
