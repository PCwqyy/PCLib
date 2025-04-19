#pragma once
#define PCL_TUI

#include<string>
#include<cctype>
#include<vector>
#include<print>
#include"Ansi.hpp"

using std::string;
using std::vector;
using std::map;

#include"./TUI/SytleSheet.hpp"
#include"./TUI/Element.hpp"
#include"./TUI/TextBox.hpp"
#include"./TUI/ProgressBar.hpp"

class ConsoleContext
{
	short x=0,y=0,my=0,width;
	map<string,StyleSheet> styles;
public:
	ConsoleContext(short w){width=w;}
	void Print(Element& ele)
	{
		pcpri::COORD a=ele.Print(x,y,width-x,&styles);
		if(a.x==-1)
			y=my,a=ele.Print(0,y,width,&styles);
		x=a.x,my=std::max(my,a.y);
		if(a.x==width)	x=0,y=my;
		CursorGoto(x,y);
	}
	void AddClass(string className,StyleSheet s)
		{styles[className]=s;}
};

#include"Multinclude.hpp"