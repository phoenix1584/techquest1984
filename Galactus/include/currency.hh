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
            std::map<std::string,unsigned int> m_rate_chart;

            /// string representing the unit , used for identifying query syntax
            std::string m_unit_name;

        public:
            /// Constructor
            Currency()
                :m_unit_name("Credits")
            {}
            
            /// Add a commodity and its base price per unit deduced from the provided string
            void AddToken(std::string commodity, std::string value);

            /// Return the value represented. This could be in any number system.
            /// \param[in] sym - data to be translated to value.
            /// \return unsidned int - value in credits
            std::string ToValue(std::string sym,std::string token = " ");
    
            bool IsValidCommodity(std::string comm_name) const
            { return (m_rate_chart.end() != m_rate_chart.find(comm_name)); }

            /// TODO : Should be useful for dynamic cases but difficulty in deducing from query text only.
            // void SetUnit(std::string unit_name)
            // { m_unit_name = unit_name ; }
    };
};

#endif /* CURRENCY_HH */
