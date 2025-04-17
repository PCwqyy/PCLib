#pragma once
#define PCL_TUI_TEXTBOX

#include<vector>
using std::vector;

#include"Element.hpp"

class TextBox:public Element
{
protected:
	string text,name;
	short visHei;
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
	void breakWord(short width,vector<word>& words)
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
	bool prepareLine(short height,short width,vector<line>& lines)
	{
		vector<word> words;
		breakWord(width,words);
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
	void printBox(short height,short width,short left,short top)
	{
		CursorGoto(left,top);
		for(int i=0;i<visHei;i++)
			CursorGoto(left,top+i),
			std::print("{}",string(width,' '));
		CursorGoto(left,top);
	}
	void printText(short height,short width,short left,short top,vector<line> lines,bool ell)
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
	void printBorder(short height,short width,short left,short top)
	{
		int borderPos[4][2]=
		{
			{left-1,top-1},
			{left+width,top-1},
			{left-1,top+visHei},
			{left+width,top+visHei}
		};
		stylepri::Border border=style.GetBorder();
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
		string t=name;
		if(name.length()>width)
			t=name.substr(0,width)+"…";
		if(style["title-space"]=="true")
			t=" "+t+" ";
		int len=t.length();
		if(style["title-align"]=="center")
			CursorGoto(left+width/2-len/2,top-1);
		else if(style["title-align"]=="right")
			CursorGoto(left+width-len,top-1);
		else if(style["title-align"]=="left")
			CursorGoto(left,top-1);
		AnsiPrint("{}",t);
	}
public:
	pcpri::COORD Print(short left,short top,short visWid=-1)
	{
		short height,width;
		bool hasBorder=style["border"]!="none";
		style.GetSize(width,height);
		if(visWid<width)
			return {-1,-1};
		if(hasBorder&&visWid!=-1)
			left++,top++,visWid-=2;
		if(visWid!=-1&&width==-1)
			width=visWid,visWid=-2;
		vector<line> lines;
		bool ell=prepareLine(height,width,lines);
		HideCursor();
		ResetAnsiStyle();
		if(hasBorder)
			style.ApplyBorderStyle(),
			printBorder(height,width,left,top);
		AnsiPrint("{}",style.GetTextAnsi());
		printBox(height,width,left,top);
		printText(height,width,left,top,lines,ell);
		ResetAnsiStyle();
		ShowCursor();
		left+=width,top+=visHei;
		if(hasBorder)
			left++,top++;
		if(visWid==-2)	width=-1;
		return {left,top};
	}
	string GetText(){return text;}
	void SetText(string t)
	{
		text=t;
		return;
	}
	/**
	 * @brief Construct a textbox. Ansi supported
	 * @param x left
	 * @param y top
	 * @param h height
	 * @param w width
	 * @param t text
	 * @param n title
	 * @param s stylesheet (Yes the grammar IS what you think.)
	 */
	TextBox(string n,string t,string s=""):
		text(t),name(n){style=s;}
};