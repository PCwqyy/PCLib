#include<string>
#include<iostream>
#include<print>
#include"../../src/Container/Color.hpp"
#include"../../src/Utility/Ansi.hpp"
int main()
{
	std::println("Color demo program");
	std::println("PClib version: 26v2a");
	std::println("Input a color");
	Color c;
	std::string a;
	while(true){
		std::print(">");
		std::getline(std::cin,a);
		try{
			c=a;
			AnsiPrint("%Cb[#{0:06x}]  %/ #{0:06x} rgb({1},{2},{3})\n",
				c.toHex(),c.R,c.G,c.B);
		}catch(pc::Exception& xpt){
			AnsiPrint("%Cf[red]{}%/\n",xpt.what());
			xpt.HandleAnyway();
		}
	}
	return 0;
}