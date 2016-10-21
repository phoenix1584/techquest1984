#define BOOST_TEST_MODULE B_VARIANT

/*
* Objective
* Explore the boost::variant type and the associated implementation of visitor pattern (double dispatch)
* provided by boost. 
*/
#include <boost/test/unit_test.hpp>
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#if (BOOST_VERSION >= 105500)
#include <boost/variant/multivisitors.hpp>
#endif

#define PF_BTM BOOST_TEST_MESSAGE(__PRETTY_FUNCTION__);
#define BT_START BOOST_TEST_MESSAGE( "\n*****\n" << boost::unit_test::framework::current_test_case().p_name << "\n*******\n");
/// Test enum. Would be replaced by Protobug enums in production code/ later versions of this test suite.
enum COMMAND{
    REGISTER,
    GETLOGS,
    SETCONFIG,
    DIFFBEHAVIOR
};


template <COMMAND C> 
class Command{
    public:
        COMMAND m_cmd_id;
        std::string m_name;
        Command(std::string name)
            :m_cmd_id(C),
            m_name(name)
        {}
};

class T1{
    friend std::ostream& operator<<(std::ostream& os, const T1& t );
    public:
        std::string m_name;
        T1(std::string name)
            :m_name(name)
        {}
};

std::ostream& operator<<(std::ostream& os, const T1& t ){
    os << t.m_name;
    return os;
}

// Basic handler to demonstrate the handling of variant is defined with PODs and user defined classes
template <typename DATA=std::string>
class output : public boost::static_visitor<>
{
    private:
        // Data being accpeted as constructur argument. Allows type specific data to be passed while invoking the functor via apply_visitor();
        DATA m_data;
    public:
        output(DATA data):
            m_data(data)
            {}
        template<class ARG>
        void operator()(ARG arg) const { PF_BTM }
};

BOOST_AUTO_TEST_SUITE(variant_tests)
BOOST_AUTO_TEST_CASE(basic_test){
    BT_START
    boost::variant<double, char, std::string,T1,Command<REGISTER> > v;

    // Type : double 
    v = 3.14;
    boost::apply_visitor(output<>("output"), v);

    // Type : char  
    v = 'A';
    boost::apply_visitor(output<int>(1), v);

    // Type : string 
    v = "Boost";
    boost::apply_visitor(output<double>(3.14), v);

    // Type : User type T1 
    v = T1("TEST");
    boost::apply_visitor(output<T1>(T1("DUMMY")), v);

    // Type : Enum class wrapper object 
    v = Command<REGISTER>("Register");
    boost::apply_visitor(output<>("output"), v);
}

// Sample Data classes 
class RegisterData{
    private:
        std::string m_part1;
        std::string m_part2;
        double m_part3;
        int m_part4;
        std::string m_name;
    public:
        RegisterData()
            :m_name("REGISTER")
        {}
        void Deserialize(){ PF_BTM };
        void Serialize(std::string& input){ PF_BTM }
        void Execute(){ PF_BTM }
};

class GetLogsData{
    private:
        std::string m_part1;
        std::string m_part2;
        std::string m_name;
    public:
        GetLogsData():
            m_name("GETLOGS")
    {}
        void Deserialize(){ PF_BTM }
        void Serialize(std::string& input){ PF_BTM }
        void Execute(){ PF_BTM }
};

class SetConfigData{
    private:
        std::string part1;
        std::string m_name;
    public:
        SetConfigData()
            :m_name("SETCONFIGDATA")
        {}
        void Deserialize(){PF_BTM}
        void Serialize(std::string& input){ PF_BTM }
        void Execute(){ PF_BTM }
};

// Non conforming API.
class DiffBehavior{
    private:
        std::string part1;
        int part2;
        std::string m_name;
    public:
        DiffBehavior(int x)
            :part2(x),
            m_name("DIFFBEHAVIOR")
        {}
        void Diff_Deserialize(){ PF_BTM }
        void Diff_Execute(){ PF_BTM }
};

// Command oriented handler
class Handler : public boost::static_visitor<>
{
    public:
       // TODO : 
       // - Is it possible to get rid of the const from the function?? (Reduce data copy)
       // - Test the return type variation to allow serialized responses to be sent back from handler itself.
       //   Unfortunately the multivisitor to allow variant return types is not available in boost.1.48. But a
       //   closer look at documentation is needed to understand if that is completely impossible in the mentioned version.

       //// Option 1 : Individual implementation of handlers allows a flexible implementation of 
       //// data APIs. The compile time safety is still guarenteed but option of passing different
       //// types of argements is possible.
       //// Disadvantage being the verbosity of code and hence possibility of too many local variable 
       //// and those kind of issues (trivial but better if avoided altogether).
       //void operator()(const RegisterData& c_reg) const { BOOST_TEST_MESSAGE("REGISTER");} 
       //void operator()(SetConfigData& data) const { BOOST_TEST_MESSAGE("SETCONFIG");} 
       //void operator()(const GetLogsData& data) const { 
       //     BOOST_TEST_MESSAGE("GETLOGS");
       //     GetLogsData local_data = data;
       //     local_data.Deserialize();
       //} 
       
