#pragma once
#define PCL_TUI_OS

#include<print>

#ifdef _WIN32

#include<windef.h>
#include<winbase.h>
#include<wingdi.h>
#include<wincon.h>

bool InitTerminal()
{
	HANDLE hInput=GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode=0;
	GetConsoleMode(hInput,&dwMode);
	dwMode|=ENABLE_MOUSE_INPUT|ENABLE_VIRTUAL_TERMINAL_INPUT;
	dwMode&=~(ENABLE_ECHO_INPUT|ENABLE_LINE_INPUT);
	GetConsoleMode(hOutput,&dwMode);
	dwMode|=ENABLE_PROCESSED_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOutput,dwMode);
	std::print("\e[?1003h\e[?1015h\e[?1006h");
}

/// @brief Get how big the term is 
int GetTerminalWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if(GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),&info))
		return info.dwSize.X;
	return 80;
}

#elif defined(__linux__)||defined(__APPLE__)

#include<sys/ioctl.h>
#include<unistd.h>

/// @brief Get how big the term is 
int GetTerminalWidth()
{
	struct winsize w;
	if(ioctl(STDOUT_FILENO,TIOCGWINSZ,&w)!=-1)
		return w.ws_col;
	return 80;
}

#endif