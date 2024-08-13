#ifndef PCL_CONSOLE
#define PCL_CONSOLE

#include<windows.h>
#include<pthread.h>
#include<cstdio>
#include<mutex>
using std::mutex;

const HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE hIn=GetStdHandle(STD_INPUT_HANDLE);

short ConDefaultColor=0x07;
char* ConsoleTitle="UnTitled Window";

/*HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH*/#include"Log.hpp"
	#ifdef PCL_LOG
	Log<1000> ConLog("Console.log",OVERWRITE);
	#endif

void CursorGoto(COORD Pos)
{
    SetConsoleCursorPosition(hOut,Pos);
	return;
}
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
 * The First Digit is for background. \n
 * The Second is for foregrund. \n
 * @note
 * ```
 * 0=Black
 * 1=Blue
 * 2=Green
 * 3=Aqua
 * 4=Red
 * 5=Purple
 * 6=Yellow
 * 7=White
 * 8=Gray
 * 9=Light_Blue
 * A=Light_Green
 * B=Light_Aqua
 * C=Light_Red
 * D=Light_Purple
 * E=Light_Yellow
 * F=Light_White
 * ```
 */
template<typename... types>
void ColorPrintf(short x,short y,int col,const char* format,types... args)
{
    CursorGoto(x,y);
    SetColorIO(col);
    printf(format,args...);
    SetColorIO(ConDefaultColor);
    return;
}
template<typename... types>
void ColorPrintf(COORD Pos,int col,const char* format,types... args)
{
    CursorGoto(Pos);
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

void ConTitleA(char *Title)
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

bool KeyDown(int vKey){return GetKeyState(vKey)&0x8000?true:false;}

#define PCgmp_INTERVAL 50
mutex lkInput;
namespace pcpri
{
	COORD MousePos;
	bool LastLDown=false,ThisLDown=false;
	bool LastRDown=false,ThisRDown=false;
	pthread_t ptReadIn;
	void* InReader(void* arg)
	{
		INPUT_RECORD MouseRec;
		DWORD res;
		while(true)
		{
			lkInput.lock();
			ReadConsoleInputA(hIn,&MouseRec,1,&res);
			MousePos=MouseRec.Event.MouseEvent.dwMousePosition;
			LastLDown=ThisLDown;ThisLDown=KeyDown(VK_LBUTTON);
			LastRDown=ThisRDown;ThisRDown=KeyDown(VK_RBUTTON);
			lkInput.unlock();
			Sleep(PCgmp_INTERVAL);
		}
		return NULL;
	}
};
void StartGetMouseInput()
{
	pthread_create(&pcpri::ptReadIn,NULL,pcpri::InReader,NULL);
	return;
}
COORD GetMousexy(){return pcpri::MousePos;}
void GetMousexy(COORD &mouse){mouse=pcpri::MousePos;}
/** @warning You may use `lkInput.lock()` before and `lkInput.unlock()` after. */
bool MouseLClick(){return pcpri::LastLDown!=pcpri::ThisRDown;}
/** @warning You may use `lkInput.lock()` before and `lkInput.unlock()` after. */
bool MouseRClick(){return pcpri::LastRDown!=pcpri::ThisRDown;}

void SetSelectState(bool ban)
{
	DWORD mode;
	GetConsoleMode(hIn,&mode);
	if(ban)	mode&=~ENABLE_QUICK_EDIT_MODE;
	else	mode&=ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hIn,mode);
}

#endif