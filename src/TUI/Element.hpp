#pragma once
#define PCL_TUI_ELEMENT

#include<string>
#include<set>
using std::vector;
using std::set;

#include"SytleSheet.hpp"

class Element
{
protected:
	StyleSheet style,eleStyle;
	short left,top,height,width;
	map<string,StyleSheet>* StyleMap;
	void applyStyleByClass()
	{
		style=eleStyle;
		if(StyleMap==nullptr)	return;
		for(string i:ClassList)
		{
			auto a=StyleMap->find(i);
			if(a==StyleMap->end())	continue;
			style=style+a->second;
		}
	}
	pcpri::COORD printInit(short x,short y,
		map<string,StyleSheet>* c=nullptr)
	{
		left=x,top=y;
		if(c!=nullptr)	StyleMap=c;
		applyStyleByClass();
		return {left,top};
	}
public:
	stylepri::StringSet ClassList;
	virtual pcpri::COORD Print(short x,short y,
		short visWidth,map<string,StyleSheet>* c=nullptr)
		{return printInit(x,y,c);}
	string GetStyle(string attr){return style[attr];}
	void SetStyle(string attr,string val){style.SetAttribute(attr,val);}
	void SetStyle(StyleSheet a){style=a;}
	Element(StyleSheet a="")
		{height=0,width=0,left=0,top=0;eleStyle=a;StyleMap=nullptr;}
	virtual ~Element(){}
};