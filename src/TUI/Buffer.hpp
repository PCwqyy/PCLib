#define PC_TUI_BUFFER
#pragma once

#include<vector>
#include"Color.hpp"
#include"Ansi.hpp"
#include"OS.hpp"
#include"Util.hpp"
#include"../Container/String.hpp"
#include"Color.hpp"
using std::vector;

#include <cstdint>

namespace pcuni
{
	inline bool isHighSurrogate(char16_t ch)
		{return ch>=0xD800&&ch<=0xDBFF;}
	inline bool isLowSurrogate(char16_t ch)
		{return ch>=0xDC00&&ch<=0xDFFF;}

	inline int getUnicodePlane(char16_t ch){return 0;}
    /// @brief Get the Unicode plane for surrogates.
	inline int getUnicodePlane(char16_t high,char16_t low)
	{
		if(!isHighSurrogate(high)||!isLowSurrogate(low)) // invalid
			return -1;
		unsigned int code=((high-0xD800)<<10)+(low-0xDC00)+0x10000;
		return code>>16;
	}
	unsigned int surrogateToCode(char16_t high,char16_t low)
	{
		if(!isHighSurrogate(high)||!isLowSurrogate(low)) // invalid
			return -1;
		unsigned int u10=high-0xD800;
		unsigned int l10=low-0xDC00;
		return 0x10000+(u10<<10)+l10;
	}

    struct range{unsigned short first,last;};
    bool biSearch(unsigned short u,const range *table,size_t max)
    {
        int l=0,r=max;
        while(l<r)
		{
            int m=(l+r)/2;
            if(u<table[m].first)		r=m;
            else if(u>table[m].last)	l=m+1;
            else return true;
        }
        return false;
    }

    /* Combining characters (width 0) - from Markus Kuhn's wcwidth */
    const range combining[]=
	{
        {0x0300,0x036F},{0x0483,0x0489},{0x0591,0x05BD},{0x05BF,0x05BF},
        {0x05C1,0x05C2},{0x05C4,0x05C5},{0x05C7,0x05C7},{0x0610,0x061A},
        {0x064B,0x065F},{0x0670,0x0670},{0x06D6,0x06DC},{0x06DF,0x06E4},
        {0x06E7,0x06E8},{0x06EA,0x06ED},{0x0711,0x0711},{0x0730,0x074A},
        {0x07A6,0x07B0},{0x07EB,0x07F3},{0x0816,0x0819},{0x081B,0x0823},
        {0x0825,0x0827},{0x0829,0x082D},{0x0859,0x085B},{0x08D3,0x08E1},
        {0x08E3,0x0903},{0x093A,0x093C},{0x093E,0x094F},{0x0951,0x0957},
        {0x0962,0x0963},{0x0981,0x0983},{0x09BC,0x09BC},{0x09BE,0x09C4},
        {0x09C7,0x09C8},{0x09CB,0x09CD},{0x09D7,0x09D7},{0x09E2,0x09E3},
        {0x0A01,0x0A03},{0x0A3C,0x0A3C},{0x0A3E,0x0A42},{0x0A47,0x0A48},
        {0x0A4B,0x0A4D},{0x0A51,0x0A51},{0x0A70,0x0A71},{0x0A75,0x0A75},
        {0x0A81,0x0A83},{0x0ABC,0x0ABC},{0x0ABE,0x0AC5},{0x0AC7,0x0AC9},
        {0x0ACB,0x0ACD},{0x0AE2,0x0AE3},{0x0B01,0x0B03},{0x0B3C,0x0B3C},
        {0x0B3E,0x0B44},{0x0B47,0x0B48},{0x0B4B,0x0B4D},{0x0B56,0x0B57},
        {0x0B62,0x0B63},{0x0B82,0x0B82},{0x0BBE,0x0BC2},{0x0BC6,0x0BC8},
        {0x0BCA,0x0BCD},{0x0BD7,0x0BD7},{0x0C00,0x0C04},{0x0C3E,0x0C44},
        {0x0C46,0x0C48},{0x0C4A,0x0C4D},{0x0C55,0x0C56},{0x0C62,0x0C63},
        {0x0C81,0x0C83},{0x0CBC,0x0CBC},{0x0CBE,0x0CC4},{0x0CC6,0x0CC8},
        {0x0CCA,0x0CCD},{0x0CD5,0x0CD6},{0x0CE2,0x0CE3},{0x0D01,0x0D03},
        {0x0D3B,0x0D3C},{0x0D3E,0x0D44},{0x0D46,0x0D48},{0x0D4A,0x0D4D},
        {0x0D57,0x0D57},{0x0D62,0x0D63},{0x0D81,0x0D83},{0x0DCA,0x0DCA},
        {0x0DCF,0x0DD4},{0x0DD6,0x0DD6},{0x0DD8,0x0DDF},{0x0DF2,0x0DF3},
        {0x0E31,0x0E31},{0x0E34,0x0E3A},{0x0E47,0x0E4E},{0x0EB1,0x0EB1},
        {0x0EB4,0x0EBC},{0x0EC8,0x0ECD},{0x0F18,0x0F19},{0x0F35,0x0F35},
        {0x0F37,0x0F37},{0x0F39,0x0F39},{0x0F3E,0x0F3F},{0x0F71,0x0F84},
        {0x0F86,0x0F8B},{0x0F90,0x0F95},{0x0F97,0x0F97},{0x0F99,0x0FAD},
        {0x0FB1,0x0FB7},{0x0FB9,0x0FB9},{0x20D0,0x20DC},{0x20E1,0x20E1},
        {0x302A,0x302F},{0x3099,0x309A},{0xFE20,0xFE2F}
    };
    /* East Asian wide/fullwidth ranges (width 2) */
    const range wide[]=
	{
        {0x1100,0x115F},{0x2329,0x232A},{0x2E80,0xA4CF},
        {0xAC00,0xD7A3},{0xF900,0xFAFF},{0xFE10,0xFE19},
        {0xFE30,0xFE6F},{0xFF00,0xFF60},{0xFFE0,0xFFE6}
    };

