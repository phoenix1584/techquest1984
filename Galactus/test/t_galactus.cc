/*
* Objective
*/

#define PF_BTM BOOST_TEST_MESSAGE(__PRETTY_FUNCTION__)
#define BT_START BOOST_TEST_MESSAGE( "\n**************************************************************************\n" \
        << boost::unit_test::framework::current_test_case().p_name << \
        "\n**************************************************************************\n")

#define BOOST_TEST_MODULE GALACTUS 
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(galactus)
BOOST_AUTO_TEST_CASE(test1){
    BT_START;
}
BOOST_AUTO_TEST_SUITE_END()
