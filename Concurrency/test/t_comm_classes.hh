#ifndef COMM_CLASSES_HH
#define COMM_CLASSES_HH
#include "zmq.hpp"
#include "zhelpers.hpp"
#include <string>
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define VERSION 1
#define HW "Hello World"
#define OK "Okay"

//FIXME : Decide the serialization mechanism ASAP.
namespace test_comm{

    enum MESSAGE_TYPE : int{
        MESSAGE_BASE,
        MESSAGE_CONTAINER,
        MESSAGE_HEADER
    };

    // In ascending order to allow natural integral comparisons.
    enum MESSAGE_PRIORITY: int{
        PR_INFO,
        PR_LOW,
        PR_NORMAL,
        PR_HIGH,
        PR_IMMEDIATE
    };

    /**
      * Message header class. Conatins all meta data necessary to compare tow message or route a message.
      */ 
    class MessageHeader{
        private:
            friend class boost::serialization::access;
            template<class Archive>
                void serialize(Archive & ar, const unsigned int version){
                    ar & const_cast<std::string &> (m_src_id);
                    ar & const_cast<std::string &> (m_dst_id);
                    ar & const_cast<MESSAGE_PRIORITY &> (m_priority);
                }
            std::string GetTime(){
                time_t raw_time;
                time(&raw_time);
                return (asctime(localtime(&raw_time)));
            }
        public:
            const MESSAGE_TYPE m_type;
            const std::string m_src_id;
            const std::string m_dst_id;
            const MESSAGE_PRIORITY m_priority;
            //FIXME : Ideally this should be replaced by appropriate event ID or token ID.
            // time is just a place holder for now but given the time sync issues it has
            // to be replaced in final design. 
            // Also adding event ID will enable tracing life cycle of events from logs by turning on
            // the header logging. Something similar to grep on dmsg or syslogs.
            //const std::string m_timestamp;
            MessageHeader(const std::string& src_id,
                    const std::string& dst_id, const MESSAGE_PRIORITY priority=PR_NORMAL):
                m_type(MESSAGE_HEADER)
                ,m_src_id(src_id),m_dst_id(dst_id),m_priority(priority)
        {}
            MessageHeader(std::stringstream& msg_ss):
                m_type(MESSAGE_HEADER),m_priority(PR_NORMAL){
                    boost::archive::binary_iarchive deserializer(msg_ss);
                    deserializer >> *this; 
                }

            std::string Serialize(){
                std::stringstream ss;
                boost::archive::binary_oarchive serializer(ss);
                serializer << *this;
                return ss.str();
            }

            std::string ToString(){
                std::stringstream ss;
                ss << "Type : " << m_type 
                    << ", Priority : " << m_priority
                    << " ,Src : " << m_src_id 
                    << " ,Dst : " << m_dst_id;
                return ss.str();
            }

            /**
            * Define the comparison criterion for messages.
            * Used in priority queue dedicate for the ZMQ sockets
            */
            class MessageCompare{
                public:
                    bool operator()(const MessageHeader& lhs,const MessageHeader& rhs) const {
                        // TODO: More rules to be added here to increase the probability of 
                        // unique messages and minimal chances of collision in heap.
                        return (!(lhs.m_priority > rhs.m_priority));
                    }
            };
    };


    /**
      *  TODO:Could property trees be used here to allow a more generic variable length data transport ??
      */
    class MessagePayload{
        private:
        public:
    };

    /**
      *  FIXME: Could this class be a variadic template ??
      */
    class Message{
        private:
            friend class boost::serialization::access;
            template<class Archive>
                void serialize(Archive & ar, const unsigned int version){
                    ar & m_type;
                    ar & m_src_id;
                    ar & m_dst_id;
                    ar & m_timestamp;
                    ar & m_fixed_data_1;
                    ar & m_fixed_data_2;
                }
        public:
            MESSAGE_TYPE m_type;
            std::string m_src_id;
            std::string m_dst_id;
            std::string m_timestamp;
            int32_t m_fixed_data_1;
            int64_t m_fixed_data_2;
            void GetMessage(std::unique_ptr<zmq::message_t>&);
            void ExtractMessage(const zmq::message_t&);
            virtual ~Message() = default;
            Message(const Message &) = default;
            Message& operator=(const Message&) = default;
            Message(MESSAGE_TYPE type = MESSAGE_BASE):
                m_type(type)
        {};
    };


    void Message::GetMessage(std::unique_ptr<zmq::message_t>& zmq_input){
        std::stringstream ss;
        boost::archive::binary_oarchive serializer(ss);
        serializer << *this;
        zmq_input.reset(new zmq::message_t(ss.str().c_str(),ss.str().length())); 
    }

