#include<string>
#include<cctype>
#include<vector>
#include<print>
#include<regex>
#include<map>
#include<set>
#include"Ansi.hpp"

using std::string;
using std::vector;
using std::map;
// using std::set;
using std::regex;


#ifndef PCL_COLOR
map<string,int> NamedColor=
{
	// TODO
}
#define COLOR_MODE pcANSI_COLOR_256
#else
#define COLOR_MODE pcANSI_COLOR_TRUE
#endif

#define pcTB_LEFT		0x0
#define pcTB_CENTER		0x1
#define pcTB_RIGHT		0x2
#define pcTB_ELLIPSIS	0x0
#define pcTB_CUT		0x4
#define pcTB_TITLE_LEFT		0x0
#define pcTB_TITLE_CENTER	0x8
#define pcTB_TITLE_RIGHT	0x10
#define pcTB_NO_TITLE		0x20
#define pcTB_NO_TITLE_SPACE	0x40

namespace pcpri
{
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
		{"rhombic",Border("╱","╲","╲","╱","─","│")},
		{"dotdot",Border(".",".",".",".",".",":")},
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
		{"color",{{"*COLOR"},"White"}},
		{"background-color",{{"*COLOR"},"Black"}},
		{"border-color",{{"*COLOR"},"White"}},
		{"border",{{"*BORDER"},"solid"}},
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
		if(it->second.has("*COLOR"))
			return NamedColor.find(val)!=NamedColor.end();
		if(it->second.has("*BORDER"))
			return NamedBorders.find(val)!=NamedBorders.end();
		return it->second.has(val);
	}
}

class StyleSheet
{
private:
	map<string,string> s;
	void parse(string t)
	{
		std::smatch res;
		auto begin=std::sregex_iterator(t.begin(),t.end(),pcpri::MatchAttr);
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
		if(!pcpri::ValidStyle(attr,val))
			return false;
		s[attr]=val;
		return true;
	}
	string GetAttribute(string attr)
	{
		if(s.find(attr)==s.end())
			return pcpri::NamedStyle.find(attr)->second.def;
		else return s[attr];
	}
	string operator[] (string k){return GetAttribute(k);}
	string GetTextAnsi()
	{
		ResetAnsiStyle();
		string res;
		if(s.find("color")!=s.end())
			res+=std::format("${}f[{}]",COLOR_MODE,GetAttribute("color"));
		if(s.find("background-color")!=s.end())
			res+=std::format("${}b[{}]",COLOR_MODE,GetAttribute("background-color"));
		return res;
	}
	void ApplyBorderStyle()
	{
		ResetAnsiStyle();
		if(s.find("border-color")!=s.end())
			AnsiPrint("${}f[{}]",COLOR_MODE,GetAttribute("border-color"));
	}
	pcpri::Border GetBorder()
		{return pcpri::NamedBorders.find(GetAttribute("border"))->second;}
};

