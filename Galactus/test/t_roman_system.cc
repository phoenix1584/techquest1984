/*
* Objective
* Test cases for roman numeric implementation
*/

#define PF_BTM BOOST_TEST_MESSAGE(__PRETTY_FUNCTION__)
#define BT_START BOOST_TEST_MESSAGE( "\n**************************************************************************\n" \
        << boost::unit_test::framework::current_test_case().p_name << \
        "\n**************************************************************************\n")

#define BOOST_TEST_MODULE ROMAN_SYSTEM 
#include <boost/test/unit_test.hpp>
#include "romansystem.hh"

using namespace galaxy_trade;

BOOST_AUTO_TEST_SUITE(roman_system)
    BOOST_AUTO_TEST_CASE(data_order){
        BT_START;
        std::set<RomanSymbol,RomanValueComparator> test_set;
        test_set.emplace(RomanSymbol(1000,"A"));
        test_set.emplace(RomanSymbol(100,"B"));
        for(std::set<RomanSymbol>::reverse_iterator itr = test_set.rbegin() ; itr != test_set.rend() ; ++itr){
            BOOST_TEST_MESSAGE( itr->GetValue()); 
        }
    }

BOOST_AUTO_TEST_CASE(data_comp){
    BT_START;
    try{
        std::set<RomanSymbol,RomanValueComparator> test_set;
        test_set.emplace(RomanSymbol(100,"A"));
        test_set.emplace(RomanSymbol(1000,"A"));
        for(std::set<RomanSymbol>::reverse_iterator itr = test_set.rbegin() ; itr != test_set.rend() ; ++itr){
            BOOST_TEST_MESSAGE( itr->GetValue()); 
        }
    }catch(RomanCoversionException& e){
        BOOST_TEST_MESSAGE(e.what());
    }
}

BOOST_AUTO_TEST_CASE(to_roman){
    BT_START;
    try{
        RomanSystem rs;
        BOOST_TEST_MESSAGE("Input 101 : " << rs.ToRoman(101));
        BOOST_TEST_MESSAGE("Input 5000 : " << rs.ToRoman(5000));
    }catch(RomanCoversionException& e){
        BOOST_TEST_MESSAGE(e.what());
    }
}

BOOST_AUTO_TEST_CASE(to_value){
    BT_START;
    RomanSystem rs;
    BOOST_TEST_MESSAGE(rs.ToValue("XXI"));
}

BOOST_AUTO_TEST_CASE(sym_check){
    BT_START;
    RomanSystem rs;
    try{
        /// Following is done to avoid the duplication of transform inside Roman system class.
        std::string test("iiii");
        std::transform(test.begin(),test.end(),test.begin(),toupper);
        BOOST_CHECK( true == rs.SymbolChecker(test));
    }catch(RomanCoversionException& e){
        BOOST_TEST_MESSAGE(e.what());
    }


}
BOOST_AUTO_TEST_SUITE_END()
