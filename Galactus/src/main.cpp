#include <iostream>
#include <fstream>

#include "romansystem.hh"

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
            while(std::getline(f_queries,query)){
                std::cout << query << "\n";        
            } 
            std::cout << "Galactus signing off gracefully." << std::endl;
        }
    }catch(galaxy_trade::BaseException& e){
        std::cout << "Galactic Exception : " << e.what() << "\n";
    }catch(std::exception& e){
        std::cout << "std::exception : " << e.what() << "\n";
    }

    return 0;
}
