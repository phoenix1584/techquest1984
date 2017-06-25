/*
* Objective
* Test cases for query parser implementation
*/

#define PF_BTM BOOST_TEST_MESSAGE(__PRETTY_FUNCTION__)
#define BT_START BOOST_TEST_MESSAGE( "\n**************************************************************************\n" \
        << boost::unit_test::framework::current_test_case().p_name << \
        "\n**************************************************************************\n")

#define BOOST_TEST_MODULE QUERY_PARSER 
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "queryparser.hh"
using namespace galaxy_trade;

void Populate(std::vector<std::string>& test_data){
    test_data.push_back("test query");
    test_data.push_back("glob is I");
    test_data.push_back("prok is V");
    test_data.push_back("pish is X");
    test_data.push_back("tegj is L");
    test_data.push_back("glob glob Silver is 34 Credits");
    test_data.push_back("glob prok Gold is 57800 Credits");
    test_data.push_back("pish pish Iron is 3910 Credits");
    test_data.push_back("how much is pish tegj glob glob ?");
    test_data.push_back("how many Credits is glob prok Silver ?");
    test_data.push_back("how many Credits is glob prok Gold ?");
    test_data.push_back("how many Credits is glob prok Iron ?");
    test_data.push_back("how much wood could a woodchuck chuck if a woodchuck could chuck wood ?");
}

BOOST_AUTO_TEST_SUITE(query_parser)
    BOOST_AUTO_TEST_CASE(verify){
        std::vector<std::string> test_data;
        Populate(test_data);
        QueryParser qp;
        for(const auto& v : test_data){
            try{
                qp.VerifyQuery(v);
            }catch(QuerySyntaxException& e){
                BOOST_TEST_MESSAGE( "Query : " << v  << ", Error Response : "<< e.what());
            }
        }
    }

BOOST_AUTO_TEST_CASE(white_space){
    BT_START;
    std::string value("  Space   Balanced string   ");
    BOOST_TEST_MESSAGE( " Before : " << value);
    // Ref : https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
    value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");
    BOOST_TEST_MESSAGE( " After : " << value);
}

BOOST_AUTO_TEST_CASE(str_replace){
   BT_START; 
   std::string query("How many Credits in this query ?");
   std::regex terminators("C");
   // Ref : http://en.cppreference.com/w/cpp/regex/regex_replace
    std::regex_replace(std::ostreambuf_iterator<char>(std::cout),query.begin(), query.end(), terminators, " "); 
    std::cout << std::regex_replace(query,terminators,"") << "\n";
    BOOST_TEST_MESSAGE(std::regex_replace(query,terminators,"TOKEN"));
}

BOOST_AUTO_TEST_CASE(split_query){
    BT_START;
    std::string query("test is value");
    std::string token("is");
    std::size_t found = query.find(token);
    BOOST_TEST_MESSAGE(((std::string::npos == found) ? "Token NOT found" : std::string(query.begin()+found + token.size(), query.end())));
    BOOST_TEST_MESSAGE(std::string(query.begin(),query.begin() + found));
}

BOOST_AUTO_TEST_CASE(term_test){
    BT_START;
    std::string text("This is test data?");
    std::string result;
    std::remove_copy_if(text.begin(), text.end(),            
            std::back_inserter(result), //Store output           
            std::ptr_fun<int, int>(&std::ispunct)  
            );
    BOOST_TEST_MESSAGE(result);
}
BOOST_AUTO_TEST_SUITE_END()
