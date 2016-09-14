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

template<const char* NAME> 
class Name{
    std::string m_name;
    public:
        Name()
        :m_name(NAME)
        {}
};

int main()
{
    Test<int> ti(10); 
    Test<std::string> ts("TEST");
    Test<void> td;
    Name<"TEST"> mn;
}
