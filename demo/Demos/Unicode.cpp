#include"Utility/Unicodes.hpp"
#include<format>
#include<iostream>
int main(){
	std::string a;
	a=pc::uni::GetUTF();
	std::u32string c=pc::uni::UTF8to32(a);
	int b;
	while(true){
		std::cin>>b;
		auto [code,read]=pc::uni::Char8CodeAt(a,b);
		pc::uni::PrintLnUTF("a:<{}>[{:#x}] {} bytes read",
			pc::uni::ParseCode8(code),code,read);
		pc::uni::PrintLnUTF("c:<{}>[{:#x}]",c[b],int(c[b]));
	}
}