#pragma once
#define PC_TUI_BUFFER

#include<vector>
#include"../Container/String.hpp"
#include"Color.hpp"
#include"Ansi.hpp"
#include"OS.hpp"
#include"Util.hpp"
using std::vector;

class Pixel
{
private:
	char16_t high,low;
	bool surrogate;
public:
	Color fore,back;
	void Set(char16_t c,Color f=-1,Color b=-1)
		{high=c;fore=f,back=b;surrogate=false;}
	void Set(char16_t h,char16_t l,Color f=-1,Color b=-1)
		{high=h,low=l;fore=f,back=b;surrogate=true;}
	Pixel(char16_t c,Color f=-1,Color b=-1)
		{Set(c,f,b);}
	Pixel(char16_t h,char16_t l,Color f=-1,Color b=-1)
		{Set(h,l,f,b);}
	Pixel(){Set(u'\0',-1,-1);}
	inline bool isSurrogate(){return surrogate;}
	inline bool isEmpty(){return high==u'\0';}
	void putchar(bool color=true)
	{
		SetForegroundColor(fore);
		SetBackgroundColor(back);
		String s;s.Append(high);
		if(surrogate)	s.Append(low);
		OutputUnicode(s);
	}
	inline int width()
		{return pcuni::charWidthInConsole(high);}
};

class Buffer
{
private:
	vector<vector<Pixel>> canvas;
	Coord size;
	void resize(short w,short h)
	{
		for(auto i:canvas)
			i.resize(w);
		canvas.resize(h,vector<Pixel>(w));
	}
	Pixel& get(Coord pos){return canvas[pos.y][pos.x];}
	inline void cursorIncrease(Coord& cur)
	{
		cur.x++;
		if(cur.x>=size.x)	cur.x=0,cur.y++;
		if(cur.y>=size.y)	cur={0,0};
	}
	inline void cursorBreakLine(Coord& cur)
	{
		cur.x=0,cur.y++;
		if(cur.y>=size.y)	cur={0,0};
	}
public:
	void Print(Coord pos,Color fore,Color back,String text)
	{
		int len=text.Size();
		for(int i=0;i<len;)
		{
			if(pcuni::isHighSurrogate(text[i]))
				get(pos).Set(text[i],text[i+1],fore,back),
				cursorIncrease(pos),cursorIncrease(pos),
				i+=pcUNI_SURROGATE_WIDTH;
			else
			{
				int w=pcuni::charWidthInConsole(text[i]);
				if(pos.x+w>size.x)	cursorBreakLine(pos);
				get(pos).Set(text[i],fore,back);
				while(w--)	cursorIncrease(pos);
				i++;
			}
		}
	}
	inline void Print(Coord pos,String text)
		{Print(pos,-1,-1,text);}
	void Render(Coord pos)
	{
		Color nowFore=-1,nowBack=-1;
		Pixel &p=get(pos);
		Coord spos=pos,npos=pos;
		int n;
		for(int i=0;i<size.y;i++)
		{
			for(int j=0;j<size.x;j++)
			{
				p=canvas[i][j];
				if(p.isEmpty())	continue;
				spos.Set(pos.x+j,pos.y+i);
				if(npos!=spos)
					CursorGoto(spos),
					npos=spos;
				if(p.fore!=nowFore)	
					nowFore=p.fore,
					SetForegroundColor(nowFore);
				if(p.back!=nowBack)	
					nowBack=p.back,
					SetBackgroundColor(nowBack);
				p.putchar(false);
				n=p.width();
				while(n--)
					cursorIncrease(npos);
			}
			ResetAnsiStyle();
		}
	}
	Buffer(short w,short h):size({w,h})
		{resize(w,h);}
};