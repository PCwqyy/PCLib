#pragma once
#define PC_TUI_BUFFER

#include<vector>
#include"../Container/String.hpp"
#include"Color.hpp"
#include"../Utility/Ansi.hpp"
#include"OS.hpp"
#include"Util.hpp"
using std::vector;

class Pixel
{
private:
	char16_t high,low;
	bool surrogate=false;
public:
	Color fore,back;
	inline void Reset(){Set(u' ',-1,-1);}
	void Set(char16_t c,Color f=-1,Color b=-1)
		{high=c;fore=f,back=b;surrogate=false;}
	void SetSurrogate(char16_t h,char16_t l,Color f=-1,Color b=-1)
		{high=h,low=l;fore=f,back=b;surrogate=true;}
	Pixel(char16_t c,Color f=-1,Color b=-1)
		{Set(c,f,b);}
	Pixel(char16_t h,char16_t l,Color f=-1,Color b=-1)
		{SetSurrogate(h,l,f,b);}
	Pixel(Color back)
		{Set(u' ',-1,back);}
	Pixel(){Set(u' ',-1,-1);}
	inline bool isSurrogate(){return surrogate;}
	/// @brief Check if the pixel is occupied by a wide character in front of this
	inline bool isEmpty(){return high==u'\0';}
	inline bool isSpace(){return high==u' ';}
	void putchar(bool color=true)
	{
		if(color)
			SetForegroundColor(fore),
			SetBackgroundColor(back);
		String s;s.Append(high);
		if(surrogate)	s.Append(low);
		OutputUnicode(s);
	}
	inline int width()
		{return pcuni::charWidthInConsole(high);}
	Pixel GiveBackColor(Color f,Color b)
	{
		if(isSurrogate())
			return Pixel(high,low,fore|f,back|b);
		else
			return Pixel(high,fore|f,back|b);
	}
};

#define pcTUI_BUF_DEFAULT_WIDTH 20
#define pcTUI_BUF_DEFAULT_HEIGHT 5
#define pcTUI_BUF_EXTEND_MULTIPLE 1.5

class Buffer
{
private:
	Coord size;
	Color BackgroundColor;
	vector<vector<Pixel>> canvas;
	void resize(short w,short h)
	{
		int len=canvas.size();
		for(int i=0;i<len;i++)
			canvas[i].resize(w);
		canvas.resize(h,vector<Pixel>(w));
		size.Set(w,h);
	}
	Pixel& get(Coord pos)
	{
		if(pos.x<0||pos.y<0)
			throw pc::Exception(pcXPT_OUT_OF_RANGE,"Invalid buffer position access");
		if(pos.y>=size.y||pos.x>=size.x)
			resize(std::max(pos.x,size.x)*pcTUI_BUF_EXTEND_MULTIPLE,
				std::max(pos.y,size.y)*pcTUI_BUF_EXTEND_MULTIPLE);
		return canvas[pos.y][pos.x];
	}
	/**
	 * @brief Increase cursor position by one, with line break
	 * @param cur Current cursor position
	 * @param p View start position
	 * @param s View size
	 */
	inline void cursorIncrease(Coord& cur,Coord p,Coord s)
	{
		cur.x++;
		if(cur.x>p.x+s.x)	cur.x=p.x,cur.y++;
		if(cur.y>p.y+s.y)	cur=p;
	}
	inline void cursorIncrease(Coord& cur)
		{cursorIncrease(cur,{0,0},size);}
	/**
	 * @brief Move cursor to the beginning of next line
	 * @param cur Current cursor position
	 * @param p View start position
	 * @param s View size
	 */
	inline void cursorBreakLine(Coord& cur,Coord p,Coord s)
	{
		cur.x=p.x,cur.y++;
		if(cur.y>p.y+s.y)	cur=p;
	}
	inline void cursorBreakLine(Coord& cur)
		{cursorBreakLine(cur,{0,0},size);}
public:
	void SetBkgColor(Color col)
		{BackgroundColor=col;}
	void Clear()
	{
		for(int i=0;i<size.y;i++)
			for(int j=0;j<size.x;j++)
				canvas[i][j].Set(u' ',-1,BackgroundColor);
	}
	void SetWidth(short w)
	{
		if(w<=size.x)	return;
		resize(w,size.y);
	}
	void ShinkToFit()
	{
		short maxw=0,i,j;
		for(i=0;i<size.y;i++)
		{
			for(j=size.x-1;j>=0;j--)
				if(!canvas[i][j].isSpace())
				{
					maxw=std::max(maxw,short(j+1));
					break;
				}
			if(j<0)	break;
		}
		resize(maxw,i);
	}
	Coord Size(){return size;}
	/**
	 * @brief Print text at position with colors
	 * @param pos Start position
	 * @param vpos View start position
	 * @param vsize View size
	 */
	Coord Render(Coord pos,Color fore,Color back,String text,Coord vpos,Coord vsize)
	{
		int len=text.Size(),w;
		for(int i=0;i<len;)
		{
			w=pcuni::charWidthInConsole(text[i]);
			if(pos.x+w>vpos.x+vsize.x)
				cursorBreakLine(pos,vpos,vsize);
			if(pcuni::isHighSurrogate(text[i]))
				if(i+1<len)
					get(pos).SetSurrogate(text[i],text[i+1],fore,back),i+=2;
				else
					// truncated surrogate pair: treat high surrogate as replacement
					get(pos).Set(text[i],fore,back),i++;
			else
				get(pos).Set(text[i],fore,back),i++;
			w--,cursorIncrease(pos,vpos,vsize);
			while(0<w--)	// 被宽字符占用的空格为 u'\0'
				get(pos).Set(u'\0',-1,BackgroundColor),
				cursorIncrease(pos,vpos,vsize);
		}
		return pos;
	}
	inline Coord Render(Coord pos,Color fore,Color back,String text)
		{return Render(pos,fore,back,text,{0,0},size);}
	inline Coord Render(Coord pos,String text)
		{return Render(pos,-1,-1,text,{0,0},size);}
	void RenderTo(Buffer& Target,Coord pos)
	{
		Coord npos=pos;
		for(int i=0;i<size.y;i++)
			for(int j=0;j<size.x;j++)
			{
				npos.Set(pos.x+j,pos.y+i);
				Target.get(npos)=canvas[i][j].GiveBackColor(-1,BackgroundColor);
			}
	}
	void Print(Coord pos)
	{
		ResetAnsiStyle();
		Color nowFore=-1,nowBack=-1;
		Pixel p;
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
					CursorGoto(spos),npos=spos;
				if(p.fore!=nowFore)	
					nowFore=p.fore,SetForegroundColor(p.fore);
				if(p.back!=nowBack)	
					nowBack=p.back,SetBackgroundColor(p.back);
				p.putchar(false);
				n=p.width();
				while(n--)
					cursorIncrease(npos);
			}
			ResetAnsiStyle();
		}
	}
	Buffer(short w,short h,Color back=-1):
		size({w,h}),BackgroundColor(back),
		canvas(h,vector<Pixel>(w,Pixel(back)))
		{resize(w,h);Clear();}
	Buffer():
		size({pcTUI_BUF_DEFAULT_WIDTH,pcTUI_BUF_DEFAULT_HEIGHT}),
		BackgroundColor(-1),
		canvas(size.y,vector<Pixel>(size.x,Pixel(BackgroundColor)))
		{resize(size.x,size.y);Clear();}
};