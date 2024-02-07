#ifndef PCL_CONSOLE
#define PCL_CONSOLE

#include<windows.h>
#include<cstdio>
#include<vector>

using std::vector;

short ConDefaultColor=0x07;
short cW=-1,cH=-1,mW=0,mH=0;
char* ConsoleTitle="No Title";

void APIgotoxy(short x,short y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
	return;
}
void APIsetColor(short col)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),col);
	return;
}
COORD GetAPIxy()
{
	CONSOLE_SCREEN_BUFFER_INFO a;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&a);
	return a.dwCursorPosition;
}
/**
  * @brief
  * Color is a base-16 Num. \n
  * The First Number is background color. \n
  * The Second Number is foregrund color. \n
  * @note
  * 0 black, \n
  * 1 blue, \n
  * 2 green, \n
  * 3 light green, \n
  * 4 red, \n
  * 5 purple, \n
  * 6 yellow, \n
  * 7 light gray, \n
  * 8 gray, \n
  * 9 light blue, \n
  * A light green, \n
  * B double light green, \n
  * C light red, \n
  * D light purple, \n
  * E light yellow, \n
  * F white. \n
  */
template<typename... types>
void ColorPrintf(int x,int y,int col,const char* format,types... args)
{
    APIgotoxy(x,y);
    APIsetColor(col);
    printf(format,args...);
    APIsetColor(ConDefaultColor);
    return;
}
template<typename... types>
void ColorPrintf(int col,const char* format,types... args)
{
    APIsetColor(col);
    printf(format,args...);
    APIsetColor(ConDefaultColor);
    return;
}
void ConTitle(char *Title)
{
	ConsoleTitle=Title;
    SetConsoleTitle(Title);
	Sleep(40);
    return;
} 
int ConSize(short BufLen,short BufHig,short ScrLen,short ScrHig)
{
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),COORD{BufLen,BufHig});
	HWND This=GetConsoleWindow();
	ConTitle(ConsoleTitle);
	return SetWindowPos(This,NULL,0,0,ScrLen*cW+mW,ScrHig*cH+mH,SWP_NOMOVE);
}
int ConSize(short ScrLen,short ScrHig)
{
	ScrLen+=5,ScrHig+=3;//?
	HWND This=GetConsoleWindow();
	return SetWindowPos(This,NULL,0,0,ScrLen*cW+mW,ScrHig*cH+mH,SWP_NOMOVE);
}
void APISize(unsigned int size)
{
	CONSOLE_CURSOR_INFO info;
	if(size<=0)			info={1,FALSE};
	else if(size>100)	info={100,TRUE};
	else				info={size,TRUE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    return;
}
void ConCantClose()
{
    DeleteMenu(GetSystemMenu(GetConsoleWindow(),FALSE),SC_CLOSE,MF_DISABLED);
    DrawMenuBar(GetConsoleWindow());
    return;
}
COORD GetConsoleFontSize()
{
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize=sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),FALSE,&fontInfo);
	return fontInfo.dwFontSize;
}
void InitCharSize()
{
    CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize=sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),FALSE,&fontInfo);
	cW=fontInfo.dwFontSize.X,cH=fontInfo.dwFontSize.Y;
}
void GetMousexy(POINT& Mouse)
{
	if(cW==-1||cH==-1)	return;
	HWND Hwnd=GetConsoleWindow();
	POINT Win={0,0};
	ScreenToClient(Hwnd,&Win);
	GetCursorPos(&Mouse);
	Mouse.x+=Win.x,Mouse.y+=Win.y;
	Mouse.x-=mW,Mouse.y-=mH;
	Mouse.x/=cW,Mouse.y/=cH;
	if(Mouse.x<0)	Mouse.x=0;
	if(Mouse.y<0)	Mouse.y=0;
	return;
}
bool KeyDown(int vKey){return GetKeyState(vKey)&0x8000?true:false;}
void SetSelectState(bool ban)
{
	HANDLE hStdin=GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin,&mode);
	if(ban)	mode&=~ENABLE_QUICK_EDIT_MODE;
	else	mode&=ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);
}

