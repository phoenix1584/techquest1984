#include <iostream>
#include <vector>
#include <list>

template <template<class,class> class Container, class Object>
class UDTContainer{
	private:
		Container<Object,std::allocator<Object> > m_cointainer;		
	public:

};

int main(){

	UDTContainer<std::vector,std::string> obj_udtc;
}
