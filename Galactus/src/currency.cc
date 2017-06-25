#include "currency.hh"
#include <boost/lexical_cast.hpp>
#include <regex>
#ifdef LIB_DEBUG
#include <iostream>
#endif
using namespace galaxy_trade;

void Currency::AddToken(std::string commodity, std::string value){
    /// Eval expr will have two kinds of tokens dynamic system followed by currency
    /// So append till non dynamic token is found. Not using std::partition since it will reorder container and might
    /// cause issues while expnading the framework.
    std::vector<std::string> split_data;
    commodity = std::regex_replace(commodity, std::regex("^ +| +$|( ) +"), "$1");
    boost::split(split_data,commodity,boost::is_any_of(" "));
#ifdef LIB_DEBUG
    std::cout << commodity << "," << value << "," << split_data.size() << "\n";
#endif
    if (split_data.size() == 1){
        DynamicNumberSystem::AddSymbol(commodity,value);
    }else{
        std::string dyn_symbols;
        std::string commodity_symbols;
        for(auto& v : split_data){
            if(IsValidSymbol(v)){
                dyn_symbols.append(v);
                dyn_symbols.append(" ");
            }else{
                commodity_symbols.append(v);
                commodity_symbols.append(" ");
            }
        }

        unsigned int quantity = DynamicNumberSystem::ToValue(dyn_symbols);

        split_data.clear();
        boost::split(split_data,value,boost::is_any_of(" "));
        if(split_data.size() < 2){
            throw CurrencyExceptions("Insufficient rate data provided for assignment.");
        }

        // Ignore the unit will work if unit is absent. 
        unsigned int cost_per_unit = (RomanSystem::IsRoman(split_data[0]) ? RomanSystem::ToValue(split_data[0]) : boost::lexical_cast<unsigned int>(split_data[0]))/quantity;

        // Append the new information to the rate chart. If it already exists then it will not be update so update manually.
        std::pair<std::map<std::string,unsigned int>::iterator, bool> ret;
        ret = m_rate_chart.insert(std::pair<std::string,unsigned int>(commodity_symbols,cost_per_unit));
        if(!ret.second){
            ret.first->second = cost_per_unit;
        } 
    }
}

std::string Currency::ToValue(std::string sym,std::string token){
    // TODO : Code replication , convert to function.
    std::vector<std::string> split_data;
    sym= std::regex_replace(sym, std::regex("^ +| +$|( ) +"), "$1");
    boost::split(split_data,sym,boost::is_any_of(" "));
    std::string dyn_symbols;
    std::string commodity_symbols;
    for(auto& v : split_data){
        if(IsValidSymbol(v)){
            dyn_symbols.append(v);
            dyn_symbols.append(" ");
        }else{
            commodity_symbols.append(v);
            commodity_symbols.append(" ");
        }
    }
#ifdef LIB_DEBUG
    std::cout << dyn_symbols << "," << commodity_symbols << "\n";
#endif
    std::string response; 
    if(commodity_symbols.size()){
        std::map<std::string,unsigned int>::iterator itr = m_rate_chart.find(commodity_symbols);
        if(itr == m_rate_chart.end()){
            throw CurrencyExceptions("Unkown symbol provided."); 
        }
        response.append(commodity_symbols);
        response.append(token);
        response.append(boost::lexical_cast<std::string>(DynamicNumberSystem::ToValue(dyn_symbols) * itr->second));
        response.append(" ");
        response.append(m_unit_name);
    }else{
        response = boost::lexical_cast<std::string>(DynamicNumberSystem::ToValue(dyn_symbols));
    }
    return response;
}