namespace pcpri
{
	bool isbt(char a,char l,char r)
		{return l<=a&&a<=r;}
	bool isable(char c)
		{return isbt(c,32,126)||c=='\n'||c=='\t';}
}

#define CS_TAB 4

#define CS_TB 0x01
#define CS_BT 0x02

#include"Log.hpp"
#include<bitset>
using std::bitset;
namespace ConScr
{
	template<typename Tp>
	void NewSquare(Tp** &a,int n,int m)
	{
		a=new Tp*[n];
		for(int i=0;i<n;i++)
			a[i]=new Tp[m];
		return;
	}
	struct ScreenDot
	{
		short col;
		char ch;
		bool operator==(ScreenDot a){return a.ch==ch&&a.col==col;}
		bool operator!=(ScreenDot a){return a.ch!=ch||a.col!=col;}
		ScreenDot operator= (ScreenDot a){col=a.col,ch-a.ch;return a;}
	};
	class Object
	{
		protected:
			int x,y,w,h,uid;
			short col;
			ScreenDot** Buf;
		public:
			bool changed,posc;
			void Color(short c)
			{
				for(int i=0;i<w;i++)
					for(int j=0;j<h;j++)
						Buf[i][j].col=c;
				col=c;changed=true;
			}
			void Position(int X,int Y)
				{x=X,y=Y,posc=true;}
			void Size(int W,int H)
			{
				ScreenDot** NewBuf;
				NewSquare(NewBuf,W,H);
				for(int i=0;i<w;i++)
					for(int j=0;j<h;j++)
						NewBuf[i][j]=Buf[i][j];
				w=W,h=H;
				delete Buf;
				Buf=NewBuf;
				posc=true;
			}
			int GetX(){return x;}
			int GetY(){return y;}
			int GetW(){return w;}
			int GetH(){return h;}
			int GetUid(){return uid;}
			short GetCol(){return col;}
			ScreenDot** GetBuf(){return Buf;}
			bool MouseOn(){}
			bool Click(){}
			bool Drag(){}
			COORD DragTo(){}
			void Init(int X,int Y,int W,int H,short c)
			{
				x=X,y=Y,w=W,h=H,col=c;
				NewSquare(Buf,w,h);
				for(int j=0;j<h;j++)
					for(int i=0;i<w;i++)
						Buf[i][j].col=col;
				changed=posc=true;
			}
			void Locate(int &x,int &y,int &w,int &h)
				{x=GetX(),y=GetY(),w=GetW(),h=GetH();}
	};
	class TextBox:public Object
	{
		protected:
			char* text=NULL;
		public:
			int modify()
			{
				for(int j=0;j<h;j++)
					for(int i=0;i<w;i++)
						Buf[i][j].ch=' ';
				if(text==NULL)	return -1;
				int now=0,i=0,j=0;
				while(text[now]!='\0'&&j<h)
				{
					if(text[now]=='\n')
						{j++,i=0,now++;continue;}
					else if(text[now]=='\t')
						do	Buf[i][j].ch=' ',i++;
						while(i%CS_TAB!=0&&i<w);
					else
						Buf[i][j].ch=text[now],i++;
					now++;
					if(i==w)	j++,i=0;
				}
				changed=false;
				return 0;
			}
			void Text(char* t)
			{
				if(t==NULL)	return;
				text=t;changed=true;
			}
			void Init(char* t,int X,int Y,int W,int H,short c)
				{Object::Init(X,Y,W,H,c);Text(t);}
			TextBox(char* t,int X,int Y,int W,int H,short c)
				{Init(t,X,Y,W,H,c);}
			TextBox(){}
	};
	class Button:public TextBox
	{
		protected:
			void (*func)();
			char *texton;
			short colon;
		public:
			void Init(void (*f)(),char* t,char* to,int X,int Y,int W,int H,short c,short co)
			{
				TextBox::Init(t,X,Y,W,H,c);
				colon=co;func=f;texton=to;
				changed=true;
			}
			Button(void (*f)(),char* t,char* to,int X,int Y,int W,int H,short c,short co)
				{Init(f,t,to,X,Y,W,H,c,co);}

	};
Log<100> ConLog("Consolelog.log",OVERWRITE);
	template<int maxn>
	class ConScreen
	{
		private:
			char* title;
			int sw,sh,sx,sy;
			short basecol;
			ScreenDot **Out;
			int **PrintMap;
			int UniCnt=0;
			void PutChar(int x,int y,ScreenDot ch)
			{
				if(Out[x][y]==ch)	return;
				Out[x][y]=ch;
				APIgotoxy(sx+x,sy+y);
				APIsetColor(ch.col);
				putchar(ch.ch);
				return;
			}
		public:
			void Init(char* Title,int x,int y,int w,int h)
			{
				title=Title;
				sw=w,sh=h,sx=x,sy=y;
				ConsoleTitle=Title;
				ConSize(sw,sh);
				NewSquare(Out,sw,sh);
				NewSquare(PrintMap,sw,sh);
				for(int i=0;i<w;i++)
					for(int j=0;j<h;j++)
						PrintMap[i][j]=-1;
				APISize(0);
			}
			struct iterator
			{
				int type,layer;
				Object* data;
			};
			iterator Stuff[maxn+5];
			int Add(Object* N,int type,int layer)
			{
				if(layer==-1||layer>UniCnt)	layer=UniCnt;
				else for(int i=0;i<UniCnt;i++)
					if(Stuff[i].layer>=layer)
						Stuff[i].layer++;
				Stuff[UniCnt]={type,layer,N};
				N->posc=N->changed=true;
				return UniCnt++;
			}
			int Add(TextBox &O,int layer)
			{
				Object* N=&O;
				return Add(N,CS_TB,layer);
			}
		private:
			void modifyItem()
			{
				for(int i=0;i<UniCnt;i++)
				{
					if(!Stuff[i].data->changed)
						continue;
					if(Stuff[i].type==CS_TB)
						((TextBox*)Stuff[i].data)->modify();
					//New controls will modify here~
				}
				return;
			}
			void flushPrintMap()
			{
				bool ifdo=false;
				for(int o=0;o<UniCnt;o++)
					if(Stuff[o].data->posc)
						{ifdo=true;break;}
				if(!ifdo)	return;//find if some control's pos changed
				int nx,ny,ex,ey;
				for(int o=0;o<UniCnt;o++)
				{
					Stuff[o].data->Locate(nx,ny,ex,ey);
					ex+=nx,ey+=ny;
					for(int i=nx;i<ex;i++)
						for(int j=ny;j<ey;j++)
							if(PrintMap[i][j]==-1
							 ||Stuff[o].layer<Stuff[PrintMap[i][j]].layer)
								PrintMap[i][j]=o;
				}
ConLog.lprintf("Debug","Oops!Now PrintMap be like:");
for(int j=0;j<sh;j++)
{
	for(int i=0;i<sw;i++)
		ConLog.aprintf("%d ",PrintMap[i][j]);
	ConLog.aprintf("\n");
}
			}
			void flushScreen()
			{
				int nx,ny,nw,nh;
				for(int o=0;o<UniCnt;o++)
				{
					Stuff[o].data->Locate(nx,ny,nw,nh);
					for(int i=0;i<nw;i++)
						for(int j=0;j<nh;j++)
							if(PrintMap[nx+i][ny+j]==o)
								PutChar(nx+i,ny+j,Stuff[o].data->GetBuf()[i][j]);
				}
			}
		public:
			void flush()
			{
				modifyItem();
				flushPrintMap();
				flushScreen();
			}
	};
};

#endif