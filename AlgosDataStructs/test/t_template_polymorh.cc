#include <iostream>
#include <vector>

template <template <typename>class Impl, class Param_list>
class APIExector{
	private:
		Impl<Param_list> m_impl;
	public:
		APIExector(Param_list p)
			:m_impl(p)
		{}
		void Execute()
		{ m_impl.Run(); }
		void Reload(const Param_list& p)
		{ m_impl.Configure(p); }
		
};

template <class Param_list>
class Variant1{
	private:
		Param_list m_param_list;
	public:
		Variant1(Param_list pl)
			:m_param_list(pl)
		{std::cout << "Variant1 : c'tor." << std::endl; }
		/* No Run() is required here unlike the virtual inheritance case */
		void Configure(const std::vector<std::string>){std::cout << "Variant1 : vector<string> configurator." << std::endl;}
};

template <class Param_list>
class Variant2{
	private:
		Param_list m_param_list;
	public:
		Variant2(Param_list pl)
			:m_param_list(pl)
		{std::cout << "Variant2 : Vector<int> c'tor" << std::endl; }
		void Run(){std::cout << "Variant2 : Run ()" << std::endl; }
};

int main(){
	APIExector<Variant1,std::string> var1("Variant1");
	APIExector<Variant1,std::vector<std::string> > var1_1(std::vector<std::string>(0));
	var1_1.Reload(std::vector<std::string>(0));

	APIExector<Variant2,std::vector<int> > var2(std::vector<int>(0));
	var2.Execute();
}
