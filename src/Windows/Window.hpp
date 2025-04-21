#pragma once
#define PCL_WINDOWS

#include<cstdio>
#include<cwchar>
#include<windows.h>
#include<tchar.h>

#define UNICODE
#define _UNICODE

extern "C"{

// @brief Default Proc.
LRESULT CALLBACK PCDefProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0;
}
/**
 * @brief To create a window.
 * @param ClassName String name of window.
 * @param Title Title of window.
 * @param Type Window Type. Learn More: https://learn.microsoft.com/windows/win32/winmsg/window-styles
 * @param X X location to the parent window or desktop (if there are no parent).You can use CW_USEDEFAULT.
 * @param Y similar to X. You can use CW_USEDEFAULT.
 * @param Parent HWND of parent window or NULL (if there are no parent).
 * @param Menu HWND of menu or NULL (if there are no parent).
*/
HWND PCCreateWindow(HINSTANCE hinstance,LPCWSTR ClassName,LPCWSTR Title,DWORD Type,int X,int Y,int Width,int Height,HWND Parent,HMENU Menu,WNDPROC Proc=PCDefProc)
{
	WNDCLASSEXW win;
	win.cbSize			=sizeof(WNDCLASSEXW);
	win.style			=0;
	win.lpfnWndProc		=Proc;
	win.cbClsExtra		=0;
	win.cbWndExtra		=0;
	win.hInstance		=hinstance;
	win.hIcon			=LoadIcon(NULL,IDI_APPLICATION);
	win.hCursor			=LoadCursor(NULL,IDC_ARROW);
	win.hbrBackground	=(HBRUSH)(COLOR_WINDOW+1);
	win.lpszMenuName	=NULL;
	win.lpszClassName	=ClassName;
	win.hIconSm			=LoadIcon(NULL,IDI_APPLICATION);
	if(!RegisterClassEx(&win))
	{
		MessageBox(NULL,L"Window Registration Failed!",L"Error!",MB_ICONEXCLAMATION|MB_OK);
		return NULL;
	}
	wprintf(L"t:%s\n",Title);
	return CreateWindowW(ClassName,Title,Type,X,Y,Width,Height,Parent,Menu,hinstance,NULL);
}
/*
HWND PCCreateWindow(HINSTANCE hinstance,LPCWSTR ClassName, LPCWSTR Title, DWORD Type, int X, int Y, int Width, int Height, HWND Parent, HMENU Menu, WNDPROC Proc = PCDefProc) {
    WNDCLASSEXW win;
    win.cbSize = sizeof(WNDCLASSEX);
    win.style = 0;
    win.lpfnWndProc = Proc;
    win.cbClsExtra = 0;
    win.cbWndExtra = 0;
    win.hInstance = hinstance;
    win.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win.hCursor = LoadCursor(NULL, IDC_ARROW);
    win.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    win.lpszMenuName = NULL;
    win.lpszClassName = ClassName;
    win.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&win)) {
        MessageBoxW(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    // 直接将LPCWSTR类型赋值给LPCWSTR类型，无需转换
    LPCWSTR title = Title;

    // 创建窗口时，将LPCWSTR类型的Title参数使用
    return CreateWindowW(ClassName, title, Type, X, Y, Width, Height, Parent, Menu, hinstance, NULL);
}
*/

void PCRunWindow(HWND Window,int nCmdShow)
{
	ShowWindow(Window,nCmdShow);
	UpdateWindow(Window);
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0)>0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

/**
 * @brief Create a front to use.
 * @param Fs Font size (0 means default).
 * @param ltA Left turn angle (unit: 0.1°).
 * @param Bold The extent of the font boldness (1~1000, 0 means default).
*/
HFONT PCCreateFont(HWND hwnd,LPCWSTR Name,int Fs,int ltA,int Bold,DWORD Italic,DWORD Underline,DWORD StrikeOut)
{
	HDC hdc=GetDC(hwnd);
	HFONT hf=CreateFont(Fs,0,0,ltA,Bold,Italic,Underline,StrikeOut,0,0,0,0,0,Name);
	if(!hf)	MessageBox(NULL,L"Font creation failed!",L"Error",MB_OK|MB_ICONEXCLAMATION);
	ReleaseDC(hwnd,hdc);
	return hf;
}

/**
 * @brief To paint texts.
 * @param hwnd Target window's hwnd.
*/
void PCDrawText(HWND hwnd,int X,int Y,HFONT Font,int col,LPCWSTR Text)
{
	HDC hdc=GetDC(hwnd);
	SelectObject(hdc,Font);
	SetTextColor(hdc,col);
	TextOut(hdc,X,Y,Text,wcslen(Text));
	ReleaseDC(hwnd,hdc);
}

}//extern

#include"../Multinclude.hpp"