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
	string type,title,st,sb,sf,su;
	double tot=100,advance=0,last=0;
	int stamp=0,speedIt=0,lazy=0;
	double speedCulc[pcPB_SPEED_CALC_COUNT]={0.0};
	string genInfo()
	{
		double percent=advance/tot*100,speed=0;
		int now=clock();
		if(now!=stamp)
			speedCulc[speedIt]=(advance-last)/(now-stamp),
			speedIt++,speedIt%=pcPB_SPEED_CALC_COUNT;
		for(int i=0;i<pcPB_SPEED_CALC_COUNT;i++)
			speed+=speedCulc[i];
		speed/=pcPB_SPEED_CALC_COUNT;
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
	void drawBar(int n,int k,bool br)
	{
		int i=0;
		stylepri::Bar bar=style.GetBar();
		if(br)	std::print("{}",sb),std::print("{}",bar.l);
		std::print("{}",sf);
		for(;i<k;i++)	std::print("{}",bar.t);
		std::print("{}",su);
		for(;i<n;i++)	std::print("{}",bar.f);
		if(br)	std::print("{}",sb),std::print("{}",bar.r);
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
	void getStyles()
	{
		if(st.empty())	st=style.GetTitleStyle();
		if(sb.empty())	sb=style.GetBorderStyle();
		if(sf.empty())	sf=style.GetBarStyle(true);
		if(su.empty())	su=style.GetBarStyle(false);
		lazy=style.GetLazy();
		return;
	}
public:
	pcpri::COORD Print(short x,short y,short visWidth=-1)
	{
		left=x,top=y;
		if(width==0)	style.GetSize(width,height);
		getStyles();
		if(visWidth!=-1&&visWidth<width)	return {-1,-1};
		if(width==-1)	width=visWidth,visWidth=-2;
		HideCursor();
		CursorGoto(x,y);
		string t=genTitle(),u=genInfo();
		std::print("{}",st);
		AnsiPrint("{}",t);
		int n=width-t.length()-u.length(),k=floor(advance/tot*n);
		bool br=style["bar-bracket"]=="true";
		drawBar(br?n-2:n,k,br);
		CursorGoto(x+n+t.length(),y);
		std::print("{}",st);
		AnsiPrint("{}",u);
		ShowCursor();
		return {short(left+width),short(top)};
	}
	void Reprint(short x,short y)
	{
		left=x,top=y;
		if(width==0)	return;
		getStyles();
		string t=genTitle(),u=genInfo();
		if(clock()-stamp<lazy)	return;
		stamp=clock(),last=advance;
		HideCursor();
		int n=width-t.length()-u.length(),k=floor(advance/tot*n);
		bool br=style["bar-bracket"]=="true";
		CursorGoto(x+t.length(),y);
		drawBar(br?n-2:n,k,br);
		CursorGoto(x+n+t.length(),y);
		std::print("{}",st);
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
		advance=a,last=0,stamp=0,speedIt=0,lazy=0;
		for(int i=0;i<pcPB_SPEED_CALC_COUNT;i++)
			speedCulc[i]=0;
		Print(left,top);
	}
	ProgressBar(string tt,double t,double a,StyleSheet s=""):
		title(tt),tot(t),advance(a){style=s;}
	ProgressBar(string tt,StyleSheet s=string("")):
		title(tt),tot(100),advance(0),last(0){style=s;}
};
