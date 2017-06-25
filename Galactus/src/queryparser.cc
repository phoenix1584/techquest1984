#include "queryparser.hh" 
#ifdef LIB_DEBUG
#include <iostream>
#endif

using namespace galaxy_trade;

QueryParser::QueryParser()
    : m_pivot_token(" IS ")
{
    m_operators.emplace("HOW MUCH");
    m_operators.emplace("HOW MANY");

    m_terminators.emplace("?");
}

std::string QueryParser::ProcessQuery(std::string query){
    query = std::regex_replace(query, std::regex("^ +| +$|( ) +"), "$1");
    std::transform(query.begin(),query.end(),query.begin(),toupper);
    VerifyQuery(query);
    // Identify the query and accordingly invoke API call of currency system
    // All queries are currently pivoted around token "IS".
    // Split the query around IS , check if the first half is an operator or Value
    // Value will start with symbol from dynamic system. But this assumption might become
    // obsolete as the language syntax expands. 
    std::size_t found = query.find(m_pivot_token);
    std::string first_query_part(query.begin(),query.begin() + found);
    std::string second_query_part(query.begin() + found + m_pivot_token.size(), query.end());
    // Remove punctuation terminators.
    std::string result;
    std::remove_copy_if(second_query_part.begin(), second_query_part.end(),            
            std::back_inserter(result), //Store output           
            std::ptr_fun<int, int>(&std::ispunct)  
            );
    second_query_part = std::regex_replace(result, std::regex("^ +| +$|( ) +"), "$1");
    

    /// If first half has an operator its evaluation else assignment
    std::string response ;
    bool is_assignment = true;
    for(auto& o : m_operators){
        if(std::string::npos != first_query_part.find(o)){
            response.append(ProcessEvaluationQuery(second_query_part));
            is_assignment = false;
            break;
        }
    }
    if(is_assignment){
        ProcessAssignmentQuery(first_query_part,second_query_part);
    }
    return response;
}

void QueryParser::VerifyQuery(std::string query){
   if(0 == query.size())
       throw QuerySyntaxException("Empty query submitted.");

   std::transform(query.begin(),query.end(),query.begin(),toupper);

   /// Check if pivot token is present. 
   if(std::string::npos == query.find(m_pivot_token)){
       throw QuerySyntaxException("I have no idea what you are talking about.");
   }
   
   /// find if atleast one of the operators is present.
   /// TODO : split query here itself for processing. But returning a collection of strings by value
   /// or accepting collection by reference ? Both seem expensive than searching.
   for(auto& o : m_operators){
       if(std::string::npos != query.find(o))
           return ;
   }
}

void QueryParser::ProcessAssignmentQuery(std::string token,std::string value){
#ifdef LIB_DEBUG
    std::cout << __func__ << "\n";
#endif
    m_currency_system.AddToken(token,value);
}

std::string QueryParser::ProcessEvaluationQuery(std::string eval_expression){
    return m_currency_system.ToValue(eval_expression,m_pivot_token);
}
