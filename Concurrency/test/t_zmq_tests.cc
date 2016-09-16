#define BOOST_TEST_MODULE ZMQ_TESTS
#include <boost/test/unit_test.hpp>    
/*
 * Objective
 * =========
 * The primary purpose of this test case to simulate a network of clients and servers
 * to replicate a use case. Examples
 * -- Server outage.
 * -- Client failure.
 * -- Route unavailability.
 * -- Queuing parameter effects.
 *
 * Test Suite Layout
 * ==================
 * - Individual test cases like zmq_server, zmq_client will provide the individual process codes 
 *   necessary.
 * - The zmq_driver test case will launch the other test cases process. (using the run_test=<suite/name>)
 * - Configuration parameters will be passed 
 *     -- using poco process arguments (need to verify how to forward them to Boost test cases.)
 *     -- creating a configuration file on disk
 * - POCOLogger class should be used to create loggers for each test case.
 * - Multiple driver test cases should be written to replicate specific failure scenarios.
 * - Addition debugging is possible by using tools like netstat, tcpdump.
 * - ZMQ socket configuration change should allow communication across machines without need of compilation.
 *
 */
#include "zmq.hpp"
#include "zhelpers.hpp"
#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include <boost/filesystem.hpp>
#include <sstream>
#include <vector>
#include "logger.hh"
namespace utils{
    class TestUtils{
        public:
            std::string LogPathCreator(std::string test_name){
                std::string log_path("/tmp/");
                log_path.append(boost::unit_test::framework::current_test_case().p_name);
                log_path.append(".log");
                return log_path;
            }
    };
};

using namespace utils;
BOOST_AUTO_TEST_SUITE(zmq_tests)
BOOST_AUTO_TEST_CASE(zmq_server){
    TestUtils test_utils;
    POCOLogger t_zserver_log(test_utils.LogPathCreator(boost::unit_test::framework::current_test_case().p_name),boost::unit_test::framework::current_test_case().p_name);
    for(int i = 0 ; i < boost::unit_test::framework::master_test_suite().argc; ++i){
        t_zserver_log.m_file_logger.information(boost::unit_test::framework::master_test_suite().argv[i]);
    }
}

BOOST_AUTO_TEST_CASE(zmq_client){
    TestUtils test_utils;
    POCOLogger t_zclient_log(test_utils.LogPathCreator(boost::unit_test::framework::current_test_case().p_name),boost::unit_test::framework::current_test_case().p_name);
    for(int i = 0 ; i < boost::unit_test::framework::master_test_suite().argc; ++i){
        t_zclient_log.m_file_logger.information(boost::unit_test::framework::master_test_suite().argv[i]);
    }
}

BOOST_AUTO_TEST_CASE(zmq_driver){
    std::vector<std::string> poco_proc_args;
    std::string suite_name(boost::unit_test::framework::get<boost::unit_test::test_suite>(boost::unit_test::framework::current_test_case().p_parent_id).p_name);
    {
        std::stringstream ss_args;
        ss_args << "--run_test=" << suite_name << "/zmq_server" ; 
        poco_proc_args.push_back(ss_args.str());
    }
    poco_proc_args.push_back( "--log_level=message");
    poco_proc_args.push_back( "--");
    for(int i = 1 /* ignoring program name */ ; i < boost::unit_test::framework::master_test_suite().argc; ++i){
          poco_proc_args.push_back(boost::unit_test::framework::master_test_suite().argv[i]);
    }
    Poco::ProcessHandle h_zmq_server = Poco::Process::launch(boost::unit_test::framework::master_test_suite().argv[0],poco_proc_args);
    {
        std::stringstream ss_args;
        ss_args << "--run_test=" << suite_name << "/zmq_client" ; 
        poco_proc_args[0] = ss_args.str(); 
    }
    Poco::ProcessHandle h_zmq_client = Poco::Process::launch(boost::unit_test::framework::master_test_suite().argv[0],poco_proc_args);
    Poco::Process::wait(h_zmq_server);
    Poco::Process::wait(h_zmq_client);
}

BOOST_AUTO_TEST_SUITE_END()
