#pragma once
#define PCL_TUI_DOCUMENT

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
	Document():Element(){}
	Document(Element a):Element(a){}
	void Print()
	{
		Render();
		view.Print({x,y});
	}
};