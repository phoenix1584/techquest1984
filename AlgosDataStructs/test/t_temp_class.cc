#include <iostream>

template <class T,class U = int>
class Test{
    public:
        Test(T t, U u){}

        Test(T t){}
};

template<>
class Test<void>{
    public:
        Test(){}
};

template<typename NAME=const char*> 
class Name{
    std::string m_name;
    public:
        Name(NAME name)
        :m_name(name)
        {}
};

int main()
{
    Test<int> ti(10); 
    Test<std::string> ts("TEST");
    Test<void> td;
    Name<> mn("TEST");
}
