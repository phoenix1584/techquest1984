//  Asynchronous client-to-server (DEALER to ROUTER)
//
//  While this example runs in a single process, that is to make
//  it easier to start and stop the example. Each task has its own
//  context and conceptually acts as a separate process.

#define BOOST_TEST_MODULE COMMUNICATION 
#include <boost/test/unit_test.hpp>

#include <array>
#include <thread>
#include <memory>
#include <functional>

#include <zmq.hpp>
#include <zhelpers.hpp>
#include "t_comm_classes.hh"

using namespace test_comm;

/**
 * General purpose data and functions
 */

enum { kMaxThread = 10 };

/**
* Utilites class.
*/
class Utils{
    public:
        static std::string GetIDString(){
            // generate random identity
            char identity[10] = {};
            sprintf(identity, "%04X-%04X", within(0x10000), within(0x10000));
            std::string id(identity);
            BOOST_ASSERT(id.length() == strlen(identity));
            return id; 
        }
};

/**
*  This is our client task class.
*  It connects to the server, and then sends a request once per second
*  It collects responses as they arrive, and it prints them out. We will
*  run several client tasks in parallel, each with a different random ID.
*  Attention! -- this random work well only on linux.
*/


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
            //TODO : Deserialization will happen here.
            MessageHeader msg(data);
            BOOST_CHECK(!strcmp(msg.m_dst_id.c_str(),"SERVER"));
            BOOST_CHECK(!strcmp(msg.m_src_id.c_str(),m_identity.c_str()));
            return data.str().length(); 
        }
    public:
        client_task()
            : ctx_(1),
            client_socket_(ctx_, ZMQ_DEALER),
            m_identity(Utils::GetIDString())
    {}
        void start() {
            client_socket_.setsockopt(ZMQ_IDENTITY, m_identity.c_str(), m_identity.length());
            client_socket_.connect("tcp://localhost:5570");

            zmq::pollitem_t items[] = {{static_cast<void *>(client_socket_), 0, ZMQ_POLLIN, 0}};
            auto request_nbr = 0;
            try {
                // TODO : Find message based trigger the loop.
                while (request_nbr++ < 10) {
                    {
                        //Message generator.
                        MessageHeader msg(m_identity,"SERVER");
                        std::string str_msg = msg.Serialize();
                        BOOST_CHECK(str_msg.length());
                        s_send(client_socket_,str_msg);
                    }
                    for (int i = 0; i < 100; ++i) {
                        zmq::poll(items, 1, 10);// 10 milliseconds
                        if (items[0].revents & ZMQ_POLLIN) {
                            MessageHandler();
                        }
                    }
                }
            }
            catch (std::exception &e) {}
        }

    private:
        zmq::context_t ctx_;
        zmq::socket_t client_socket_;
        const std::string m_identity;
};

/**
*  .split worker task
*  Each worker task works on one request at a time and sends a random number
*  of replies back, with random delays between replies:
*/
class server_worker {
    private:
        bool MessageHandler(){
            //TODO: send custom messages to indicate end of session.
            zmq::message_t identity , msg , copied_id , copied_msg;
            worker_.recv(&identity);
            worker_.recv(&msg);

            int replies = within(5) + 1;
            for (int reply = 0; reply < replies; ++reply) {
                s_sleep(within(100) + 1);
                copied_id.copy(&identity);
                copied_msg.copy(&msg);
                worker_.send(copied_id, ZMQ_SNDMORE);
                worker_.send(copied_msg);
            }
            return replies;
        }
    public:
        server_worker(zmq::context_t &ctx, int sock_type)
            : ctx_(ctx),
            worker_(ctx_, sock_type),
            m_identity(Utils::GetIDString())
    {}
        void work() {
            worker_.connect("inproc://backend");

            try {
                while (MessageHandler())
                    ;
            }
            catch (std::exception &e) {}
        }
    private:
        zmq::context_t &ctx_;
        zmq::socket_t worker_;
        const std::string m_identity;
};

/**
*  This is our server task.
*  It uses the multithreaded server model to deal requests out to a pool
*  of workers and route replies back to clients. One worker can handle
*  one request at a time but one client can talk to multiple workers at
*  once.
*/

class server_task {
    public:
        server_task()
            : ctx_(1),
            frontend_(ctx_, ZMQ_ROUTER),
            backend_(ctx_, ZMQ_DEALER)
    {}


        void run() {
            frontend_.bind("tcp://*:5570");
            backend_.bind("inproc://backend");
            
            std::array<std::pair<std::unique_ptr<server_worker>,std::unique_ptr<std::thread> >,kMaxThread > worker_registry;
            for(auto&& reg : worker_registry){
                reg.first.reset(new server_worker(ctx_, ZMQ_DEALER)); 
                reg.second.reset(new std::thread(std::bind(&server_worker::work, reg.first.get())));
                reg.second.get()->detach();
            }

            try {
                zmq::proxy(static_cast<void *>(frontend_), static_cast<void *>(backend_), nullptr);
            }
            catch (std::exception &e) {}
        }
    private:
        zmq::context_t ctx_;
        zmq::socket_t frontend_;
        zmq::socket_t backend_;
};


//  The main thread simply starts several clients and a server, and then
//  waits for the server to finish.

BOOST_AUTO_TEST_SUITE(zmq)
BOOST_AUTO_TEST_CASE(async_server){
    // Setup server
    server_task st;
    std::thread server_thread(std::bind(&server_task::run, &st));
    server_thread.detach();

    //Create task object registry 
    std::array<std::pair<client_task,std::unique_ptr<std::thread> >,kMaxThread> client_registry;
    for(auto&& reg : client_registry){
        reg.second.reset(new std::thread(std::bind(&client_task::start, &reg.first)));
        usleep(100);
    }

    //Wait for completion of client threads
    for(auto&& thr : client_registry){
        thr.second.get()->join();
    }
}
BOOST_AUTO_TEST_SUITE_END()
