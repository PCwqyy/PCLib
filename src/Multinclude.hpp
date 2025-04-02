// IO x Rational
#ifdef PCL_IO
#ifndef PCL_RATIONAL
#define PCL_IO_INCLUDE_1ST
#endif
#endif

#ifdef PCL_IO
#ifdef PCL_RATIONAL
#ifdef PCL_IO_INCLUDE_1ST
#error Do not include IO before rational!
#endif
#ifndef PCL_IO_X_RATIONAL
#define PCL_IO_X_RATIONAL
namespace pc
{
template<typename Tp>
int sprinter(char* Dest,basicRational<Tp>& Th)
{
	Th.ToString(pcpri::temp);
	int tlen=strlen(pcpri::temp);
	pcpri::temp[tlen]='\0';
	strcpy(Dest,pcpri::temp);
	return tlen;
}
template<typename Tp>
int sscanner(const char* Src,basicRational<Tp>& Th)
{
	sscanf(Src,"%s",pcpri::temp);
	Th=pcpri::temp;
	return strlen(pcpri::temp);
}
}//namespace
#endif
#endif
#endif


// Console x Color
#ifdef PCL_CONSOLE
#ifdef PCL_COLOR
#ifndef PCL_COLOR_X_CONSOLE
#define PCL_COLOR_X_CONSOLE
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
#endif
#endif

// Console x IO
#ifdef PCL_CONSOLE
#ifdef PCL_IO
#ifndef PCL_CONSOLE_X_IO
#define PCL_CONSOLE_X_IO

namespace pcpri
{
	struct color_changer{short col;};
	void ColorPrintNoLock(){return;}
	template<typename Tp,typename... types>
	void ColorPrintNoLock(Tp arg,types... args)
	{
		pc::sprinter(pcpri::buff,arg);
		int now=0;
		while(pcpri::buff[now]!='\0')
			putchar(pcpri::buff[now]),now++;
		ColorPrintNoLock(args...);
		return;
	}
	template<typename... types>
	void ColorPrintNoLock(color_changer col,types ...args)
	{
		SetColorIO(col.col);
		ColorPrintNoLock(args...);
	}
}
/// @brief change color for `ColorPrint()`
pcpri::color_changer CC(short col)
	{return pcpri::color_changer({col});}
template<typename... types>
void ColorPrint(types... args)
{
	lkOutput.lock();
	pcpri::ColorPrintNoLock(args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
template<typename... types>
void PosPrintf(short x,short y,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	pc::print(args...);
	lkOutput.unlock();
	return;
}
template<typename... types>
void ColorPosPrint(short x,short y,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	pcpri::ColorPrintNoLock(args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
#endif

// Console x Color x IO
#ifdef PCL_COLOR
#ifndef PCL_CONSOLE_X_COLOR_X_IO
#define PCL_CONSOLE_X_COLOR_X_IO

namespace pcpri
{
	struct color_changerEx{Color fore,back;};
	void ColorPrintNoLockEx(){return;}
	template<typename Tp,typename... types>
	void ColorPrintNoLockEx(Tp arg,types... args)
	{
		pc::sprinter(pcpri::buff,arg);
		int now=0;
		while(pcpri::buff[now]!='\0')
			putchar(pcpri::buff[now]),now++;
		ColorPrintNoLockEx(args...);
		return;
	}
	template<typename... types>
	void ColorPrintNoLockEx(color_changerEx col,types ...args)
	{
		SetColorIOEx(col.fore,col.back);
		ColorPrintNoLockEx(args...);
	}
}
/// @brief change color for `ColorPrint()`
pcpri::color_changerEx CCEx(Color fore)
	{return pcpri::color_changerEx({fore,-1});}
pcpri::color_changerEx CCEx(Color fore,Color back)
	{return pcpri::color_changerEx({fore,back});}
template<typename... types>
void ColorPrintEx(types... args)
{
	lkOutput.lock();
	pcpri::ColorPrintNoLockEx(args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
template<typename... types>
void ColorPosPrintEx(short x,short y,types... args)
{
	lkOutput.lock();
	CursorGoto(x,y);
	pcpri::ColorPrintNoLockEx(args...);
	SetColorIO(ConDefaultColor);
	lkOutput.unlock();
	return;
}
#endif
#endif

#endif
#endif

#ifndef PCL_COLOR
#ifdef PCL_ANSI
#define PCL_ANSI_INCLUDE_1ST
#endif
#else
#ifdef PCL_ANSI_INCLUDE_1ST
#warning Please including Color before Ansi, \
so you can use true color print
#endif
#endif
