#pragma once
#define PCL_TUI_PROGRESSBAR

#include<vector>
#include<ctime>
using std::vector;

#include"Element.hpp"

class ProgressBar:public Element
{
protected:
	string type,title;
	double tot=100,advance=0,last=0;
	int stamp=0;
	string genInfo()
	{
		double percent=advance/tot*100;
		int now=clock();
		double speed=(advance-last)/(now-stamp);
		stamp=now,last=advance;
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
		return std::vformat(style["bar-format"],
			std::make_format_args(advance,tot,percent,speed,estimate,title));
	}
	void drawBar(int n)
	{
		bool br=style["bar-bracket"]=="true";
		if(br)	n-=2;
		stylepri::Bar bar=style.GetBar();
		int k=(advance/tot)*n,i=0;
		if(br)	style.ApplyBorderStyle(),std::print("{}",bar.l);
		style.ApplyBarStyle(true);
		for(;i<k;i++)	std::print("{}",bar.t);
		style.ApplyBarStyle(false);
		for(;i<n;i++)	std::print("{}",bar.f);
		if(br)	style.ApplyBorderStyle(),std::print("{}",bar.r);
	}
	void drawTitle(string& t)
	{
		if(style["title"]=="hidden")	t="";
		else
		{
			if(style["title-space"]=="true")	t=title+" ";
			else	t=title;
			style.ApplyTitleStyle();
			AnsiPrint("{}",t);
		}
	}
public:
	pcpri::COORD Print(short x,short y,short visWidth=-1)
	{
		left=x,top=y;
		if(width==0)
			style.GetSize(width,height);
		if(visWidth!=-1&&visWidth<width)	return {-1,-1};
		if(width==-1)	width=visWidth,visWidth=-2;
		HideCursor();
		CursorGoto(x,y);
		string t,u=genInfo();
		drawTitle(t);
		drawBar(width-t.length()-u.length());
		style.ApplyTitleStyle();
		std::print("{}",u);
		ShowCursor();
		return {short(left+width),short(top)};
	}
	void Update(double a,double t=0)
	{
		advance=std::max(std::min(a,tot),0.0);
		if(t!=0)	tot=t;
		Print(left,top);
	}
	ProgressBar(string tt,double t,double a,StyleSheet s=""):
		title(tt),tot(t),advance(a){style=s;}
	ProgressBar(string tt,StyleSheet s=string("")):
		title(tt),tot(100),advance(0),last(0){style=s;}
};
