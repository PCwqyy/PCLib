#pragma once
#define PCL_ANSI

#include<format>
#include<string>
#include<print>
#include<deque>
#include<regex>


/// @brief Reset all ansi style
inline void ResetAnsiStyle()
	{std::print("\e[0m");return;}
#ifndef PCL_CONSOLE
/// @brief Move cursor to horizontal `x` and vertical `y`
inline void CursorGoto(short x,short y)
	{std::print("\e[{};{}H",y+1,x+1);return;}
#endif
/// @brief Offset the cursor horizontally by `x` and vertically by `y`
inline void CursorOffset(short x,short y)
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
/// @brief Clear the line where the cursor is
inline void ClearCurrentLine()
	{std::print("\e[2K");return;}
/** @brief Fill the visible screen with white spaces.
 *  The overflowed content won't be effected */
inline void ClearWholeScreen()
	{std::print("\e[2J");return;}
/** @brief Save the current position of the cursor.
 *  Later you can use `RestoreCursorPos()` to restore this position. */
inline void SaveCurrentCursorPos()
	{std::print("\e[2s");return;}
/** @brief Set position of the cursor to where you use
 *  `SaveCurrentCursorPos()`. */
inline void RestoreCursorPos()
	{std::print("\e[2u");return;}
/// @brief Let the cursor invisible.
inline void HideCursor()
	{std::print("\e[?25l");return;}
/// @brief Let the cursor visible.
inline void ShowCursor()
	{std::print("\e[?25h");return;}
/// @brief Modify the title of the console (or terminal).
inline void SetConsoleTitle(std::string title)
	{std::print("\e]0;{}\a",title);return;}
/// @brief Set the foreground color.
#ifdef PCL_COLOR
#include"Color.hpp"
inline void SetForegroundColor(Color col)
	{std::print("\e[38;2;{};{};{}m",col.R,col.G,col.B);return;}
/// @brief Set the background color.
inline void SetBackgroundColor(Color col)
	{std::print("\e[48;2;{};{};{}m",col.R,col.G,col.B);return;}
#endif
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
			col=Color(text);
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

/**
 * @brief Parse a ANSI format string to ESC string.
 * @param input the string to parse
 * @param close Add a `\a` char in the end of ESC.
 * Don't use this unless you KNOW what you are doing.
 */
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

/// @brief Using `AnsiPrintA()`
template<typename ...Tps>
inline void AnsiPosPrintA(short x,short y,std::string ftm,Tps...Args)
{
	CursorGoto(x,y);
	AnsiPrintA(ftm,Args...);
	return;
}
/// @brief Using `AnsiPrintB()`
template<typename ...Tps>
inline void AnsiPosPrintB(short x,short y,std::string ftm,Tps...Args)
{
	CursorGoto(x,y);
	AnsiPrintB(ftm,Args...);
	return;
}


#ifdef ALWAYS_PARSE_BEFORE
#define AnsiPrint AnsiPrintB
#define AnsiPosPrint AnsiPosPrintB
#else
/**
 * @brief Print an ansi string
 * ```
 * %/	END_REGION
 * %b	BOLD
 * %d	DARKEN
 * %g	GRAY
 * %!	INVERT
 * %i	ITALIC
 * %l	LINK
 * %s	STRIKETHROUGH
 * %t	TWINKLE
 * %u	UNDERLINE
 * ```
 * For colors: 
 * ```
 * %<mode><back/foreground>[<color>]
 * <mode>:
 * 	q	8-color
 * 	c	256-color
 * 	C	true-color //if Color.hpp is included
 * 		In this mode, <color> could be:
 * 		- RRGGBB (e.g. #20c0ff)
 * 		- RGB (e.g. #0f6)
 * 		- Named colors (e.g. red, dodgerblue)
 * <back/foreground>:
 * 	b	background
 * 	f	foreground
 * ```
 * For links:
 * ```
 * %l[<text>](<url>)
 * ```
 * Examples:
 * ```text
 * %Cf[red]This is red text%/ and this is normal text.
 * %bThis is bold text%/ and this is normal text.
 * %l[Click here](https://example.com)%/ to visit example.com.
 * ```
 * @note if `ALWAYS_PARSE_BEFORE` is defined,
 * this macro will redirect to `AnsiPrintB` (Parse before formatting)
 */
#define AnsiPrint AnsiPrintA
/**
 * @note if `ALWAYS_PARSE_BEFORE` is defined,
 * this macro will redirect to `AnsiPosPrintB` (Parse before formatting)
 */
#define AnsiPosPrint AnsiPosPrintA
#endif

/// @brief Calculate the visable part of a ANSI formatted string.
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