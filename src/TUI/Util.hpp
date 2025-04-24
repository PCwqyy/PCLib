#pragma once
#define PCL_TUI_UTIL

#ifdef _WIN32
#include<winbase.h>
#include<wincon.h>
#elif defined(__linux__)||defined(__APPLE__)
#include<sys/ioctl.h>
#include<unistd.h>
#endif

namespace util
{

#ifdef _WIN32
int GetTerminalWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if(GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),&info))
		return info.dwSize.X;
	return 80;
}
#elif defined(__linux__)||defined(__APPLE__)
int GetTerminalWidth() {
	struct winsize w;
	if(ioctl(STDOUT_FILENO,TIOCGWINSZ,&w)!=-1)
		return w.ws_col;
	return 80;
}
#endif

}//namespace