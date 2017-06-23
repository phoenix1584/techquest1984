#include "dynamicsystem.hh"
using namespace galaxy_trade;

void DynamicNumberSystem::AddSymbol(std::string dyn_symbol,std::string roman_symbol){
    if(IsRoman(roman_symbol) && dyn_symbol.size()){
       m_dyn_dictionary.insert(std::pair<std::string,std::string>(dyn_symbol,roman_symbol)); 
    }else{
        throw DynamicDataException("Invalid data submitted for addition to dynamic numer system. Empty symbol or invalid roman symbol.");
    }
}

bool DynamicNumberSystem::IsValidSymbol(const std::string s){
    return (m_dyn_dictionary.end() != m_dyn_dictionary.find(s));
}

unsigned int DynamicNumberSystem::ToValue(std::string sym){
    std::map<std::string,std::string>::iterator itr = m_dyn_dictionary.find(sym);
    if(itr != m_dyn_dictionary.end()){
        return RomanSystem::ToValue(itr->second);
    }else{
        throw DynamicDataException("Unkown dynamic symbol.");
    }
}

std::string DynamicNumberSystem::ToRoman(std::string sym){
    std::map<std::string,std::string>::iterator itr = m_dyn_dictionary.find(sym);
    if(itr != m_dyn_dictionary.end()){
        return itr->second;
    }else{
        throw DynamicDataException("Unkown dynamic symbol.");
    }
}
