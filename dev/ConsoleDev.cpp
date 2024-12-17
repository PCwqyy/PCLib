#include"../src/Console.hpp"
int main()
{
	// CursorSize(0);
	SetConsoleOutputCP(CP_GBK);
	ConTitleA("Test Window");
	ConSize(20,5);
	while(true)
	{
		int fore,back;
		scanf("%x %x",&fore,&back);
		ColorPrintfEx(fore,back,"AAAAA\n");
	}
	return 0;
}