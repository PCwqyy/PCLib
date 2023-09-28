#ifndef PCL_CONSOLE
#define PCL_CONSOLE

#include<windows.h>
#include<cstdio>
#include<vector>

using std::vector;

short ConDefaultColor=0x07;
short cW=9,cH=19,mW=6,mH=48;
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
	SetConsoleTitle("Temp Temp Window");Sleep(40);
	HWND This=FindWindow(NULL,"Temp Temp Window");
	ConTitle(ConsoleTitle);
	return SetWindowPos(This,NULL,0,0,ScrLen*cW+mW,ScrHig*cH+mH,SWP_NOMOVE);
}
int ConSize(short ScrLen,short ScrHig)
{
	SetConsoleTitle("Temp Temp Window");Sleep(40);
	HWND This=FindWindow(NULL,"Temp Temp Window");
	ConTitle(ConsoleTitle);
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
void GetMousexy(POINT& Mouse)
{
	HWND H=FindWindow(NULL,ConsoleTitle);
	POINT Win={0,0};
	ScreenToClient(H,&Win);
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

#define CS_VTMAX 1000
#define CS_TAB 4
#define CS_TB 0x01

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
			bool changed;
			ScreenDot** Buf;
		public:
			void Position(int X,int Y){x=X,y=Y;changed=true;}
			void Size(int W,int H){w=W,h=H;changed=true;}
			void Color(short c){col=c;changed=true;}
			int GetX(){return x;}
			int GetY(){return y;}
			int GetW(){return w;}
			int GetH(){return h;}
			int GetUid(){return uid;}
			int GetCol(){return col;}
			bool IsChanged(){return changed;}
			ScreenDot** GetBuf(){return Buf;}
			bool MouseOn(){}
			bool Click(){}
			bool Drag(){}
			COORD DragTo(){}
			void ObjInit(int X,int Y,int W,int H,short c)
				{x=X,y=Y,w=W,h=H,col=c;changed=true;}
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
			{
				ObjInit(X,Y,W,H,c);
				NewSquare(Buf,W,H);
				Text(t);
				for(int j=0;j<h;j++)
					for(int i=0;i<w;i++)
						Buf[i][j].col=col;
			}
			TextBox(char* t,int X,int Y,int W,int H,short c)
				{Init(t,X,Y,W,H,c);}
			TextBox(){}
	};
#include"Log.hpp"
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
			}Stuff[maxn+5];
			void fillPM(int xs,int xe,int ys,int ye,int uid)
			{
				int layer=Stuff[uid].layer;
ConLog.lprintf("fillPM","%d,%d %d,%d #%d z:%d",xs,ys,xe,ye,uid,layer);
				for(int i=xs;i<xe;i++)
					for(int j=ys;j<ye;j++)
					{
ConLog.lprintf("PrintMap","(%d,%d)=%d,l%d,%d %d",i,j,PrintMap[i][j],Stuff[PrintMap[i][j]].layer,uid,layer);
						if(PrintMap[i][j]==-1||Stuff[PrintMap[i][j]].layer>layer)
ConLog.lprintf("PrintMap","Get!"),
							PrintMap[i][j]=uid;
					}
				return;
			}
			int Add(TextBox &O,int layer)
			{
				Object* N=&O;
				if(layer==-1)	layer=UniCnt;
				else for(int i=0;i<UniCnt;i++)
					if(Stuff[i].layer>=layer)
						Stuff[i].layer++;
				Stuff[UniCnt]={CS_TB,layer,N};
				fillPM(O.GetX(),O.GetX()+O.GetW(),O.GetY(),O.GetY()+O.GetH(),UniCnt);
				return UniCnt++;
			}
			void flush()
			{
				for(int i=0;i<UniCnt;i++)
				{
					if(!Stuff[i].data->IsChanged())
						continue;
					if(Stuff[i].type==CS_TB)
						((TextBox*)Stuff[i].data)->modify();
				}
				int nx,ny,nw,nh,nuid=0;
				for(int o=0;o<UniCnt;o++)
				{
					nx=Stuff[o].data->GetX(),ny=Stuff[o].data->GetY();
					nw=Stuff[o].data->GetW(),nh=Stuff[o].data->GetH();
					for(int i=0;i<nw;i++)
						for(int j=0;j<nh;j++)
							if(PrintMap[nx+i][ny+j]==nuid)
								PutChar(nx+i,ny+j,Stuff[o].data->GetBuf()[i][j]);
					nuid++;
				}
			}
	};
};

#endif