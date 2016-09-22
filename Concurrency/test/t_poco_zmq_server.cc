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

class Worker : public Poco::Task{
    private:
        Poco::Util::Application& m_app;
        zmq::socket_t m_socket;
        // Lot of receive related utilities are available with ZMQ.
        // For PoC, some have been written out explicitly for readability and understanding.
        void MessageHandler(){
            std::deque<std::string> message;
            int more = 0;
            size_t more_size = sizeof(more);
            do{
                message.push_back(s_recv(m_socket));
                m_socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);
            }while(more);
            m_app.logger().information(message[1]); 
            
            // Sample acknowledgement for testing
            m_socket.send(message[0].c_str(),message[0].length(),ZMQ_SNDMORE);
            std::string delim("");
            m_socket.send(delim.c_str(),delim.length(),ZMQ_SNDMORE);
            m_socket.send(message[1].c_str(),message[1].length(),ZMQ_SNDMORE);
            std::string response((rand() % 7) ? "NOT_OK" : "OK");
            m_socket.send(response.c_str(),response.length());
        }
    public:
        Worker(std::string peer_name,zmq::context_t &ctx)
            :Task(peer_name)
             ,m_app(Poco::Util::Application::instance())
             ,m_socket(ctx,ZMQ_DEALER)
    {}

        void runTask(){
            s_catch_signals();
            m_socket.connect("inproc://backend");
            m_app.logger().information("Worker task with name %s started",name());
            zmq::pollitem_t items[] = {{static_cast<void *>(m_socket), 0, ZMQ_POLLIN, 0}};
            while(!s_interrupted){
                zmq::poll(items, 1, 10);
                    if (items[0].revents & ZMQ_POLLIN) {
                    MessageHandler();
                }
            }
        }
};

class ZMQCore : public Poco::Task {
    private:
        zmq::socket_t m_frontend;
        zmq::socket_t m_backend;
        std::string m_identity;
        
    public:
        ZMQCore(zmq::context_t & ctx)
            :Task(Poco::Util::Application::instance().config().getString("test.server.name"))
            ,m_frontend(ctx,ZMQ_ROUTER)
            ,m_backend(ctx,ZMQ_DEALER)
            ,m_identity(Poco::Util::Application::instance().config().getString("test.server.name"))
        {}
        void runTask(){
            Poco::Util::Application& app = Poco::Util::Application::instance();
            m_frontend.bind(app.config().getString("test.server.address"));
            m_backend.bind("inproc://backend");
            app.logger().information("Starting proxy...");
            zmq::proxy(static_cast<void *>(m_frontend), static_cast<void *>(m_backend), NULL);
        }
};

class ZMQProxy : public Poco::Util::ServerApplication{
    protected:
        void initialize(Poco::Util::Application& self){
            loadConfiguration(); 
            Poco::Util::ServerApplication::initialize(self);
            logger().information("Server Initialized.");
        }

        void uninitialize(){
            logger().information("Server Exiting.");
            Poco::Util::ServerApplication::uninitialize();
        }

    public:
        int main(const ArgVec& args){
            Poco::Util::Application& app = app.instance();
            Poco::ThreadPool thread_pool(2,app.config().getInt("test.threadpool.max"),app.config().getInt("test.threadpool.idle_seconds")); 
            Poco::TaskManager task_manager(thread_pool);
            try{
                zmq::context_t zmq_context(1);
                task_manager.start(new ZMQCore(zmq_context));
                int worker_id = 1;
                while(thread_pool.available()){
                    task_manager.start(new Worker(boost::lexical_cast<std::string>(worker_id),zmq_context));
                    ++worker_id;
                }
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

POCO_SERVER_MAIN(ZMQProxy)
