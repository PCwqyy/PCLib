#include<ctime>
#include<thread>
#include<chrono>
#include<conio.h>
#include"../../src/Container/Color.hpp"
#include"../../src/Utility/Ansi.hpp"
const std::string digits[][7]={
	{" ### ","#   #","#   #","#   #","#   #","#   #"," ### "},
	{"  #  "," ##  ","  #  ","  #  ","  #  ","  #  "," ### "},
	{" ### ","#   #","    #","   # ","  #  "," #   ","#####"},
	{" ### ","#   #","    #","   # ","    #","#   #"," ### "},
	{"#   #","#   #","#   #"," ####","    #","    #","    #"},
	{"#####","#    ","#### ","    #","    #","   # ","###  "},
	{" ### ","#    ","#### ","#   #","#   #","#   #"," ### "},
	{"#####","    #","    #","   # ","  #  "," #   "," #   "},
	{" ### ","#   #","#   #"," ### ","#   #","#   #"," ### "},
	{" ### ","#   #","#   #"," ### ","    #","    #","    #"},
};
Color DigitCol("dodgerblue");
void PrintDigit(int x,int y,int d)
{
	SetBackgroundColor(DigitCol);
	for(int i=0;i<7;i++)
		for(int j=0;j<5;j++)
			if(digits[d][i][j]!=' ')
				AnsiPosPrint(x+j*2,y+i,"  ");
	ResetAnsiStyle();
	for(int i=0;i<7;i++)
		for(int j=0;j<5;j++)
			if(digits[d][i][j]==' ')
				AnsiPosPrint(x+j*2,y+i,"  ");
}
void PrintNowColor()
{
	auto [r,g,b]=DigitCol.toRGB();
	auto [h,s,l]=DigitCol.toHSL();
	SetForegroundColor(DigitCol);
	AnsiPosPrint(15,10,"RGB: {:3} {:3} {:3} HSL: {:3} {:3} {:3}        ",r,g,b,h,s,l);
	ResetAnsiStyle();
}
int main()
{
	SetConsoleTitle("ANSI Clock Demo");
	AnsiPrint("PClib version: %Cf[gold]{}%/\n","25v20d");
	HideCursor();
	time_t nowtime,lasttime;
	tm* local;
	string buf;
	int x,y;
	char ch;
	bool cleared=false;
	PrintNowColor();
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(30));
		CursorGoto(0,1);
		cleared=false;
		while(kbhit())
		{
			if(!cleared)
				ClearCurrentLine(),
				cleared=true;
			ch=getch();
			if(ch==-32)
			{
				ch=getch();
				if(ch==75)	DigitCol.HueRotate(-3);
				if(ch==77)	DigitCol.HueRotate(3);
				PrintNowColor();
			}
		}
		nowtime=time(nullptr);
		if(nowtime==lasttime)	continue;
		local=localtime(&nowtime);
		lasttime=nowtime;
		buf=std::format("{:02}:{:02}:{:02}",
			local->tm_hour,local->tm_min,local->tm_sec);
		x=2,y=2;
		int len=buf.length();
		for(int i=0;i<len;i++)
			if(buf[i]==':')
				AnsiPosPrint(x+1,y+2,"%Cb[gray]  %/"),
				AnsiPosPrint(x+1,y+4,"%Cb[gray]  %/"),
				x+=5;
			else
				PrintDigit(x,y,buf[i]-'0'),
				x+=11;
	}
	return 0;
}