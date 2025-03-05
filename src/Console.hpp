#ifndef PCL_CONSOLE
#define PCL_CONSOLE

#include<windows.h>
#include<thread>
#include<cstdio>
#include<mutex>
#include<cwchar>
#include<conio.h>
using std::thread;
using std::mutex;

#define pcCS_MAX_WINDOW_NAME 500

const HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE hIn=GetStdHandle(STD_INPUT_HANDLE);

short ConDefaultColor=0x07;
char ConsoleTitle[pcCS_MAX_WINDOW_NAME]="UnTitled Window";

#ifdef PCL_LOG
Log<1000> ConLog("Console.log",OVERWRITE);
#endif

mutex lkOutput,lkOutputOpt;

void CursorGoto(COORD Pos)
{
	lkOutputOpt.lock();
	SetConsoleCursorPosition(hOut,Pos);
	lkOutputOpt.unlock();
	return;
}
void CursorGoto(short x,short y)
{
	lkOutputOpt.lock();
	SetConsoleCursorPosition(hOut,(COORD){x,y});
	lkOutputOpt.unlock();
	return;
}
void SetColorIO(short col)
{
	lkOutputOpt.lock();
	SetConsoleTextAttribute(hOut,col);
	lkOutputOpt.unlock();
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
 * The Second is for foreground. \n
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

#ifdef UNICODE

#define pcCS_MAX_PRINT_BUFFER 1000
wchar_t ptwBuffer[pcCS_MAX_PRINT_BUFFER];
template<typename... types>
void printf(const wchar_t* format,types ...args)
{
	wsprintfW(ptwBuffer,format,args...);
	WriteConsoleW(hOut,ptwBuffer,wcslen(ptwBuffer),NULL,NULL);
	return;
}
template<typename... types>
void PosPrintf(short x,short y,const wchar_t* format,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	printf(format,args...);
	lkOutput.unlock();
	return;
}
#endif

#ifdef PCL_COLOR
void SetColorIOEx(Color fore,Color back)
{
	lkOutputOpt.lock();
	if(!back.DontModify())
		printf("\033[48;2;%d;%d;%dm",
			back.R,back.G,back.B);
	if(!fore.DontModify())
		printf("\033[38;2;%d;%d;%dm",
			fore.R,fore.G,fore.B);
	lkOutputOpt.unlock();
	return;
}
template<typename... types>
void ColorPrintfEx(Color fore,Color back,const char* format,types... args)
{
	lkOutput.lock();
	SetColorIOEx(fore,back);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
template<typename... types>
void ColorPosPrintfEx(Color fore,Color back,short x,short y,const char* format,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	SetColorIOEx(fore,back);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}

#ifdef UNICODE
template<typename... types>
void ColorPrintfEx(Color fore,Color back,const wchar_t* format,types... args)
{
	lkOutput.lock();
	SetColorIOEx(fore,back);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
template<typename... types>
void ColorPosPrintfEx(Color fore,Color back,short x,short y,const wchar_t* format,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	SetColorIOEx(fore,back);
	printf(format,args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
#endif

#endif

#define pcCS_INTERVAL 50
#define pcCS_MAX_BUFFER 1024
class ConCursor
{
private:
	COORD cursor;
	mutex lk;
	thread ptInput;
	char buffer[pcCS_MAX_BUFFER];
	bool end;
	int it=0,len=0,in;
	void scanner(bool print,short col)
	{
		for(int i=0;i<pcCS_MAX_BUFFER;i++)
			buffer[i]='\0';
		while(true)
		{
			if(end)	return;
			while(kbhit())
			{
				lk.lock();
				in=getch();
				if(in=='\b')
					if(it>0)
					{
						for(int i=it;i<len;i++)
							buffer[i-1]=buffer[i];
						len--,it--;
						buffer[len]='\0';
						if(print)
							Seek(-1,0),
							PrintfNoMove(col,"%s ",buffer+it);
					}
					else;
				else if(in=='\n'||in=='\r');
				else if(in==224)
				{
					int in2=getch();
					if(print)
					{
						if(in2==75&&it>0)
							Seek(-1,0),it--;
						if(in2==77&&it<len)
							Seek(1,0),it++;
					}
					else
					{
						if(in2==75&&it>0)	it--;
						if(in2==77&&it<len) it++;
					}
				}
				else
				{
					for(int i=len;i>it;i--)
						buffer[i]=buffer[i-1];
					buffer[it]=in,
					PrintfNoMove(col,"%s",buffer+it);
					Seek(1,0);
					it++;
					len++;
				}
				lk.unlock();
			}
			Sleep(pcCS_INTERVAL);
		}
		return;
	}
public:
	void Goto(short x,short y)
	{
		if(x==-1)	x=cursor.X;
		if(y==-1)	y=cursor.Y;
		cursor=(COORD){x,y};
		CursorGoto(cursor);
		return;
	}
	void Goto(COORD dest)
	{
		cursor=dest;
		CursorGoto(cursor);
		return;
	}
	void Seek(int dx,int dy)
	{
		cursor.X+=dx;
		cursor.Y+=dy;
		CursorGoto(cursor);
		return;
	}
	COORD Query(){return cursor;}
	template<typename ...types>
	void Printf(short color,const char* format,types ...args)
	{
		lkOutput.lock();
		CursorGoto(cursor);
		SetColorIO(color);
		printf(format,args...);
		cursor=GetCursorxy();
		SetColorIO(ConDefaultColor);
		lkOutput.unlock();
		return;
	}
	template<typename ...types>
	void PrintfNoMove(short color,const char* format,types ...args)
	{
		lkOutput.lock();
		CursorGoto(cursor);
		SetColorIO(color);
		printf(format,args...);
		CursorGoto(cursor);
		SetColorIO(ConDefaultColor);
		lkOutput.unlock();
		return;
	}
	void StartScan(bool print=false,short col=ConDefaultColor)
	{
		end=false;
		ptInput=thread(scanner,this,print,col);
		return;
	}
	void EndScan()
	{
		end=true;
		ptInput.join();
		return;
	}
	void ClearScanPrint()
	{
		lk.lock();
		Seek(-it,0);
		for(int i=0;i<len;i++)
			Printf(ConDefaultColor," ");
		Seek(it-len,0);
		lk.unlock();
		return;
	}
	void ClearScan()
	{
		lk.lock();
		Seek(-it,0);
		for(int i=0;i<len;i++)
			Printf(ConDefaultColor," ");
		Seek(-len,0);
		for(int i=0;i<pcCS_MAX_BUFFER;i++)
			buffer[i]='\0';
		it=0,len=0;
		lk.unlock();
		return;
	}
	char* GetScan(){return buffer;}
};

void ConTitleA(const char *Title)
{
	strcpy(ConsoleTitle,Title);
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

mutex lkInput;
namespace pcpri
{
/** @warning You may use `lkInput.lock()` before and `lkInput.unlock()` after. */
	COORD MousePos;
/** @warning You may use `lkInput.lock()` before and `lkInput.unlock()` after. */
	bool LastLDown=false,ThisLDown=false,LastRDown=false,ThisRDown=false;
	pthread_t ptReadIn;
	void InReader()
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
			Sleep(pcCS_INTERVAL);
		}
		return;
	}
};
void StartGetMouseInput()
{
	thread ptReadIn(pcpri::InReader);
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
void ConSetFontSize(short w,short h)
{
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize=sizeof(fontInfo);
	fontInfo.dwFontSize=(COORD){w,h};
	SetCurrentConsoleFontEx(hOut,FALSE,&fontInfo);
	return;
}

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
bool isWindowsTerminal(){
	if (hOut==INVALID_HANDLE_VALUE)
		return false;
	DWORD mode;
	if (!GetConsoleMode(hOut,&mode))
		return false;
	if (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
		return true;
	return false;
}

#endif