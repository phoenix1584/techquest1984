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


#define BOOST_TEST_MODULE  CMD_HANDLERS
#include <boost/test/unit_test.hpp>
#include <boost/assign.hpp>
#include <map>
#include <string>
#include <boost/variant.hpp>

/*
* Objective
* Test options for command handler registry objects.
* FIXME : Not integrable with runtime as of now. Provides just the hardwired backend !!
* Boost vault has a possible solution for this calle Boost.Enum but given the untested nature of source it shoule NOT be used.
* Ref : https://github.com/flaub/boost-enum
*/

/// Test enum. Would be replaced by Protobug enums in production code/ later versions of this test suite.
enum COMMAND{
    REGISTER =1,
    GETDATA,
    SETDATA,
    RESET,
    UNKOWN
};

/// A class that converts enum values into strong type by using the non-type parameter way of template instansiation
/// This also poses a problem of storing all Command objects in a squence. Since all have become different types.
template <COMMAND C> 
class Command{
    public:
        COMMAND m_cmd_id;
        std::string m_name;
        Command(std::string name)
            :m_cmd_id(C),
            m_name(name)
        {}
        Command(const Command& c){
            m_cmd_id = c.m_cmd_id;
            m_name = c.m_name;
        }
};

template <COMMAND CMD>
class VariantCommand{
    private:
        COMMAND m_cmd_id;
    public:
        VariantCommand():
            m_cmd_id(CMD)
    {}
};


/// Handler implementation class that is similar to switch case but implemented in type safe manner
class RegisterHandler{
    private:
        void Print(const std::string& str ) const{
            BOOST_TEST_MESSAGE("Handler for " << str);  
        }

    public:
        //void operator()(Command<REGISTER>& cmd){
        //    Print(cmd.m_name);
        //}
        void operator()(Command<REGISTER> cmd){
            Print(cmd.m_name);
        }
        void operator()(Command<GETDATA>& cmd){
            Print(cmd.m_name);
        }
        void operator()(Command<RESET>& cmd){
            Print(cmd.m_name);
        }
};
/// Alternate class for showing pluggable nature of handler classes.
class AltHandler{
    private:
        void Print(const std::string& str ) const{
            BOOST_TEST_MESSAGE("Alternate Handler for " << str);  
        }

    public:
        void operator()(Command<REGISTER>& cmd){
            Print(cmd.m_name);
        }
        void operator()(Command<GETDATA>& cmd){
            Print(cmd.m_name);
        }
        void operator()(Command<RESET>& cmd){
            Print(cmd.m_name);
        }
};

/// Handler class that will handle variants

/// Executor interface that decouples the handler implementation from the command object.
template <class T,class H>
class Executor{
    private:
       T m_command; 
    public:
       Executor(T cmd)
           :m_command(cmd)
       {}
       void Execute(H handler){
            handler(m_command);  
       }
};

typedef  boost::variant<VariantCommand<REGISTER>,VariantCommand<GETDATA>,VariantCommand<RESET>,VariantCommand<SETDATA>,VariantCommand<UNKOWN> > USER_VARIANT;

/// Static enum to string registry. Aides in consitent logging across all modules.
class NameRegistry{
    private:
        std::map<COMMAND,std::string> m_registry;
        std::map<COMMAND,USER_VARIANT> m_cmd_registry;
    public:
        NameRegistry(){
            m_registry = boost::assign::map_list_of(REGISTER, std::string("REGISTER"))(GETDATA, std::string("GETDATA"))(SETDATA, std::string("SETDATA"))(RESET,std::string("RESET"))(UNKOWN,std::string("UNKOWN"));
            m_cmd_registry.insert(std::pair<COMMAND,VariantCommand<REGISTER> >(REGISTER, VariantCommand<REGISTER>()));
            m_cmd_registry.insert(std::pair<COMMAND,VariantCommand<GETDATA> >(GETDATA, VariantCommand<GETDATA>()));
            m_cmd_registry.insert(std::pair<COMMAND,VariantCommand<SETDATA> >(SETDATA, VariantCommand<SETDATA>()));
            m_cmd_registry.insert(std::pair<COMMAND,VariantCommand<RESET> >(RESET, VariantCommand<RESET>()));
            m_cmd_registry.insert(std::pair<COMMAND,VariantCommand<UNKOWN> >(UNKOWN, VariantCommand<UNKOWN>()));
        }
        std::string GetName(COMMAND id) {
            return m_registry[id];
        }

        void GetCommandObject(COMMAND id, USER_VARIANT& cmd_obj){
            cmd_obj = m_cmd_registry[id];
        }

        const std::map<COMMAND,USER_VARIANT>& GetCommandRegistry() const {
            return m_cmd_registry;
        }
};

