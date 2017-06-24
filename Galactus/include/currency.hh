#ifndef CURRENCY_HH
#define CURRENCY_HH

#include "dynamicsystem.hh"

namespace galaxy_trade{

    ///@brief Stores collection of information regarding tokens and values but not bounded by roman limits.
    ///
    /// Needs to convert the dynamic number system to its value to get the actual factors for deciding values to be stored.
    class Currency : public DynamicNumberSystem { 
        private:

            /// Token vs value
            std::map<std::string,int> m_rate_chart;

            /// string representing the unit , used for identifying query syntax
            std::string m_unit_name;

        public:
            /// Constructor
            Currency()
                :m_unit_name("Credits")
            {}

            /// Return the value represented. This could be in any number system.
            /// \param[in] sym - data to be translated to value.
            /// \return unsidned int - value in credits
            unsigned int ToValue(std::string sym); 
    
            /// Calculate the price represented as sequence of symbols
            /// \param[in] sym - string representation of data as "quantity commodity" 
            /// \return unsigned int - result of computation
            unsigned int GetPrice(std::string sym);

            /// TODO : Should be useful for dynamic cases but difficulty in deducing from query text only.
            // void SetUnit(std::string unit_name)
            // { m_unit_name = unit_name ; }


    };
};

#endif /* CURRENCY_HH */
