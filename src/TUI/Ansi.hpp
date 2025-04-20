#pragma once
#define PCL_ANSI

#include<format>
#include<string>
#include<print>
#include<deque>
#include<regex>

namespace pcpri
{
	int string2int(std::string a)
	{
		int l=a.length(),ans=0;
		for(int i=0;i<l;i++)
			ans*=10,
			ans+=a[i]-'0';
		return ans;
	}
}

#define pcANSI_MARKER_CHAR '%'
#define pcANSI_END_REGION '/'
#define pcANSI_BOLD 'b'
#define pcANSI_COLOR_8 'q'
#define pcANSI_COLOR_256 'c'
#define pcANSI_COLOR_BACK 'b'
#define pcANSI_COLOR_FORE 'f'
#define pcANSI_DARKEN 'd'
#define pcANSI_GRAY 'g'
#define pcANSI_INVERT '!'
#define pcANSI_ITALIC 'i'
#define pcANSI_LINK 'l'
#define pcANSI_STRIKETHROUGH 's'
#define pcANSI_TWINKLE 't'
#define pcANSI_UNDERLINE 'u'
#ifdef PCL_COLOR
	#define pcANSI_COLOR_TRUE 'C'
#endif

namespace pcpri
{
	std::string parseSimpleMark(char mark)
	{
		switch(mark)
		{
		case pcANSI_MARKER_CHAR:
			return std::format("{}",pcANSI_MARKER_CHAR);
		case pcANSI_END_REGION:
			return "\e[0m";
		case pcANSI_BOLD:
			return "\e[1m";
		case pcANSI_GRAY:
			return "\e[2m";
		case pcANSI_ITALIC:
			return "\e[3m";
		case pcANSI_UNDERLINE:
			return "\e[4m";
		case pcANSI_TWINKLE:
			return "\e[5m";
		case pcANSI_INVERT:
			return "\e[7m";
		case pcANSI_DARKEN:
			return "\e[8m";
		case pcANSI_STRIKETHROUGH:
			return "\e[9m";
		default:
			return std::format("{}",pcANSI_MARKER_CHAR);
		}
	}
	int parseMark(std::string& res,std::string input,int len,int i,bool close=false)
	{
		std::string link,text;
		int temp1=0,temp2=0;
#ifdef PCL_COLOR
		Color col;
#endif
		switch(input[i])
		{
		break;case pcANSI_LINK:
			if(input[++i]!='[')	break;	i++;
			for(;i<len&&input[i]!=']';i++)
				text+=input[i];
			if(input[++i]!='(')	break;	i++;
			for(;i<len&&input[i]!=')';i++)
				link+=input[i];
			res+=std::format("\e]8;;{0}\e{2}{1}\e]8;;\e{2}",
				link,text,close?"\\\a":"\\");
		break;case pcANSI_COLOR_8:
			i++;
			if(input[i]==pcANSI_COLOR_BACK)	temp1=40;
			if(input[i]==pcANSI_COLOR_FORE)	temp1=30;
			res+=std::format("\e[{}{}",
				temp1+input[++i]-'0',close?"m\a":"m");				
		break;case pcANSI_COLOR_256:
			temp2=0;i++;
			if(input[i]==pcANSI_COLOR_BACK)	temp1=48;
			if(input[i]==pcANSI_COLOR_FORE)	temp1=38;
			if(input[++i]!='[')	break;	i++;
			for(;i<len&&input[i]!=']';i++)
				temp2*=10,temp2+=input[i]-'0';
			res+=std::format("\e[{};5;{}{}",
				temp1,temp2,close?"m\a":"m");
#ifdef PCL_COLOR
		break;case pcANSI_COLOR_TRUE:
			i++;
			if(input[i]==pcANSI_COLOR_BACK)	temp1=48;
			if(input[i]==pcANSI_COLOR_FORE)	temp1=38;
			if(input[++i]!='[')	break;	i++;
			for(;i<len&&input[i]!=']';i++)
				text+=input[i];
			col=StringToColor(text);
			if(col.DontModify())
				break;
			res+=std::format("\e[{};2;{};{};{}{}",
				temp1,col.R,col.G,col.B,close?"m\a":"m");
#endif
		break;default:
			res+=parseSimpleMark(input[i])+(close?"\a":"");
		break;}
		return i;
	}
}

std::string AnsiParse(std::string input,bool close=false)
{
	std::string res,link,text;
	std::deque<std::string> fmt;
	int len=input.length(),last,temp1,temp2;
	for(int i=0;i<len;i++)
		if(input[i]==pcANSI_MARKER_CHAR&&i!=len-1)
		{
			i++;last=i;
			if(input[i]!=pcANSI_END_REGION)
			{
				i=pcpri::parseMark(res,input,len,i,close);
				fmt.push_back(input.substr(last,i-last+1));
			}
			else
			{
				res+=pcpri::parseSimpleMark(pcANSI_END_REGION)+(close?"\a":"");
				if(!fmt.empty())
					fmt.pop_back();
				for(auto j:fmt)
					pcpri::parseMark(res,j,j.size(),0,close);
			}
		}
		else	res+=input[i];
	return res;
}

/// @brief Parse ansi after formatting
template<typename ...Tps>
inline void AnsiPrintA(std::string fmt,Tps ...args)
{
	std::print("{}",AnsiParse(std::vformat(fmt,std::make_format_args(args...))));
	return;
}
/// @brief Parse ansi before formatting
template<typename ...Tps>
inline void AnsiPrintB(std::string fmt,Tps ...args)
{
	std::print("{}",std::vformat(AnsiParse(fmt),std::make_format_args(args...)));
	return;
}

#ifdef ALWAYS_PARSE_BEFORE
#define AnsiPrint AnsiPrintB
#else
#define AnsiPrint AnsiPrintA
#endif

#ifndef PCL_CONSOLE
void ResetAnsiStyle()
	{std::print("\e[0m");return;}
void CursorGoto(short x,short y)
	{std::print("\e[{};{}H",y+1,x+1);return;}
#endif
void CursorDelta(short x,short y)
{
	char op;
	if(x!=0)
	{
		if(x>0)			op='C';
		else if(x<0)	op='D',x=-x;
		std::print("\e[{}{}",x,op);
	}
	if(y!=0)
	{
		if(y>0)			op='B';
		else if(y<0)	op='A',y=-y;
		std::print("\e[{}{}",y,op);
	}
}
void ClearCurrentLine()
	{std::print("\e[2K");return;}
void ClearWholeScreen()
	{std::print("\e[2J");return;}
void SaveCurrentCursorPos()
	{std::print("\e[2s");return;}
void RestoreCursorPos()
	{std::print("\e[2u");return;}
void HideCursor()
	{std::print("\e[?25l");return;}
void ShowCursor()
	{std::print("\e[?25h");return;}
void SetConsoleTitle(std::string title)
	{std::print("\e]0;{}\a",title);return;}

int AnsiVisLen(std::string s)
{
	std::string t=AnsiParse(s,true);
	int len=t.length(),ans=0;
	for(int i=0;i<len;i++)
		if(t[i]=='\e')
			while(t[i]!='\a')	i++;
		else if(t[i]=='\a')
			continue;
		else ans++;
	return ans;
};

#include "Multinclude.hpp"