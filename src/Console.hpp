#ifndef PCL_CONSOLE
#define PCL_CONSOLE

#include<windows.h>
#include<pthread.h>
#include<cstdio>
#include<mutex>
#include<wchar.h>
using std::mutex;

const HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE hIn=GetStdHandle(STD_INPUT_HANDLE);

short ConDefaultColor=0x07;
char* ConsoleTitle="UnTitled Window";

#ifdef PCL_LOG
Log<1000> ConLog("Console.log",OVERWRITE);
#endif

mutex lkOutput;

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
void SetColorIOEx(int fore,int back)
{
	if(back!=-1)
		printf("\033[48;2;%d;%d;%dm",
			(back&0xff0000)>>16,
			(back&0x00ff00)>>8,
			(back&0x0000ff));
	if(fore!=-1)
		printf("\033[38;2;%d;%d;%dm",
			(fore&0xff0000)>>16,
			(fore&0x00ff00)>>8,
			(fore&0x0000ff));
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
void ColorPrintf(short col,const char* format,types... args)
{
	lkOutput.lock();
	SetColorIO(col);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}

template<typename... types>
void ColorPrintfEx(int fore,int back,const char* format,types... args)
{
	lkOutput.lock();
	SetColorIOEx(fore,back);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}

template<typename... types>
void PosPrintf(short x,short y,const char* format,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	printf(format,args...);
	lkOutput.unlock();
	return;
}
template<typename... types>
void ColorPosPrintf(short col,short x,short y,const char* format,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	SetColorIO(col);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
template<typename... types>
void ColorPosPrintfEx(int fore,int back,short x,short y,const char* format,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	SetColorIOEx(fore,back);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}

\
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
/** @warning You may use `lkInput.lock()` before and `lkInput.unlock()` after. */
	COORD MousePos;
/** @warning You may use `lkInput.lock()` before and `lkInput.unlock()` after. */
	bool LastLDown=false,ThisLDown=false,LastRDown=false,ThisRDown=false;
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
bool MouseLClick()
{
	lkInput.lock();
	bool ret=pcpri::LastLDown!=pcpri::ThisRDown;
	lkInput.unlock();
	return ret;
}
bool MouseRClick()
{
	lkInput.lock();
	bool ret=pcpri::LastRDown!=pcpri::ThisRDown;
	lkInput.unlock();
	return ret;
}

void SetSelectState(bool ban)
{
	DWORD mode;
	GetConsoleMode(hIn,&mode);
	if(ban)	mode&=~ENABLE_QUICK_EDIT_MODE;
	else	mode&=ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hIn,mode);
}


#define CP_GBK 936
#define CP_US 437
void ConSetFontSize(int w,int h)
{
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize=sizeof(fontInfo);
	fontInfo.dwFontSize=(COORD){w,h};
	SetCurrentConsoleFontEx(hOut,FALSE,&fontInfo);
	return;
}

#endif