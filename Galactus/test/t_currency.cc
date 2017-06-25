/*
* Objective
* Test cases for currency implementation
*/

#define PF_BTM BOOST_TEST_MESSAGE(__PRETTY_FUNCTION__)
#define BT_START BOOST_TEST_MESSAGE( "\n**************************************************************************\n" \
        << boost::unit_test::framework::current_test_case().p_name << \
        "\n**************************************************************************\n")

#define BOOST_TEST_MODULE CURRENCY_SYSTEM 
#include <boost/test/unit_test.hpp>
#include "currency.hh"

using namespace galaxy_trade;

BOOST_AUTO_TEST_SUITE(currency_system)
    BOOST_AUTO_TEST_CASE(add_value){
    }
BOOST_AUTO_TEST_SUITE_END()
