/*
 * Copyright (c) 2016
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/Exception.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"

#include "zmq.hpp"
#include "zhelpers.hpp"

#include "boost/lexical_cast.hpp"
#include <iostream>
#include <deque>
#include <map>

// Sends "maxcommand" messages sequentially. Resends till the response from the server is "OK".
class Client : public Poco::Task{
    private:
        class Tracker{
            public:
                bool m_response;
                bool m_success;
                Tracker()
                    :m_response(false)
                    ,m_success(false)
            {}
        };    

        Poco::Util::Application& m_app;
        zmq::socket_t m_socket;
        std::map<std::string,Tracker> m_response_tracker;
        
        bool MessageHandler(){
            std::deque<std::string> message;
            int more = 0;
            size_t more_size = sizeof(more);
            do{
                message.push_back(s_recv(m_socket));
                m_socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);
            }while(more);
            m_app.logger().debug( name() + ":[M]:" + message[1] + " => [R]:" + message[2]); 
            return(std::strcmp(message[2].c_str(),"OK"));
        }
    public:
        Client(zmq::context_t& ctx)
            :Task(Poco::Util::Application::instance().config().getString("system.pid"))
            ,m_app(Poco::Util::Application::instance())
            ,m_socket(ctx,ZMQ_DEALER)
    {}
        void runTask(){
            //Simulate a command data using time stamp
            s_catch_signals();
            m_app.logger().information("Task with name %s started",name());
            m_socket.connect(m_app.config().getString("test.server.address"));
            Poco::Timestamp now;
            std::string message = Poco::DateTimeFormatter::format(now,"%dd %H:%M:%S.%i");
            s_send(m_socket,message);
            zmq::pollitem_t items[] = {{static_cast<void *>(m_socket), 0, ZMQ_POLLIN, 0}};
            bool resend = false;
            while(!s_interrupted){
                zmq::poll(items, 1, 10);
                if (items[0].revents & ZMQ_POLLIN) {
                    resend = MessageHandler();
                }else{
                    // 5 seconds periodicity as sample
                    if(now.elapsed() > (5 * 1000 * 1000 )){
                        now.update();
                        if(!resend){
                            message = Poco::DateTimeFormatter::format(now,"%dd %H:%M:%S.%i"); 
                        }
                        s_send(m_socket,message);
                    }  
                }
            }
        }
};

// Sends a messages and exits after the acknowlegement arrives.
// Reconnects after rand() seconds and repeats the process.
class SupriousClient : public Poco::Task{
    private:
        Poco::Util::Application& m_app;
        zmq::socket_t m_socket;
    public:
        SupriousClient(zmq::context_t& ctx)
            :Task("SClient")
            ,m_app(Poco::Util::Application::instance())
            ,m_socket(ctx,ZMQ_DEALER)
    {}
        void runTask(){
        
        }
};


class ZMQService : public Poco::Util::ServerApplication{
    protected:
        void initialize(Poco::Util::Application& self){
            loadConfiguration(); 
            Poco::Util::ServerApplication::initialize(self);
            logger().information("Service Initialized.");
        }

        void uninitialize(){
            logger().information("Service Exiting.");
            Poco::Util::ServerApplication::uninitialize();
        }

    public:
        int main(const ArgVec& args){
            Poco::Util::Application& app = app.instance();
            Poco::ThreadPool thread_pool(2,app.config().getInt("test.threadpool.max"),app.config().getInt("test.threadpool.idle_seconds")); 
            Poco::TaskManager task_manager(thread_pool);
            try{
                zmq::context_t zmq_context(1);
                task_manager.start(new Client(zmq_context));
                //task_manager.start(new SupriousClient(zmq_context));
                waitForTerminationRequest();
            }catch(Poco::NotFoundException& e){
                app.instance().logger().fatal(e.what());
                return app.EXIT_CONFIG;
            }
            catch(Poco::NoThreadAvailableException& e){
                app.instance().logger().warning(e.what());
            }
            catch (...){
                app.instance().logger().fatal("Unexpected system error encountered.");
                return app.EXIT_OSERR;
            }
            task_manager.cancelAll();
            task_manager.joinAll();
            return app.EXIT_OK;
        }
};

POCO_SERVER_MAIN(ZMQService)
