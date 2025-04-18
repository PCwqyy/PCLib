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

const regex MatchAttr(R"(([^:\s]+?):\s*([^;]+?);)");
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
map<string,Border> NamedBorders=
{
	{"ascii",Border("+","+","+","+","-","|")},
	{"solid",Border("┌","┐","└","┘","─","│")},
	{"round",Border("╭","╮","╰","╯","─","│")},
	{"double",Border("╔","╗","╚","╝","═","║")},
	{"dotdot",Border("·","·","·","·","·",":")},
	{"block",Border(" "," "," "," "," "," ")},
	{"none",Border("","","","","","")}
};
struct Bar
{
	string t,f,l,r;
	Bar(string finished,string unfinished,
		string leftbracket,string rightbracket):
		t(finished),f(unfinished),l(leftbracket),r(rightbracket){}
};
map<string,Bar> NamedBars=
{
	{"line",Bar("━","─","<",">")},
	{"block",Bar(" "," ","[","]")},
	{"cube",Bar("■","□","[","]")},
	{"dashed",Bar("─","┈","<",">")},
	{"ascii",Bar("#"," ","[","]")}
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
	{"title-color",{{"<COLOR>"},"lightgray"}},
	{"background-color",{{"<COLOR>"},"black"}},
	{"border-color",{{"<COLOR>"},"lightgray"}},
#else
	{"color",{{"<COLOR>"},"7"}},
	{"title-color",{{"<COLOR>"},"7"}},
	{"background-color",{{"<COLOR>"},"0"}},
	{"border-color",{{"<COLOR>"},"7"}},
#endif
	{"text-align",{{"left","right","center"},"left"}},
	{"title-align",{{"left","right","center"},"left"}},
	{"overflow",{{"cut","ellipsis"},"ellipsis"}},
	{"title",{{"hidden","visible"},"visible"}},
	{"title-space",{{"true","false"},"true"}},
	{"border",{{"<BORDER>"},"solid"}},
	{"bar",{{"<BAR>"},"line"}},
	{"bar-bracket",{{"true","false"},"false"}},
	// 0:advance, 1:tot, 2:percent, 3:speed, 4:estimate, 5:title
	{"bar-format",{{"<STRING>"}," {0:.1f}/{1:.1f}"}},
	{"bar-lazy",{{"<NUMBER>"},"200"}},
};
bool ValidStyle(string att,string val)
{
	auto it=NamedStyle.find(att);
	if(it==NamedStyle.end())
		return false;
	if(it->second.has("<STRING>"))
		return true;
	if(it->second.has("<COLOR>"))
	{
		if(val=="unset")
			return true;
#ifdef PCL_COLOR
		return !StringToColor(val).DontModify();
#else
		return ValidNamedColor(val);
#endif
	}
	if(it->second.has("<BORDER>"))
		return NamedBorders.find(val)!=NamedBorders.end();
	if(it->second.has("<BAR>"))
		return NamedBars.find(val)!=NamedBars.end();
	if(it->second.has("<LENGTH>")||it->second.has("<NUMBER>"))
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
	bool SetAttribute(string attr,string val)
	{
		if(!stylepri::ValidStyle(attr,val))
			return false;
		s[attr]=val;
		return true;
	}
	string GetAttribute(string attr)
	{
		auto res=s.find(attr);
		if(res==s.end()||res->second=="unset")
		{
			auto res2=stylepri::NamedStyle.find(attr);
			if(res2==stylepri::NamedStyle.end())
				throw "Invalid attribute!";
			return res2->second.def;
		}
			
		else return res->second;
	}
	string operator[] (string k){return GetAttribute(k);}
	string GetTextAnsi()
	{
		ResetAnsiStyle();
		string res;
		res+=std::format("{}{}f[{}]",pcANSI_MARKER_CHAR,COLOR_MODE,GetAttribute("color"));
		res+=std::format("{}{}b[{}]",pcANSI_MARKER_CHAR,COLOR_MODE,GetAttribute("background-color"));
		return res;
	}
	string GetBarStyle(bool fini)
	{
		ResetAnsiStyle();
		string v=GetAttribute(fini?"color":"background-color");
		return AnsiParse(std::format("{}{}{}[{}]",pcANSI_MARKER_CHAR,COLOR_MODE,(v=="block")?'b':'f',v));
	}
	string GetBorderStyle()
	{
		ResetAnsiStyle();
		string v=GetAttribute("border-color");
		return AnsiParse(std::format("{}{}{}[{}]",pcANSI_MARKER_CHAR,COLOR_MODE,(v=="block")?'b':'f',v));
	}
	string GetTitleStyle()
	{
		ResetAnsiStyle();
		string v=GetAttribute("title-color");
		return AnsiParse(std::format("{}{}f[{}]",pcANSI_MARKER_CHAR,COLOR_MODE,v));
	}
	void GetSize(short& width,short& height)
	{
		string att=GetAttribute("width");
		int a=-1,b=-1;
		if(att=="max")	a=-1;
		else if(sscanf(att.c_str(),"%d",&a)!=1)	a=-1;
		att=GetAttribute("height");
		if(att=="fit")	b=-1;
		else if(sscanf(att.c_str(),"%d",&b)!=1)	b=-1;
		width=a,height=b;
	}
	int GetLazy()
	{
		int a=25;
		sscanf(GetAttribute("bar-lazy").c_str(),"%d",&a);
		return a;
	}
	stylepri::Border GetBorder()
		{return stylepri::NamedBorders.find(GetAttribute("border"))->second;}
	stylepri::Bar GetBar()
		{return stylepri::NamedBars.find(GetAttribute("bar"))->second;}
	StyleSheet(){}
	StyleSheet(string t){parse(t);}
	StyleSheet(const char* s){parse(string(s));}
};