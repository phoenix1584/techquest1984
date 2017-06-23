#ifndef GALACTIC_EXCPETIONS_HH
#define GALACTIC_EXCPETIONS_HH

#include <exception>
#include <string>

namespace galaxy_trade{
    /**
     *  UFO base exception class.
     */
    class BaseException : public std::exception
    {
        protected:
            /// Exception name to append to message in case the catch block is generic.
            /// Recommended format : "[<class name>]:"
            std::string m_exception_name;

            /// Custom error string to be defined by deriving classes as required.
            /// This will be by default appended with Exception class name.
            std::string m_error;

            /// Custom error message that needs to be displayed on screen.
            std::string m_display_message;

            /// Constructor
            /// \param[in] exception_name - name of the exception used for logging
            /// \param[in] message - string to be used for logging purposes.
            BaseException(std::string exception_name, std::string message) 
                :m_exception_name(exception_name)
                ,m_display_message(message)
            {
                m_error = "["+ m_exception_name +"]: "+ message ;
            }
            
            /// Constructor
            /// \param[in] exception_name -string to be used for logging purposes.
            /// \param[in] message - string to be used for logging purposes.
            /// \param[in] display_message - string to be used for logging purposes.
            BaseException(std::string exception_name, std::string message,std::string display_message) 
                : m_exception_name(exception_name)
                , m_display_message(display_message)
            {
                m_error = "["+ m_exception_name +"]: "+ message ;
            }

        public:
            /// Destructor
            ~BaseException() throw(){};

            /** Overriding the std::exceptions what() function.
             *  Returns a pointer to the (constant) error description.
             *  \return A pointer to a const char*. The underlying memory
             *          is in posession of the Exception object. Callers must
             *          not attempt to free the memory.
             */
            virtual const char* what() const throw()
            {
                return m_error.c_str();
            }

            /** Get the error string only without the class name appended.
            * \return const char * - pointer to the display message string
            */
            virtual const char * getdisplaymessage() const
            {   return m_display_message.c_str();  }
    };
    
    /// Exception class to indicate conversion related exceptions.
    /// Should be caught and corresponding query processing should be terminated.
    class RomanCoversionException : public BaseException{
        public:
            RomanCoversionException(std::string message)
                :BaseException("RomanCoversionException",message)
            {}

            ~RomanCoversionException(){}
    };
    
    /// Exception class to indicate query parsing exception
    class QuerySyntaxException : public BaseException{
        public:
            QuerySyntaxException(std::string message)
                :BaseException("QuerySyntaxException",message)
            {}

            ~QuerySyntaxException(){}
    };

};

#endif /*GALACTIC_EXCPETIONS_HH*/
