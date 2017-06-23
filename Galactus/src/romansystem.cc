#include "romansystem.hh"

using namespace galaxy_trade;

RomanSystem::RomanSystem(){
    /// TODO: Initializer list usage.
    m_value_data.insert(RomanSymbol(1000, "M"));
    m_value_data.insert(RomanSymbol(900, "CM"));
    m_value_data.insert(RomanSymbol(500, "D"));
    m_value_data.insert(RomanSymbol(400, "CD"));
    m_value_data.insert(RomanSymbol(100, "C"));
    m_value_data.insert(RomanSymbol( 90, "XC"));
    m_value_data.insert(RomanSymbol( 50, "L"));
    m_value_data.insert(RomanSymbol( 40, "XL"));
    m_value_data.insert(RomanSymbol( 10, "X"));
    m_value_data.insert(RomanSymbol( 9, "IX"));
    m_value_data.insert(RomanSymbol( 5, "V"));
    m_value_data.insert(RomanSymbol( 4, "IV"));
    m_value_data.insert(RomanSymbol( 1, "I"));
    m_value_data.insert(RomanSymbol( 0, "NOT_VALID")); 
    
    m_roman_dictionary['I'] = 1;
    m_roman_dictionary['V'] = 5;
    m_roman_dictionary['X'] = 10;
    m_roman_dictionary['L'] = 50;
    m_roman_dictionary['C'] = 100;
    m_roman_dictionary['D'] = 500;
    m_roman_dictionary['M'] = 1000;
}

std::string RomanSystem::ToRoman(unsigned int value){
    std::string result;
    for(std::set<RomanSymbol,RomanValueComparator>::reverse_iterator itr = m_value_data.rbegin() ; itr->GetValue() > 0 ; ++itr){
        const unsigned int tmp_value = itr->GetValue();
        while(value >= tmp_value){
            result.append(itr->GetSymbol());
            value-= tmp_value;
        }
    }
    return result;
}

unsigned int RomanSystem::ToValue(std::string symbols){
    unsigned int result = 0;
    for (int i = 0; i < (symbols.length() - 1); ++i) 
        unsigned int cur = m_roman_dictionary[symbols[i]];
        unsigned int next = m_roman_dictionary[symbols[i + 1]];
        if(cur >= next){
            result += cur; 
        }else{
            result -= cur;
        }
    }
    return (result + m_roman_dictionary[symbols[symbols.length() - 1]]);
}

bool SymbolChecker(const std::string symbols){

}
