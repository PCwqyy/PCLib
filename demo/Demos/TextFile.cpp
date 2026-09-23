#include"Utility/File.hpp"
#include"Container/Color.hpp"
#include"Utility/Ansi.hpp"
#include<iostream>
int main()
{
tagStart:
try{
	pc::fl::Text flTest(pc::fl::Mode::Attach,"test.txt");
	std::string a,b;
	while(true){
		std::cin>>a;
		std::getline(std::cin,b);
		if(a=="exit")	break;
		else if(a=="write")
			flTest.Print("Line:{}\n",b);
		else if(a=="seek")
			flTest.SeekWriteCur(std::stoi(b));
		else if(a=="pos")
			AnsiPrint("%Cf[cyan]Position: {}\n",flTest.GetWritePos());
		else if(a=="print")
			AnsiPrint("%Cf[gold]{}\n",flTest.ReadAll());
		else if(a=="get")
			AnsiPrint("%Cf[gold]Word: {}\n",flTest.GetWord());

		flTest.Flush();
	}
}catch(pc::Exception& xpt){
	AnsiPrint("%Cf[red]{}",xpt.what());
	xpt.HandleAnyway();
	goto tagStart;
}
}