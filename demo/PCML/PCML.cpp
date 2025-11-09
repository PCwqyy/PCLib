#include<iostream>
#include"../../src/TUI/Color.hpp"
#include"../../src/TUI/Element.hpp"
#include"../../src/TUI/PCML.hpp"
#include"../../src/Utility/File.hpp"
#include"../../src/Container/Command.hpp"
Document* doc;
Commands<> com=
{
	{ // commands
		{"find",0,[](const char* msg){
			int start=clock();
			auto ans=doc->QuerySelectorAll(msg);
			AnsiPrintB("%Cf[gray][{}ms]%/ {} element found:\n",clock()-start,ans.size());
			for(auto it:ans)
			{
				string attr;
				for(auto i:it->Attribute)
					if(i.second.empty())
						attr+=std::format(" %Cf[skyblue]{}%/",i.first);
					else
						attr+=std::format(" %Cf[skyblue]{}%/=%Cf[chocolate]\"{}\"%/",i.first,i.second);
				AnsiPrint("<%Cf[dodgerblue]{}%/{}>\n",it->GetTag(),attr);
			}
		}},
		{"attr",0,[](const char* msg){// attr <key> [value] <selector>
			string m=msg;
			string key=util::BreakWord(m);
			string val=util::BreakWord(m);
			if(val=="[empty]") val="";
			auto ans=doc->QuerySelectorAll(m);
			for(auto it:ans)
				it->Attribute.Set(key,val);
		}},
		{"add",0,[](const char* msg){// add <tag> <selector>
			string m=msg;
			string tag=util::BreakWord(m);
			auto ans=doc->QuerySelector(m);
			Element e(tag);
			if(ans)	ans->AppendChild(e);
		}},
		{"print",0,[](const char* msg){
			auto ans=doc->QuerySelector(msg);
			if(ans==nullptr)
				AnsiPrintB("%Cf[red]No such element:%/ {}\n",msg);
			else
				AnsiPrintB("%Cf[lemonchiffon]{}%/\n",ans->ToString());
		}},
		{"version",0,[](const char* msg){
			AnsiPrintB("PCL version: %Cf[skyblue]25v20c%/\n");
		}},
	},
	[](const char* msg){
		AnsiPrintB("%Cf[red]Command not found:%/ {}\n",msg);
	},
	[](const char* msg){
		AnsiPrintB("%Cf[red]Permission denied for command:%/ {}\n",msg);
	},
};
int main()
{
	AnsiPrint("Input File: ");
	string file;
	std::cin>>file;
	int start=clock();
	doc=new Document(PCML::Parse(file));
	AnsiPrintB("Parse:%Cf[gold]{}ms%/\n",clock()-start);
	string in;
	getchar();
	while(true)
		putchar('>'),
		std::getline(std::cin,in),
		com.Explain(in.c_str(),0);
	return 0;
}