#include<meta>
#include<iostream>
#include<queue>
#include<vector>
#include"Experimental/Meta.hpp"
namespace t{
	class Test{
		int member;
		int func(int arg){return 0;}
	};
}
enum class TestEnum{Hello,Goodbye};
int main()
{
	std::cout<<pc::meta::FullNameOfEnum(TestEnum::Hello)<<std::endl;
	std::cout<<pc::meta::Typename<std::vector<std::queue<int>>&>;
}