    void Message::ExtractMessage(const zmq::message_t& zmq_msg)
    {
        std::stringstream ss(std::string(static_cast<const char *>(zmq_msg.data()),zmq_msg.size())) ;
        boost::archive::binary_iarchive deserializer(ss);
        deserializer >> *this; 
    }
    
    /**
      * Serializable STL container embedded message class.
      */ 
    template <class T>
        class Message_Container : public Message{
        private:
            friend class boost::serialization::access;
            template<class Archive>
                void serialize(Archive & ar, const unsigned int version){
                    ar & m_container;
                }
        public:
            T m_container;
            zmq::message_t GetMessage();
            Message_Container():
                Message(MESSAGE_CONTAINER)
        {}
            ~Message_Container(){};
    };

    /**
      * ZeroMQ wrapper class to encapsulate ZMQ context and socket initialization.
      */
    class ZMQBase{
        protected:
            zmq::context_t m_context;
            zmq::socket_t m_socket;
        public:
            ZMQBase(int type):
                m_context(1),
                m_socket(m_context,type)
        {}
            virtual ~ZMQBase() = 0;
            template <class T>
                void Send(T) ;
            template <class T>
                void Recv(std::unique_ptr<T>& );
    };

    template <class T>
        void ZMQBase::Send(T msg){
            std::unique_ptr<zmq::message_t> zmq_msg;
            msg.GetMessage(zmq_msg);
            m_socket.send(*(zmq_msg.get()));
        }

    template <class T>
        void ZMQBase::Recv(std::unique_ptr<T>& msg){
            zmq::message_t zmq_msg;
            m_socket.recv(&zmq_msg);
            msg->ExtractMessage(zmq_msg);
        }

    ZMQBase::~ZMQBase()
    {}

    /**
      * Server class derived from ZMQBase.
      */
    class Server: public ZMQBase{
        public:
            Server(int socket_type):
                ZMQBase(socket_type)
        {}
            void Execute();
            void Publish();
    };

    void Server::Execute(){
        //  Prepare our context and socket
        m_socket.bind ("tcp://*:5555");
        auto count = 0;
        while (count < 10) {
            std::unique_ptr<Message> msg(new Message); 
            Recv<Message>(msg);
            BOOST_CHECK( msg.get()->m_src_id == HW );
            //  Do some 'work'
            sleep(1);
            //  Send reply back to client
            msg.get()->m_src_id = OK;
            Send<Message>(*(msg.get()));
            ++count;
        } 
    }

    void Server::Publish(){
        m_socket.bind("tcp://*:5556");
        auto count = 5;
        time_t raw_time;
        struct tm * timeinfo;
        while (count--)
        {
            time(&raw_time);
            timeinfo = localtime(&raw_time);
            std::string time_str(asctime(timeinfo));
            zmq::message_t zmq_msg(time_str.c_str(),time_str.length());
            m_socket.send(zmq_msg);
            sleep(2);
        }
        std::string quit("QUIT");
        zmq::message_t zmq_msg_quit(quit.c_str(),quit.length());
        m_socket.send(zmq_msg_quit);
    }

    /**
     * Client class derived from ZMQBase.
     */
    class Client: public ZMQBase{
        public:
            Client(int socket_type):
                ZMQBase(socket_type)
        {}
            void Execute();
            void Subscribe();
    };

    void Client::Execute(){
        m_socket.connect ("tcp://localhost:5555");

        //  Do 10 requests, waiting each time for a response
        for (auto request_nbr = 0; request_nbr != 10; request_nbr++) {
            std::unique_ptr<Message> msg(new Message); 
            msg.get()->m_src_id = HW;
            Send<Message>(*(msg.get()));

            //  Get the reply.
            Recv<Message>(msg);
            BOOST_CHECK( msg.get()->m_src_id == OK );
        } 
    }

    void Client::Subscribe(){
        m_socket.connect("tcp://localhost:5556");
        m_socket.setsockopt(ZMQ_SUBSCRIBE,"",0);
        while(true){
            zmq::message_t update;
            m_socket.recv(&update);
            std::string message(static_cast<const char*>(update.data()),update.size());
            if(strcmp(message.c_str(),"QUIT")){
                BOOST_TEST_MESSAGE("Time update received : " << message);
            }
            else{
                BOOST_TEST_MESSAGE("Quit message received.");
                break ;
            }
        }
    }
}
#endif /* COMM_CLASSES_HH */
BOOST_CLASS_VERSION(test_comm::Message,VERSION);
BOOST_CLASS_VERSION(test_comm::MessageHeader,VERSION);
