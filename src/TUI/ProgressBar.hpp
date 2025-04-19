#pragma once
#define PCL_TUI_PROGRESSBAR

#include<vector>
#include<ctime>
#include<cmath>
using std::vector;

#include"Element.hpp"

#define pcPB_SPEED_CALC_COUNT 10

class ProgressBar:public Element
{
protected:
	string type,title;
	double tot=100,advance=0,last=0;
	int stamp=0,start;
	string genInfo()
	{
		double percent=advance/tot*100,speed=0;
		int now=clock();
		if(now!=start)
			speed=advance/(now-start);
		int ms,sec,min;
		string estimate;
		if(speed==0)	estimate="--:--.---";
		else
		{
			ms=(tot-advance)/speed;
			sec=ms/1000;ms%=1000;
			min=sec/60;sec%=60;
			estimate=std::format("{:0>2}:{:0>2}.{:0>3}",min,sec,ms);
		}
		speed*=1000;
		return (style["title-space"]=="true"?" ":"")+std::vformat(style["bar-format"],
			std::make_format_args(advance,tot,percent,speed,estimate,title));
	}
	void drawBar(int n,int k,bool br)
	{
		int i=0;
		stylepri::Bar bar=style.GetBar();
		if(br)	std::print("{}",style.GetBorderStyle()),std::print("{}",bar.l);
		std::print("{}",style.GetBarStyle(true));
		for(;i<k;i++)	std::print("{}",bar.t);
		std::print("{}",style.GetBarStyle(false));
		for(;i<n;i++)	std::print("{}",bar.f);
		if(br)	std::print("{}",style.GetBorderStyle()),std::print("{}",bar.r);
	}
	string genTitle()
	{
		string t;
		if(style["title"]=="hidden")	t="";
		else
			if(style["title-space"]=="true")	t=title+" ";
			else	t=title;
		return t;
	}
public:
	pcpri::COORD Print(short x,short y,
		short visWidth=-1,map<string,StyleSheet>* c=nullptr)
	{
		printInit(x,y,c);
		stamp=clock(),start=clock();
		if(width==0)	style.GetSize(width,height);
		if(visWidth!=-1&&visWidth<width)	return {-1,-1};
		if(width==-1)	width=visWidth,visWidth=-2;
		HideCursor();
		CursorGoto(x,y);
		string t=genTitle(),u=genInfo();
		std::print("{}",style.GetTitleStyle());
		AnsiPrint("{}",t);
		bool br=style["bar-bracket"]=="true";
		int n=width-AnsiVisLen(t)-AnsiVisLen(u);
		if(br)	n-=2;
		int k=floor(advance/tot*n);
		drawBar(n,k,br);
		CursorGoto(x+n+AnsiVisLen(t)+(br?2:0),y);
		ResetAnsiStyle();
		std::print("{}",style.GetTitleStyle());
		AnsiPrint("{}",u);
		ShowCursor();
		return {short(left+width),short(top)};
	}
	void Reprint(short x,short y,map<string,StyleSheet>* c=nullptr)
	{
		printInit(x,y,c);
		if(width==0)	return;
		string t=genTitle(),u=genInfo();
		if(clock()-stamp<style.GetLazy()&&advance<tot)	return;
		stamp=clock(),last=advance;
		HideCursor();
		bool br=style["bar-bracket"]=="true";
		int n=width-AnsiVisLen(t)-AnsiVisLen(u);
		if(br)	n-=2;
		int k=floor(advance/tot*n);
		CursorGoto(x+AnsiVisLen(t),y);
		drawBar(n,k,br);
		CursorGoto(x+n+AnsiVisLen(t)+(br?2:0),y);
		ResetAnsiStyle();
		std::print("{}",style.GetTitleStyle());
		AnsiPrint("{}",u);
		ShowCursor();
		return;
	}
	void Update(double a)
	{
		advance=std::max(std::min(a,tot),0.0);
		Reprint(left,top);
	}
	void Reset(double a=0.0)
	{
		advance=a,last=0,stamp=clock(),start=clock();
		Print(left,top);
	}
	ProgressBar(string tt,double t,double a,StyleSheet s=""):
		Element(s),title(tt),tot(t),advance(a){}
	ProgressBar(string tt,StyleSheet s=string("")):
		Element(s),title(tt),tot(100),advance(0),last(0){}
};
