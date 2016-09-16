#ifndef POCO_LOGGER_HH__
#define POCO_LOGGER_HH__

#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"

#include "boost/filesystem.hpp"

using Poco::Logger;
using Poco::PatternFormatter;
using Poco::Channel;
using Poco::FormattingChannel;
using Poco::FileChannel;
using Poco::Message;
using Poco::Formatter;
using Poco::AutoPtr;

//FIXME
// -- Reusing a logger name while reintilialzing a logger object.
namespace utils{
    class POCOLogger{
        private:
            std::string m_name;
            AutoPtr<Formatter> m_file_formatter;
            AutoPtr<FileChannel> m_file_channel;
            AutoPtr<Channel> m_formatting_channel;
        public:
            Logger& m_file_logger;
            POCOLogger(std::string file_path,std::string name):
                m_name(name)
                ,m_file_formatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:%t"))
                ,m_file_channel (new FileChannel(file_path.c_str()))
                ,m_formatting_channel (new FormattingChannel(m_file_formatter,m_file_channel))
                ,m_file_logger(Logger::create(m_name.c_str(), m_formatting_channel, Message::PRIO_INFORMATION))
            {
                m_file_logger.information("Logger created successfully");
            }
            
            void SetLevel(int level){
               Logger::setLevel(m_name.c_str(),level);
            }
    };
};

#endif /* POCO_LOGGER_HH__ */
