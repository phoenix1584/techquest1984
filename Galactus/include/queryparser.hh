#ifndef QUERY_PARSER_HH
#define QUERY_PARSER_HH

#include "dynamicsystem.hh"

/// @brief : Class that encapsulates the query parsing and dynamic token conversions
///
/// Needs to handle the dynamic tokens being accumlated with each query being parsed.
/// TODO:
/// * Should write the data to file in a fixed pattern that will behave as the training data set
///   that can be loaded every time an instance is executed. 
/// * Inputs for operators, units , computation patterns should come from files accepted as input.
//    That will keep this code as generic algorithms independent of any static data.
class QueryParser{
    private:
        /// Base system conversion instance.
        galaxy_trade::RomanSystem m_roman_system;

        /// Collection of recognized operators
        std::set<std::string> m_operators;

        /// Collection of valid Units
        std::set<std::string> m_units;        

    public:
        
        /// @brief Verify and execute the query.
        ///
        /// \param[in] query - string representation of the computation request
        /// \return std::string - response string. TODO: How to return a specific type? Will that ever become necessary in such scenario ?
        /// \throw QuerySyntaxException, RomanCoversionException
        std::string ProcessQuery(std::string query);

        /// @brief  Verify the syntax and structure of the query.
        ///
        /// \param[in] query - query to be verified.
        /// \return 
        /// \throw QuerySyntaxException, RomanCoversionException
        void VerifyQuery(const std::string& query);

};

#endif /*QUERY_PARSER_HH*/
