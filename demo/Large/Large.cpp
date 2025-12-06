#include<iostream>
#include<print>
#include<string>
#include"../../src/Math/Large.hpp"
using std::string;
int main()
{
	Large<1000> a,b,c,d,e,f;
	int start;
	std::print("Large demo program\n");
	std::print("PClib version: 25v22a\n");
	std::print("Input a,b:\n");
	while(true)
	{
		std::print(">");
		std::cin>>a>>b;
		start=clock();
		c=a+b,d=a-b,e=a*b,f=a/b;
		std::print("Calc for {}ms\n",clock()-start);
		std::print("[+]{}\n[-]{}\n[*]{}\n[/]{}\n",c,d,e,f);
	}
	return 0;
}