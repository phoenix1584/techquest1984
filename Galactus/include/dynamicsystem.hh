#ifndef DYNAMIC_SYSTEM_HH
#define DYNAMIC_SYSTEM_HH

#include "romansystem.hh"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace galaxy_trade{
 
    /// @brief Class to store the sematics of the new number system being provided as queries
    ///
    /// This number system completely relies on Roman System for value assignment hence inheritance
    /// seems the most convinient way for sharing the information needed for translation.
    class DynamicNumberSystem : public galaxy_trade::RomanSystem{
        private:
            /// Collection of symbols vs value based on Roman system
            std::map<std::string,std::string> m_dyn_dictionary;

        public:
            /// Add a new symbol to hte dictionary based on its roman equvivalent
            void AddSymbol(std::string dyn_symbol,std::string roman_symbol);

            /// Check if the symbols exists in dictionary
            bool IsValidSymbol(const std::string s);

            /// Get the value being assigned to a symbol
            unsigned int ToValue(std::string s);

            /// Get equivalent Roman literal
            std::string ToRoman(std::string sym);

            /// operator to print the new number system
            friend std::ostream& operator<<(std::ostream& os,const galaxy_trade::DynamicNumberSystem& d){
                os << " DYN | ROMAN \n";
                os << "-----|-------\n";
                for(auto& x : d.m_dyn_dictionary){
                    os << x.first << " | " << x.second << "\n";
                }
                return os;
            }
    };
};

#endif /*DYNAMIC_SYSTEM_HH */
