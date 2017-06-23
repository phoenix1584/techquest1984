#ifndef ROMAN_SYSTEM_HH
#define ROMAN_SYSTEM_HH

#include "galactic_exceptions.hh"
#include <set>
#include <map>

namespace galaxy_trade{

    /// @ brief Represents a combination of symbol and value.
    /// 
    /// TODO: Update the Symbol or value. Risky if storing data across systems or DB. So not implemented for now.
    class RomanSymbol{
        private:
            /// Value assigned to the symbol
            unsigned int m_value;

            /// String literal represeting the symbol
            std::string m_symbol;

        public:
            /// Constructor
            RomanSymbol(const int value = 0,const std::string symbol="")
                :m_value(value)
                ,m_symbol(symbol)
            {}

            /// Get symbol
            std::string GetSymbol() const
            { return m_symbol; }

            /// Get value
            unsigned int GetValue() const
            { return m_value; }

    };

    /// Class to compare two roman symbols
    class RomanValueComparator{
        public:
            bool operator()(const RomanSymbol& lhs, const RomanSymbol& rhs){
                if(lhs.GetSymbol() == rhs.GetSymbol()){
                    throw RomanCoversionException("Symbol replication detected.");
                }
                return (lhs.GetValue() < rhs.GetValue());
            };
    };

    /// @brief Roman arithmatic and translator.
    ///  
    /// Class to encapsulate 
    /// * numeral symbols
    /// * Symbol values 
    /// * system arithmatic
    /// * data checks ( recurring pattern , subtraction rules )
    class RomanSystem{
        private:
            /// The basic symbol to value relation.
            /// Set will ensure uniqueness of combination
            std::set<RomanSymbol,RomanValueComparator> m_value_data;

            /// Symbol to value map for quick access.
            std::map<char,unsigned int> m_roman_dictionary;

			/// Collection of unacceptable patterns
			std::set<std::string> m_blacklist;

        public:
            /// Constructor
            RomanSystem(); /// TODO: Initializer list usage.
            
            /// Convert integer to roman symbol representation
            std::string ToRoman(unsigned int value);

            /// Convert from symbols to value 
            unsigned int ToValue(std::string symbols);

            /// @brief Format checker for roman symbols
			/// 
            /// TODO : accept input patterns as files
			/// Check for acceptable patterns from the dictionary object.
            bool SymbolChecker(const std::string symbols);

    };
};

#endif /*ROMAN_SYSTEM_HH */
