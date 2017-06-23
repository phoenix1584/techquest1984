#ifndef DYNAMIC_SYSTEM_HH
#define DYNAMIC_SYSTEM_HH

#include "romansystem.hh"

/// @brief Class to store the sematics of the new number system being provided as queries
///
/// This number system completely relies on Roman System for value assignment hence inheritance
/// seems the most convinient way for sharing the information needed for translation.
class DynamicNumberSystem : public galaxy_trade::RomanSystem{
    private:
        /// Collection of symbols vs value based on Roman system
        std::map<std::string,int> m_dyn_dictionary;

    public:
        /// Add a new symbol to hte dictionary based on its roman equvivalent
        void AddSymbol(std::string dyn_symbol,std::string roman_symbol);

        /// Directly add a symbol to dictonary with provided value.
        void AddSymbol(std::string dyn_symbol,int value);

};



#endif /*DYNAMIC_SYSTEM_HH */
