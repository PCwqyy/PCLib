#include<string>
#include<cctype>
#include<vector>
#include<print>
#include"Ansi.hpp"

using std::string;
using std::vector;

#define pcTB_LEFT	0x0
#define pcTB_CENTER	0x1
#define pcTB_RIGHT	0x2
#define pcTB_ELLIPSIS	0x0
#define pcTB_CUT		0x4

class TextBox
{
private:
	string text,textStyle;
	short left,top,height,width;
	int style;
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
	void breakWord(vector<word>& words)
	{
		string parsed=AnsiParse(textStyle+text,true);
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
public:
	void Print()
	{
		vector<word> words;
		vector<string> lines;
		breakWord(words);
		int x=0,y=1,sw,sx,len=words.size();
		bool ell=false;
		string cur;
		for(int i=0;i<len;i++)
		{
			sx=x,x+=words[i].visibleLen();
			if(words[i].space=='\n'||x>width)
			{
				if(!(style&pcTB_CUT)&&lines.size()==height-1&&i<len)
					if(sx<width)	cur+="…",sx++;
					else			ell=true;
				if(style&pcTB_CENTER)		sw=(width-sx)/2;
				else if(style&pcTB_RIGHT)	sw=width-sx;
				else						sw=0;
				lines.push_back(std::format("{1}{0}",cur,string(sw,' ')));
				cur="",y++,sx=0,words[i].space=-1,x=words[i].visibleLen();
			}
			cur+=words[i].toString();
		}
		sx=cur.length();
		if(style&pcTB_CENTER)		sw=(width-sx)/2;
		else if(style&pcTB_RIGHT)	sw=width-sx;
		else						sw=0;
		lines.push_back(std::format("{1}{0}",cur,string(sw,' ')));
	// Print
		HideCursor();
		SaveCurrentCursorPos();
		CursorGoto(left,top);
		ResetAnsiStyle();
		AnsiPrint("{}",textStyle);
		for(int i=0;i<height;i++)
			std::print("{}",string(width,' ')),
			CursorGoto(left,top+i+1);
		CursorGoto(left,top);
		int l=lines.size();
		for(int i=0;i<height;i++)
			std::print("{}",lines[i]),
			CursorGoto(left,top+i+1);
		if(!(style&pcTB_CUT)&&ell==true)
			CursorGoto(left+width-1,top+height-1),
			std::print("…");
		ResetAnsiStyle();
		RestoreCursorPos();
		ShowCursor();
		return;
	}
	string GetText(){return text;}
	void SetText(string t,string s="-1")
	{
		if(s=="-1")
			s=textStyle;
		text=s+t;
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
	 * @param s text style in ansi
	 * @param q textbox style
	 */
	TextBox(short x,short y,short w,short h,string t,string s="",int q=0):
		text(t),textStyle(s),style(q),left(x),top(y),height(h),width(w){}

};