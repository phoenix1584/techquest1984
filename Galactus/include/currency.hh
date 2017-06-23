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

            void SetUnit(std::string unit_name)
            { m_unit_name = unit_name ; }


    };
};

#endif /* CURRENCY_HH */
