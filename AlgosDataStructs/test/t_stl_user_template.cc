#include <iostream>
#include <vector>
#include <list>

template <template<class,class> class Container, class Object>
class UDTContainer{
	private:
		Container<Object,std::allocator<Object> > m_cointainer;		
	public:

};
template <const char* in>
class Test{
	private:
		std::string m_str;
	public:
		Test();
		
};
int main(){

	UDTContainer<std::vector,std::string> obj_udtc;
}
