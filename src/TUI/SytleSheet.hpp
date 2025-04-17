#pragma once
#define PCL_TUI_STYLESHEET

#include<string>
#include<regex>
#include<map>
#include<set>
using std::string;
using std::map;
using std::regex;

#include"../Ansi.hpp"

namespace pcpri
{
	struct COORD{short x,y;};
};

#ifndef PCL_COLOR
bool ValidNamedColor(string a)
{
	int col;
	sscanf(a.c_str(),"%d",&col);
	return col>=0&&col<=255;
}
#define COLOR_MODE pcANSI_COLOR_256
#else
#define COLOR_MODE pcANSI_COLOR_TRUE
#endif

namespace stylepri{

const regex MatchAttr(R"((\S+):\s*(\S+);)");
struct Border
{
	string c[6];
	Border(string lefttop,string righttop,string leftbottom,
		string rightbottom,string vertical,string horizontal)
	{
		c[0]=lefttop,c[1]=righttop,c[2]=leftbottom,c[3]=rightbottom;
		c[4]=vertical,c[5]=horizontal;
	}
};
const map<string,Border> NamedBorders=
{
	{"ascii",Border("+","+","+","+","-","|")},
	{"solid",Border("┌","┐","└","┘","─","│")},
	{"round",Border("╭","╮","╰","╯","─","│")},
	{"double",Border("╔","╗","╚","╝","═","║")},
	{"dotdot",Border("·","·","·","·","·",":")},
	{"none",Border("","","","","","")}
};
struct StyleVals
{
	std::set<string> vals;string def;
	bool has(string q)const{return vals.find(q)!=vals.end();}
};
// attribute, string value
const map<string,StyleVals> NamedStyle=
{
	{"width",{{"<LENGTH>"},"max"}},
	{"height",{{"<LENGTH>"},"fit"}},
#ifdef PCL_COLOR
	{"color",{{"<COLOR>"},"lightgray"}},
	{"background-color",{{"<COLOR>"},"black"}},
	{"border-color",{{"<COLOR>"},"lightgray"}},
#else
	{"color",{{"<COLOR>"},"7"}},
	{"background-color",{{"<COLOR>"},"0"}},
	{"border-color",{{"<COLOR>"},"7"}},
#endif
	{"border",{{"<BORDER>"},"solid"}},
	{"text-align",{{"left","right","center"},"left"}},
	{"title-align",{{"left","right","center"},"left"}},
	{"overflow",{{"cut","ellipsis"},"ellipsis"}},
	{"title",{{"hidden","visible"},"visible"}},
	{"title-space",{{"true","false"},"true"}}
};
bool ValidStyle(string att,string val)
{
	auto it=NamedStyle.find(att);
	if(it==NamedStyle.end())
		return false;
	if(it->second.has("<COLOR>"))
#ifdef PCL_COLOR
		return !StringToColor(val).DontModify();
#else
		return ValidNamedColor(val);
#endif
	if(it->second.has("<BORDER>"))
		return NamedBorders.find(val)!=NamedBorders.end();
	if(it->second.has("<LENGTH>"))
	{
		if(val=="fit"&&val=="max")
			return true;
		int a;
		return sscanf(val.c_str(),"%d",&a)==1;
	}
	return it->second.has(val);
}

}// namespace stylepri

class StyleSheet
{
private:
	map<string,string> s;
	void parse(string t)
	{
		std::smatch res;
		auto begin=std::sregex_iterator(t.begin(),t.end(),stylepri::MatchAttr);
		auto end=std::sregex_iterator();
		string k,v;
		for(auto it=begin;it!=end;it++)
			k=it->operator[](1).str(),v=it->operator[](2).str(),
			SetAttribute(k,v);
		return;
	}
public:
	StyleSheet(){}
	StyleSheet(string t){parse(t);}
	bool SetAttribute(string attr,string val)
	{
		if(!stylepri::ValidStyle(attr,val))
			return false;
		s[attr]=val;
		return true;
	}
	string GetAttribute(string attr)
	{
		if(s.find(attr)==s.end())
			return stylepri::NamedStyle.find(attr)->second.def;
		else return s[attr];
	}
	string operator[] (string k){return GetAttribute(k);}
	string GetTextAnsi()
	{
		ResetAnsiStyle();
		string res;
		res+=std::format("%{}f[{}]",COLOR_MODE,GetAttribute("color"));
		res+=std::format("%{}b[{}]",COLOR_MODE,GetAttribute("background-color"));
		return res;
	}
	void ApplyBorderStyle()
	{
		ResetAnsiStyle();
		if(s.find("border-color")!=s.end())
			AnsiPrint("%{}f[{}]",COLOR_MODE,GetAttribute("border-color"));
	}
	stylepri::Border GetBorder()
		{return stylepri::NamedBorders.find(GetAttribute("border"))->second;}
	void GetSize(short& w,short& h)
	{
		string wi=GetAttribute("width");
		string he=GetAttribute("height");
		if(wi=="max")	w=-1;
		else	sscanf(wi.c_str(),"%d",&w);
		if(he=="fit")	h=-1;
		else	sscanf(he.c_str(),"%d",&h);
	}
};