class VariantHandler : public boost::static_visitor<>{
    private:
        void Print(const std::string& str ) const{
            BOOST_TEST_MESSAGE("Variant Handler for " << str);  
        }

    public:
        void operator()(VariantCommand<REGISTER> cmd) const {
            Print("REGISTER");
        }
        void operator()(VariantCommand<GETDATA> cmd) const {
            Print("GETDATA");
        }
        void operator()(VariantCommand<RESET> cmd) const {
            Print("RESET");
        }
        void operator()(VariantCommand<SETDATA> cmd) const {
            Print("SETDATA");
        }
        void operator()(VariantCommand<UNKOWN> cmd) const {
            Print("UNKOWN");
        }
};
/// FIXME/ TODO : Code replication of handlers
/// Handlers encapsulate the prduct/platform specific logic and are mutually exclusive.
/// Compile time separation could be acheived using make MACRO flags.
/// But the code bloat will still be an issue. Writing product specific handlers in separate headers 
/// could solve the issue further but not from a implementor's perspective.
/// Some basic boiler plate might be a good start towards building USERPlatform SDK suite of classes.
class VHandler_Prod1 : public boost::static_visitor<>{
    private:
        void Print(const std::string& str ) const{
            BOOST_TEST_MESSAGE("Product 1 specific Variant Handler for " << str);  
        }

    public:
        void operator()(VariantCommand<REGISTER> cmd) const {
            Print("REGISTER");
        }
        void operator()(VariantCommand<GETDATA> cmd) const {
            Print("GETDATA");
        }
        void operator()(VariantCommand<RESET> cmd) const {
            Print("RESET");
        }
        void operator()(VariantCommand<SETDATA> cmd) const {
            Print("SETDATA");
        }
        void operator()(VariantCommand<UNKOWN> cmd) const {
            Print("UNKOWN");
        }
};

/// Test function that could be used as a generic message handler function.
template <COMMAND CMD,class HANDLER>
void TestFunction(NameRegistry& nm_reg,HANDLER& handler){
   Executor<Command<CMD>,HANDLER> test_exec(Command<CMD>(nm_reg.GetName(CMD)));
   test_exec.Execute(handler);
}

/// boost::apply_visitor calls need to be isolated carefully for sake of readability and maintenance.
/// The core call is the boost::apply_visitor, variations of functions with Handler classes as templates 
/// could be written as necessary.
template<class HANDLER>
void RuntimeSimulator(const std::map<COMMAND,USER_VARIANT>& cmd_reg ){
    const int count = std::rand()%10 + 1;
    BOOST_TEST_MESSAGE("Runtime command simulation count : " << count);
    for(int i = 0 ; i < count ; i++){ 
        std::map<COMMAND,USER_VARIANT>::const_iterator cmd_itr = cmd_reg.find(COMMAND(rand()%4 + 1));
        if(cmd_itr != cmd_reg.end()){
            // At runtime the call in message handlers will be as follows
            boost::apply_visitor(HANDLER(),cmd_itr->second);
        }
    }
}

BOOST_AUTO_TEST_SUITE(cmd_handlers)
BOOST_AUTO_TEST_CASE(access_test){
   NameRegistry name_registry;
   RegisterHandler reg_handl;
   AltHandler alt_handl;

   // Manual instantiation of the executor class
   Executor<Command<GETDATA>,RegisterHandler> test_exec_1(Command<GETDATA>(name_registry.GetName(GETDATA)));
   test_exec_1.Execute(reg_handl);
   Executor<Command<RESET>,RegisterHandler> test_exec_2(Command<RESET>(name_registry.GetName(RESET)));
   test_exec_2.Execute(reg_handl);

   // Shows he pluggable option of handler objects as long as API compliance is maintained by classes.
   TestFunction<REGISTER,RegisterHandler>(name_registry,reg_handl);
   TestFunction<REGISTER,AltHandler>(name_registry,alt_handl);

}

BOOST_AUTO_TEST_CASE(variant_handler){
    NameRegistry name_registry;
    const std::map<COMMAND,USER_VARIANT>& cmd_reg = name_registry.GetCommandRegistry();
    for(std::map<COMMAND,USER_VARIANT>::const_iterator itr = cmd_reg.begin() ; itr != cmd_reg.end() ; ++itr){ 
        boost::apply_visitor(VariantHandler(),itr->second);
    }
    BOOST_TEST_MESSAGE("\nHandler implementation 1 ...");
    RuntimeSimulator<VariantHandler>(cmd_reg);
    
    BOOST_TEST_MESSAGE("\nHandler implementation 2 ...");
    RuntimeSimulator<VHandler_Prod1>(cmd_reg);

}
BOOST_AUTO_TEST_SUITE_END()