       //Option 2 : If the data objects are forced to have a common API then a template version could be used 
       // along with type specific specialization.
       template <class CMD_DATA>
       void operator()(const CMD_DATA& data) const{
            CMD_DATA local_copy = data;
            local_copy.Deserialize();
            local_copy.Execute();
            std::string tmp;
            local_copy.Serialize(tmp);
       }
       // Provided as a template specialization to allow handling of APIs that are not generic.
       // This way runtime polymorphism for data classes can be completely eliminated.
       // Specifically useful in cases where deserialization frameworks need to plugged based on data 
       // Eg. XML and protobuff
       void operator()(const DiffBehavior& data) const{
            DiffBehavior local_copy = data;
            local_copy.Diff_Deserialize();
            local_copy.Diff_Execute();
       } 
};


BOOST_AUTO_TEST_CASE(variant_map){
    BT_START
    // Declared for reducing errors while adding/removing types for incremental feature analysis.
    typedef boost::variant<GetLogsData,SetConfigData,RegisterData,DiffBehavior> DATA_VARIANT;

    // Map Command related data type in a map with the command as key.
    // CONS : The data objects will reside on the stack as part of the map. Although the basic PODs will take only 
    // a few bytes, it is still a cost paid for entire lifetime of the class object.
    // One possible solution is to store them as pointers to classes, but that require more secure memory allocation so 
    // the implementation needs to be considered carefully.
    std::map<COMMAND,DATA_VARIANT> m_var_map;

    m_var_map.insert(std::pair<COMMAND,RegisterData>(REGISTER,RegisterData()));
    m_var_map.insert(std::pair<COMMAND,GetLogsData>(GETLOGS,GetLogsData()));
    m_var_map.insert(std::pair<COMMAND,SetConfigData >(SETCONFIG,SetConfigData()));
    m_var_map.insert(std::pair<COMMAND,DiffBehavior >(DIFFBEHAVIOR,DiffBehavior(rand())));

    // BOOST_FOREACH and std::for_each could also be used here.
    for(std::map<COMMAND,DATA_VARIANT>::iterator itr = m_var_map.begin();itr != m_var_map.end() ; ++itr){
        boost::apply_visitor(Handler(),itr->second);
    }
}

// Variant handler with return type
class ReturnTypeHandler : public boost::static_visitor<std::string>{
    public:
        template <class IN>
        std::string operator()(IN& x) const{ 
            PF_BTM
            return boost::lexical_cast<std::string>(x);
        }
};

BOOST_AUTO_TEST_CASE(visitor_return){
    BT_START
    typedef boost::variant<int,double> TEST_VARIANT;
    TEST_VARIANT v;
    int i = 10;
    double d = 11.0;
    v = d;
    std::cout << boost::apply_visitor(ReturnTypeHandler(),v) << "\n";

    v = i;
    std::cout << boost::apply_visitor(ReturnTypeHandler(),v) << "\n";
}

// Variant handler using binary variant i.e two variant input parameters 
class BinaryHandler : public boost::static_visitor<>{
    // Implemented explicitly for illustration purposes.
    public:
        void operator()(int , double ) const{ PF_BTM }
        void operator()(double ,int ) const{ PF_BTM }
        void operator()(double ,double ) const{ PF_BTM }
        void operator()(int ,int ) const{ PF_BTM }
};


BOOST_AUTO_TEST_CASE(binary_visitor){
    BT_START
    typedef boost::variant<int,double> TEST_VARIANT;
    TEST_VARIANT v1,v2;
    int i = 10;
    double d = 11.0;
    v1 = d;
    v2 = i;
    boost::apply_visitor(BinaryHandler(),v1,v2) ;
    boost::apply_visitor(BinaryHandler(),v2,v1) ;
    boost::apply_visitor(BinaryHandler(),v1,v1) ;
    boost::apply_visitor(BinaryHandler(),v2,v2) ;
}

#if (BOOST_VERSION >= 105500)
// NOT AVAILABLE WITH BOOST 1.48
// EXPERIMENTAL : To prove the cost of going overboard with variants !!!
// Demands simialr handling like default in a switch-case.
class TertiaryHandler : public boost::static_visitor<>{
    public:
        template<class U, class V, class W>
        void operator()(U ,V ,W) const { PF_BTM }
        
        template<class U>
        void operator()(U ,U ,U) const { PF_BTM }
       
        // One class twice
        // 0,1,1
        // 1,0,1
        // 1,1,0
        template<class U, class V>
        void operator()(U ,V ,V) const { PF_BTM }
        template<class U, class V>
        void operator()(V ,U ,V) const { PF_BTM }
        template<class U, class V>
        void operator()(V ,V ,U) const { PF_BTM }

};

BOOST_AUTO_TEST_CASE(tertiary_variant){
    BT_START
    typedef boost::variant<int,double,std::string> TEST_VARIANT;
    TEST_VARIANT v_int(10),v_dob(23.5933),v_str("TEST_STRING");
    boost::apply_visitor(TertiaryHandler(),v_int,v_dob,v_str) ;
    boost::apply_visitor(TertiaryHandler(),v_int,v_int,v_str) ;
    boost::apply_visitor(TertiaryHandler(),v_str,v_dob,v_str) ;
    boost::apply_visitor(TertiaryHandler(),v_int,v_dob,v_dob) ;
    boost::apply_visitor(TertiaryHandler(),v_int,v_int,v_int) ;

    // Ways to access values to comapre with PODs or user types.
    BOOST_CHECK_EQUAL(boost::get<int>(v_int),10);
    BOOST_CHECK_EQUAL(boost::get<double>(v_dob),23.5933);
    BOOST_CHECK(!boost::get<std::string>(v_str).compare("TEST_STRING"));
}

#endif
BOOST_AUTO_TEST_SUITE_END()