class TextBox
{
protected:
	string text,title;
	short left,top,height,width,visHei;
	StyleSheet style;
	struct word
	{
		string word;char space;
		string toString()
		{
			if(space==-1)
				return std::format("{}",word);
			else
				return std::format("{}{}",space,word);
		}
		int visibleLen()
		{
			if(word[0]=='\e')
				return 0;
			return word.length()+(space==-1?0:1);
		}
	};
	struct line{int pre;string line;};
	void breakWord(vector<word>& words)
	{
		string parsed=AnsiParse(style.GetTextAnsi()+text,true);
		char l=-1;int len=parsed.length();
		string cur;
		for(int i=0;i<len;i++)
		{
			if(cur.length()==width)
				words.push_back(word(cur,l)),
				cur="";
			if(isgraph(parsed[i]))
				cur+=parsed[i];
			else
			{
				words.push_back(word(cur,l));
				cur="";int q=i;
				if(parsed[i]=='\e')
				{
					while(parsed[i]!='\a')	i++;
					words.push_back(word(parsed.substr(q,i-q),-1));
					l=-1;
				}
				else
					l=parsed[i];
			}
		}
		words.push_back(word(cur,l));
	}
	bool prepareLine(vector<line>& lines)
	{
		vector<word> words;
		breakWord(words);
		int x=0,y=1,sw,sx,len=words.size();
		bool ell=false;
		string cur;
		for(int i=0;i<len;i++)
		{
			sx=x,x+=words[i].visibleLen();
			if(words[i].space=='\n'||x>width)
			{
				if(style["overflow"]=="ellipsis"&&i<len&&
				   height!=-1&&lines.size()==height-1)
					if(sx<width)	cur+="…",sx++;
					else			ell=true;
				if(style["text-align"]=="center")	sw=(width-sx)/2;
				else if(style["text-align"]=="right")	sw=width-sx;
				else if(style["text-align"]=="left")	sw=0;
				lines.push_back(line{sw,cur});
				cur="",y++,sx=0,words[i].space=-1,x=words[i].visibleLen();
			}
			cur+=words[i].toString();
		}
		sx=cur.length();
		if(style["text-align"]=="center")	sw=(width-sx)/2;
		else if(style["text-align"]=="right")	sw=width-sx;
		else if(style["text-align"]=="left")	sw=0;
		lines.push_back(line{sw,cur});
		visHei=(height==-1)?lines.size():height;
		return ell;
	}
	void printBox()
	{
		CursorGoto(left,top);
		for(int i=0;i<visHei;i++)
			CursorGoto(left,top+i),
			std::print("{}",string(width,' '));
		CursorGoto(left,top);
	}
	void printText(vector<line> lines,bool ell)
	{
		CursorGoto(left,top);
		int l=lines.size();
		for(int i=0;i<visHei&&i<l;i++)
			CursorGoto(left+lines[i].pre,top+i),
			std::print("{}",lines[i].line);
		if(height!=-1&&style["overflow"]=="ellipsis"&&ell==true)
			CursorGoto(left+width-1,top+height-1),
			std::print("…");
	}
	void printBorder()
	{
		short borderPos[4][2]=
		{
			{static_cast<short>(left-1),static_cast<short>(top-1)},
			{static_cast<short>(left+width),static_cast<short>(top-1)},
			{static_cast<short>(left-1),static_cast<short>(top+visHei)},
			{static_cast<short>(left+width),static_cast<short>(top+visHei)}
		};
		pcpri::Border border=style.GetBorder();
		for(int i=0;i<4;i++)
			CursorGoto(borderPos[i][0],borderPos[i][1]),
			std::print("{}",border.c[i]);
		CursorGoto(left,top-1);
		for(int i=0;i<width;i++)
			std::print("{}",border.c[4]);
		CursorGoto(left,top+visHei);
		for(int i=0;i<width;i++)
			std::print("{}",border.c[4]);
		for(int i=0;i<visHei;i++)
			CursorGoto(left-1,top+i),
			std::print("{}",border.c[5]);
		for(int i=0;i<visHei;i++)
			CursorGoto(left+width,top+i),
			std::print("{}",border.c[5]);
		if(style["title"]=="hidden")	return;
		string t=title;
		if(title.length()>width)
			t=title.substr(0,width)+"…";
		if(style["title-space"]=="true")
			t=" "+t+" ";
		int len=t.length();
		if(style["title-align"]=="center")
			CursorGoto(left+width/2-len/2,top-1);
		else if(style["title-align"]=="right")
			CursorGoto(left+width-len,top-1);
		else if(style["title-align"]=="left")
			CursorGoto(left,top-1);
		std::print("{}",t);
	}
public:
	void Print()
	{
		vector<line> lines;
		bool ell=prepareLine(lines);
		HideCursor();
		SaveCurrentCursorPos();
		ResetAnsiStyle();
		if(style["border"]!="none")
			style.ApplyBorderStyle(),
			printBorder();
		AnsiPrint("{}",style.GetTextAnsi());
		printBox();
		printText(lines,ell);
		ResetAnsiStyle();
		RestoreCursorPos();
		ShowCursor();
		return;
	}
	string GetText(){return text;}
	void SetText(string t)
	{
		text=t;
		Print();
		return;
	}
	/**
	 * @brief Construct a textbox. Ansi supported
	 * @param x left
	 * @param y top
	 * @param h height
	 * @param w width
	 * @param t text
	 * @param tt title
	 * @param s stylesheet (Yes the grammar IS what you think.)
	 */
	TextBox(short x,short y,short w,short h,string tt,string t,string s=""):
		text(t),style(s),left(x),top(y),height(h),width(w),title(tt)
	{}
};