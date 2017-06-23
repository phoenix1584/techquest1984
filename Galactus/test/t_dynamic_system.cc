/*
* Objective
* Test cases for dynamic numeric implementation
*/

#define PF_BTM BOOST_TEST_MESSAGE(__PRETTY_FUNCTION__)
#define BT_START BOOST_TEST_MESSAGE( "\n**************************************************************************\n" \
        << boost::unit_test::framework::current_test_case().p_name << \
        "\n**************************************************************************\n")

#define BOOST_TEST_MODULE ROMAN_SYSTEM 
#include <boost/test/unit_test.hpp>
#include "dynamicsystem.hh"

using namespace galaxy_trade;

/// Could add more symbols if needed but enough to prove that file could be used to perform similar task.
void Populate(DynamicNumberSystem& data){
    data.AddSymbol("glob","I");
    data.AddSymbol("prok","V");
    data.AddSymbol("pish","X");
    data.AddSymbol("tegj","L");
}

BOOST_AUTO_TEST_SUITE(roman_system)
    BOOST_AUTO_TEST_CASE(add_value){
        BT_START;
        DynamicNumberSystem dns;
        Populate(dns);
        BOOST_TEST_MESSAGE(dns);
    }

BOOST_AUTO_TEST_CASE(get_value){
    BT_START;
    DynamicNumberSystem dns;
    Populate(dns);
    std::string test("prok");
    BOOST_TEST_MESSAGE(" Test Input : prok => Roman : "<< dns.ToRoman("prok") << ", Value : " << dns.ToValue("prok"));
}

BOOST_AUTO_TEST_CASE(conversion){
    BT_START;
    std::string data("prok glob");
    DynamicNumberSystem dns;
    Populate(dns);
    BOOST_TEST_MESSAGE( data << " => " << dns.ToValue(data));
}
BOOST_AUTO_TEST_SUITE_END()
