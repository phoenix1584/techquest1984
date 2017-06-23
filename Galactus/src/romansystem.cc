#include <algorithm> 
#include "romansystem.hh"

using namespace galaxy_trade;

RomanSystem::RomanSystem(){
    /// TODO: Initializer list usage.
	/// OR Use boost assign
    m_value_data.emplace(RomanSymbol(1000, "M"));
    m_value_data.emplace(RomanSymbol(900, "CM"));
    m_value_data.emplace(RomanSymbol(500, "D"));
    m_value_data.emplace(RomanSymbol(400, "CD"));
    m_value_data.emplace(RomanSymbol(100, "C"));
    m_value_data.emplace(RomanSymbol( 90, "XC"));
    m_value_data.emplace(RomanSymbol( 50, "L"));
    m_value_data.emplace(RomanSymbol( 40, "XL"));
    m_value_data.emplace(RomanSymbol( 10, "X"));
    m_value_data.emplace(RomanSymbol( 9, "IX"));
    m_value_data.emplace(RomanSymbol( 5, "V"));
    m_value_data.emplace(RomanSymbol( 4, "IV"));
    m_value_data.emplace(RomanSymbol( 1, "I"));
    m_value_data.emplace(RomanSymbol( 0, "NOT_VALID")); 
    
    m_roman_dictionary['I'] = 1;
    m_roman_dictionary['V'] = 5;
    m_roman_dictionary['X'] = 10;
    m_roman_dictionary['L'] = 50;
    m_roman_dictionary['C'] = 100;
    m_roman_dictionary['D'] = 500;
    m_roman_dictionary['M'] = 1000;
	
	// Based on rules provided in problem statement.
 	// FIXME: Has to be a better way to create this information on the fly.
	m_blacklist.emplace("IL");
	m_blacklist.emplace("IC");
	m_blacklist.emplace("ID");
	m_blacklist.emplace("IM");
	m_blacklist.emplace("XD");
	m_blacklist.emplace("XM");
	m_blacklist.emplace("VX");
	m_blacklist.emplace("VL");
	m_blacklist.emplace("VC");
	m_blacklist.emplace("VD");
	m_blacklist.emplace("VM");
	m_blacklist.emplace("LC");
	m_blacklist.emplace("LD");
	m_blacklist.emplace("LM");
	m_blacklist.emplace("DM");
	m_blacklist.emplace("DD");
	m_blacklist.emplace("LL");
	m_blacklist.emplace("VV");
	m_blacklist.emplace("IIII");
	m_blacklist.emplace("XXXX");
	m_blacklist.emplace("CCCC");
	m_blacklist.emplace("MMMM");
}

std::string RomanSystem::ToRoman(unsigned int value){
	if((value > 3999) || (value < 1)){
		throw RomanCoversionException("Input value out of range.");
	} 
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
    std::transform(symbols.begin(),symbols.end(),symbols.begin(),toupper);
    SymbolChecker(symbols);
    unsigned int result = 0;
    for (unsigned int i = 0; i < (symbols.length() - 1); ++i){ 
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

void RomanSystem::SymbolChecker(const std::string symbols){
    for(const auto& token : m_blacklist){
        if(std::string::npos != symbols.find(token) ){
            throw RomanCoversionException("Invalid pattern detected in input string.");
        } 
    }
}
            
bool RomanSystem::IsRoman(const std::string& token) const{
    if(1 != token.size()){
        return false;
    }
    return (m_roman_dictionary.end() != m_roman_dictionary.find(toupper(token[0])));
}
