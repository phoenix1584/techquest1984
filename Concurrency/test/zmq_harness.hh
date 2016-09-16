#ifndef ZMQ_TEST_HARNESS_HH__
#define ZMQ_TEST_HARNESS_HH__

#include <boost/assign.hpp>
#include <set>
namespace utils{
    class TestUtils{
        private:
            std::map<std::string,std::string> m_cmd_resp_dict;
            std::string m_default_response;
        public:
            TestUtils()
                :m_default_response("UNKOWN_COMMAND")
            {
                m_cmd_resp_dict = boost::assign::map_list_of("REGISTER", "REG_ACK")
                    ("COMMAND_1", "CMD_ACK")
                    ("TASK_ACK", "TASK_COMPLETED")
                    ("EXIT_ACK","EXIT");
            }
            std::string LogPathCreator(std::string test_name){
                std::string log_path("/tmp/");
                log_path.append(boost::unit_test::framework::current_test_case().p_name);
                log_path.append(".log");
                return log_path;
            }
            
            std::string GetIDString(){
                // generate random identity
                char identity[10] = {};
                sprintf(identity, "%04X-%04X", within(0x10000), within(0x10000));
                std::string id(identity);
                BOOST_ASSERT(id.length() == strlen(identity));
                return id; 
            }

            std::string GetResponse(const std::string& req){
                std::map<std::string,std::string>::iterator it = m_cmd_resp_dict.find(req);
                 return (it != m_cmd_resp_dict.end()) ? it->second : m_default_response ;
                
            }
    };

    class client_task {
        private:
            bool MessageHandler()
            {   
                std::stringstream data ;
                int more = 0;
                size_t more_size = sizeof(more);
                do{
                    data << std::string(s_recv(client_socket_));
                    client_socket_.getsockopt(ZMQ_RCVMORE, &more, &more_size);
                }while(more);
                BOOST_TEST_MESSAGE(data.str());
                return (strcmp(data.str().c_str(),"EXIT"));
            }
        public:
            client_task()
                : ctx_(1),
                client_socket_(ctx_, ZMQ_DEALER),
                m_identity(m_test_utils.GetIDString())
        {}
            void run() {
                s_catch_signals();
                client_socket_.setsockopt(ZMQ_IDENTITY, m_identity.c_str(), m_identity.length());
                client_socket_.connect("tcp://localhost:5570");

                s_send(client_socket_,"REGISTER");
                zmq::pollitem_t items[] = {{static_cast<void *>(client_socket_), 0, ZMQ_POLLIN, 0}};
                try {
                    while(!s_interrupted){
                        zmq::poll(items, 1, 10);// 10 milliseconds
                        if (items[0].revents & ZMQ_POLLIN) {
                            if(!MessageHandler())
                                break;
                        }
                    }
                }
                catch (std::exception &e) {}
            }

        private:
            TestUtils m_test_utils;
            zmq::context_t ctx_;
            zmq::socket_t client_socket_;
            const std::string m_identity;

    };

    class server_task {
        private:
            bool MessageHandler(){
                std::deque<std::string> data ;
                int more = 0;
                size_t more_size = sizeof(more);
                do{
                    data.push_back(s_recv(frontend_));
                    frontend_.getsockopt(ZMQ_RCVMORE, &more, &more_size);
                }while(more);
                frontend_.send(data[0].c_str(),data[0].length(),ZMQ_SNDMORE);
                s_send(frontend_,"EXIT");
                return false;
            }
        public:
            server_task()
                : ctx_(1),
                frontend_(ctx_, ZMQ_ROUTER),
                m_identity("SERVER")
        {}

            void run() {
                s_catch_signals();
                frontend_.bind("tcp://*:5570");
                zmq::pollitem_t items[] = {{static_cast<void *>(frontend_), 0, ZMQ_POLLIN, 0}};
                try {
                    while(!s_interrupted){
                        zmq::poll(items, 1, 10);// 10 milliseconds
                        if (items[0].revents & ZMQ_POLLIN) {
                            if(!MessageHandler())
                                break;
                        }
                    }
                }
                catch (std::exception &e) {}
            }
        private:
            TestUtils m_test_utils;
            zmq::context_t ctx_;
            zmq::socket_t frontend_;
            std::string m_identity;
    };
};
#endif /* ZMQ_TEST_HARNESS_HH__ */
