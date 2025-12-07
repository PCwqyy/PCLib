#include<string>
#include<cctype>
#include<vector>
#include<print>

using std::string;
using std::vector;
using std::map;

#include"OS.hpp"
#include"Element.hpp"

class Document:public Element
{
	short x=0,y=0,width=GetTerminalWidth();
	map<string,StyleSheet> styles;
public:
	Document():Element()
		{}
	Document(Element a):Element(a)
		{canvas=screen.ViewAt({0,0},width);}
	void Render()
	{
		Print();
		screen.Render({x,y});
	}
};