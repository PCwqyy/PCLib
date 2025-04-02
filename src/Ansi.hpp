#pragma once
#define PCL_ANSI

#include<format>
#include<string>
#include<print>
#include<deque>

using std::format;
using std::vformat;
using std::print;
using std::println;
using std::deque;
using std::string;

namespace pcpri
{
	int string2int(string a)
	{
		int l=a.length(),ans=0;
		for(int i=0;i<l;i++)
			ans*=10,
			ans+=a[i]-'0';
		return ans;
	}
}

#define pcANSI_MARKER_CHAR '$'
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
	string parseSimpleMark(char mark)
	{
		switch(mark)
		{
		case pcANSI_MARKER_CHAR:
			return format("{}",pcANSI_MARKER_CHAR);
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
			return format("{}",pcANSI_MARKER_CHAR);
		}
	}
	int parseMark(string& res,string input,int len,int i)
	{
		string link,text;
		int temp1=0,temp2=0;
#ifdef PCL_COLOR
		Color col;
#endif
		switch(input[i])
		{
		break;case pcANSI_LINK:
			if(input[++i]!='[')	break;	i++;
			for(;i<len&&input[i]!=']';i++)
				link+=input[i];
			if(input[++i]!='(')	break;	i++;
			for(;i<len&&input[i]!=')';i++)
				text+=input[i];
			res+=format("\e]8;;{}\e\\{}\e]8;;\e\\",link,text);
		break;case pcANSI_COLOR_8:
			i++;
			if(input[i]==pcANSI_COLOR_BACK)	temp1=40;
			if(input[i]==pcANSI_COLOR_FORE)	temp1=30;
			res+=format("\e[{}m",temp1+input[++i]-'0');				
		break;case pcANSI_COLOR_256:
			temp2=0;i++;
			if(input[i]==pcANSI_COLOR_BACK)	temp1=48;
			if(input[i]==pcANSI_COLOR_FORE)	temp1=38;
			if(input[++i]!='[')	break;	i++;
			for(;i<len&&input[i]!=']';i++)
				temp2*=10,temp2+=input[i]-'0';
			res+=format("\e[{};5;{}m",temp1,temp2);
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
			res+=format("\e[{};2;{};{};{}m",temp1,col.R,col.G,col.B);
#endif
		break;default:
			res+=parseSimpleMark(input[i]);
		break;}
		return i;
	}
}

string AnsiParse(string input)
{
	string res,link,text;
	deque<string> fmt;
	int len=input.length(),last,temp1,temp2;
	for(int i=0;i<len;i++)
		if(input[i]==pcANSI_MARKER_CHAR&&i!=len-1)
		{
			i++;last=i;
			if(input[i]!=pcANSI_END_REGION)
			{
				i=pcpri::parseMark(res,input,len,i);
				fmt.push_back(input.substr(last,i-last+1));
			}
			else
			{
				res+=pcpri::parseSimpleMark(pcANSI_END_REGION);
				fmt.pop_back();
				for(auto j:fmt)
					pcpri::parseMark(res,j,j.size(),0);
			}
		}
		else	res.push_back(input[i]);
	return res;
}

/// @brief Parse ansi before formatting
template<typename ...Tps>
inline void AnsiPrintA(string fmt,Tps ...args)
{
	std::print("{}",AnsiParse(vformat(fmt,std::make_format_args(args...))));
	return;
}
/// @brief Parse ansi after formatting
template<typename ...Tps>
inline void AnsiPrintB(string fmt,Tps ...args)
{
	std::print("{}",vformat(AnsiParse(fmt),std::make_format_args(args...)));
	return;
}

#ifdef ALWAYS_PARSE_BEFORE
#define AnsiPrint AnsiPrintB
#else
#define AnsiPrint AnsiPrintA
#endif

#include "Multinclude.hpp"