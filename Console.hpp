#ifndef PCL_CONSOLE
#define PCL_CONSOLE

#include<windows.h>
#include<pthread.h>
#include<cstdio>
#include<vector>

using std::vector;

const HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE hIn=GetStdHandle(STD_INPUT_HANDLE);

short ConDefaultColor=0x07;
short cW=-1,cH=-1,mW=0,mH=0;
char* ConsoleTitle="UnTitled Window";

void CursorGoto(short x,short y)
{
    SetConsoleCursorPosition(hOut,(COORD){x,y});
	return;
}
void SetColorIO(short col)
{
    SetConsoleTextAttribute(hOut,col);
	return;
}
COORD GetCursorxy()
{
	CONSOLE_SCREEN_BUFFER_INFO a;
	GetConsoleScreenBufferInfo(hOut,&a);
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
    CursorGoto(x,y);
    SetColorIO(col);
    printf(format,args...);
    SetColorIO(ConDefaultColor);
    return;
}
template<typename... types>
void ColorPrintf(int col,const char* format,types... args)
{
    SetColorIO(col);
    printf(format,args...);
    SetColorIO(ConDefaultColor);
    return;
}
void ConTitle(char *Title)
{
	ConsoleTitle=Title;
    SetConsoleTitleA(Title);
	Sleep(40);
    return;
} 
void ConSize(short BufLen,short BufHig)
{
	SetConsoleScreenBufferSize(hOut,COORD{BufLen,BufHig});
	return;
}
void CursorSize(unsigned int size)
{
	CONSOLE_CURSOR_INFO info;
	if(size<=0)			info={1,FALSE};
	else if(size>100)	info={100,TRUE};
	else				info={size,TRUE};
    SetConsoleCursorInfo(hOut,&info);
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
    GetCurrentConsoleFontEx(hOut,FALSE,&fontInfo);
	return fontInfo.dwFontSize;
}
void InitCharSize()
{
    CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize=sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hOut,FALSE,&fontInfo);
	cW=fontInfo.dwFontSize.X,cH=fontInfo.dwFontSize.Y;
}
namespace pcpri
{
	pthread_t ReadIn;
	COORD Mouse;
	void* InReader(void* arg)
	{
		CONSOLE_SCREEN_BUFFER_INFO bInfo;
		INPUT_RECORD MouseRec;
		DWORD res;
		while(true)
		{
			ReadConsoleInputA(hIn,&MouseRec,1,&res);
			Mouse=MouseRec.Event.MouseEvent.dwMousePosition;
		}
		return NULL;
	}
};
void StartGetMousexy()
{
	pthread_create(&pcpri::ReadIn,NULL,pcpri::InReader,NULL);
	return;
}
COORD GetMousexy(){return pcpri::Mouse;}
void GetMousexy(COORD &mouse){mouse=pcpri::Mouse;}

bool KeyDown(int vKey){return GetKeyState(vKey)&0x8000?true:false;}
void SetSelectState(bool ban)
{
	DWORD mode;
	GetConsoleMode(hIn,&mode);
	if(ban)	mode&=~ENABLE_QUICK_EDIT_MODE;
	else	mode&=ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hIn, mode);
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

#endif