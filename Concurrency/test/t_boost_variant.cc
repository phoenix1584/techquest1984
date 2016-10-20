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

#define BOOST_TEST_MODULE B_VARIANT

/*
* Objective
* Explore the boost::variant type. 
*/
#include <boost/test/unit_test.hpp>
#include <boost/variant.hpp>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

/// Test enum. Would be replaced by Protobug enums in production code/ later versions of this test suite.
enum COMMAND{
    REGISTER,
    GETDATA,
    SETDATA
};

class C_REGISTER{
    private:
        COMMAND cmd;
    public:
        C_REGISTER()
            :cmd(REGISTER)
        {}
        C_REGISTER(const C_REGISTER& C){
            this->cmd = C.cmd;
        }
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

class C_GETDATA{
    private:
        COMMAND cmd;
    public:
        C_GETDATA()
            :cmd(GETDATA)
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

template <typename DATA=std::string>
class output : public boost::static_visitor<>
{
    private:
        DATA m_data;
    public:
        output(DATA data):
            m_data(data)
            {}
        void operator()(double d) const { std::cout << d << '\n'; }
        void operator()(char c) const { std::cout << c << '\n'; }
        void operator()(std::string s) const { std::cout << s << '\n'; }
        void operator()(T1 t) const { std::cout << t.m_name << ", DATA : " << m_data << '\n'; }
        void operator()(C_REGISTER c_reg) const { std::cout << "C_REGISTER" << '\n'; }
        void operator()(Command<REGISTER> c_reg) const { std::cout << c_reg.m_name << '\n'; }
};

BOOST_AUTO_TEST_SUITE(variant_tests)
BOOST_AUTO_TEST_CASE(basic_test){
    boost::variant<double, char, std::string,T1,C_REGISTER,Command<REGISTER> > v;
    v = 3.14;
    boost::apply_visitor(output<>("output"), v);
    v = 'A';
    boost::apply_visitor(output<int>(1), v);
    v = "Boost";
    boost::apply_visitor(output<double>(3.14), v);
    v = T1("TEST");
    boost::apply_visitor(output<T1>(T1("DUMMY")), v);
    v = Command<REGISTER>("Register");
    boost::apply_visitor(output<>("output"), v);
}

BOOST_AUTO_TEST_CASE(variant_map){
    std::map<int,boost::variant<double,char,std::string,Command<REGISTER> > > m_var_map;
    m_var_map.insert(std::pair<int,Command<REGISTER> >(REGISTER,Command<REGISTER>("Register")));
    m_var_map.insert(std::pair<int,std::string>(GETDATA,"TEST"));
    m_var_map.insert(std::pair<int,double >(SETDATA,3.14));
    // BOOST_FOREACH and std::for_each could also be used here.
    for(std::map<int,boost::variant<double,char,std::string,Command<REGISTER> > >::iterator itr = m_var_map.begin();itr != m_var_map.end() ; ++itr){
        boost::apply_visitor(output<>("output"),itr->second);
    }
}
BOOST_AUTO_TEST_SUITE_END()
