#include<ctime>
#include<thread>
#include<chrono>
#include"../../src/TUI/Color.hpp"
#include"../../src/TUI/Ansi.hpp"
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
void PrintDigit(int x,int y,int d)
{
	for(int i=0;i<7;i++)
		for(int j=0;j<5;j++)
			if(digits[d][i][j]=='#')
				AnsiPosPrint(x+j*2,y+i,"%Cb[dodgerblue]  %/");
			else
				AnsiPosPrint(x+j*2,y+i,"  ");
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
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(30));
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
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
	}
	return 0;
}