    /**
     * @brief Get display width for a single UTF-16 code unit in a monospace console:
     *  - 0 for NUL,control codes and combining marks
     *  - 2 for East Asian wide/fullwidth characters, surrogates
     *  - 1 otherwise
     */
    int charWidthInConsole(char16_t ch)
    {
        const unsigned short u=static_cast<unsigned short>(ch);
        // C0/C1 control characters
        if(u==0)			return 0;
        if(u<0x20)			return 0;
        if(u>=0x7f&&u<0xa0)	return 0;
        // Surrogates
        if(u>=0xD800&&u<=0xDFFF)	return 2;
        // Combining marks
        if(biSearch(u,combining,sizeof(combining)/sizeof(range)))
            return 0;
        // East Asian Wide/Fullwidth -> width 2
        if(biSearch(u,wide,sizeof(wide)/sizeof(range)))
            return 2;
        return 1; // default
    }
} // namespace pcuni


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
	bool isSurrogate(){return surrogate;}
	void putchar(bool color=true)
	{
		SetForegroundColor(fore);
		SetBackgroundColor(back);
		String s;
		if(surrogate)
			s.Append(high),
			s.Append(low);
		else
			s.Append(high);
		OutputUnicode(s);
	}
};

class Buffer
{
private:
	vector<vector<Pixel>> canvas;
	util::Coord size;
	void resize(short w,short h)
	{
		for(auto i:canvas)
			i.resize(w);
		canvas.resize(h,vector<Pixel>(w));
	}
	Pixel& get(util::Coord pos){return canvas[pos.y][pos.x];}
	inline void cursorIncrease(util::Coord& cur)
	{
		cur.x++;
		if(cur.x>=size.x)	cur.x=0,cur.y++;
		if(cur.y>=size.y)	cur={0,0};
	}
public:
	void Print(util::Coord pos,Color fore,Color back,String text)
	{
		int len=text.Size();
		for(int i=0;i<len;)
		{
			if(pcuni::isHighSurrogate(text[i]))
				get(pos).Set(text[i],text[i+1],back,fore),
				cursorIncrease(pos),cursorIncrease(pos),
				i+=2;
			else
			{
				get(pos).Set(text[i],back,fore);
				int w=pcuni::charWidthInConsole(text[i]);
				while(w--)	cursorIncrease(pos);
				i++;
			}
		}
	}
	inline void Print(util::Coord pos,String text)
		{Print(pos,-1,-1,text);}
	void Render(util::Coord pos)
	{
		Color nowFore=-1,nowBack=-1;
		for(int i=0;i<size.y;i++)
		{
			CursorGoto(pos.x,pos.y+i);
			for(int j=0;j<size.x;j++)
			{
				Pixel &p=canvas[i][j];
				if(p.fore!=nowFore)	
					nowFore=p.fore,
					SetForegroundColor(nowFore);
				if(p.back!=nowBack)	
					nowBack=p.back,
					SetBackgroundColor(nowBack);
				p.putchar(false);
			}
			ResetAnsiStyle();
		}
	}
	Buffer(short w,short h):size({w,h})
		{resize(w,h);}
};