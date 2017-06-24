#include "queryparser.hh" 

using namespace galaxy_trade;

QueryParser::QueryParser(){
    m_operators.emplace("IS");
    m_operators.emplace("HOW MUCH IS");
    m_operators.emplace("HOW MANY CREDITS IS");

    m_terminators.emplace("?");
}

std::string QueryParser::ProcessQuery(std::string query){
    query = std::regex_replace(query, std::regex("^ +| +$|( ) +"), "$1");
    VerifyQuery(query);
}

void QueryParser::VerifyQuery(std::string query){
   if(0 == query.size())
       throw QuerySyntaxException("Empty query submitted.");

   std::transform(query.begin(),query.end(),query.begin(),toupper);

   /// find if atleast one of the operators is present.
   /// TODO : split query here itself for processing. But returning a collection of strings by value
   /// or accepting collection by reference ? Both seem expensive than searching.
   for(auto& o : m_operators){
       if(std::string::npos != query.find(o))
           return ;
   }
   throw QuerySyntaxException("I have no idea what you are talking about.");
}
