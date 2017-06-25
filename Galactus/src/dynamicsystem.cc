#include "dynamicsystem.hh"
#include <regex>
#ifdef LIB_DEBUG
#include <iostream>
#endif

using namespace galaxy_trade;

void DynamicNumberSystem::AddSymbol(std::string dyn_symbol,std::string roman_symbol){
#ifdef LIB_DEBUG
    std::cout << __func__ << "\n";
#endif
    if(IsRoman(roman_symbol) && dyn_symbol.size()){
       std::transform(dyn_symbol.begin(),dyn_symbol.end(),dyn_symbol.begin(),toupper);
       std::transform(roman_symbol.begin(),roman_symbol.end(),roman_symbol.begin(),toupper);
       m_dyn_dictionary.insert(std::pair<std::string,std::string>(dyn_symbol,roman_symbol)); 
    }else{
        throw DynamicDataException("Invalid data submitted for addition to dynamic numer system. Empty symbol or invalid roman symbol.");
    }
}

bool DynamicNumberSystem::IsValidSymbol(const std::string s){
    return (m_dyn_dictionary.end() != m_dyn_dictionary.find(s));
}

unsigned int DynamicNumberSystem::ToValue(std::string sym){
    return RomanSystem::ToValue(ToRoman(sym));
}

std::string DynamicNumberSystem::ToRoman(std::string sym){
    sym = std::regex_replace(sym, std::regex("^ +| +$|( ) +"), "$1");
    std::transform(sym.begin(),sym.end(),sym.begin(),toupper);
    std::vector<std::string> split_data;
    boost::split(split_data,sym,boost::is_any_of(" "));    
    std::string roman;
    for(const auto& d : split_data){ 
        std::map<std::string,std::string>::iterator itr = m_dyn_dictionary.find(d);
        if(itr != m_dyn_dictionary.end()){
            roman.append(itr->second);
        }else{
            throw DynamicDataException("Unkown dynamic symbol.");
        }
    }
    return roman;
}
