#include"../Console.hpp"
int main()
{
	CursorSize(0);
	SetConsoleOutputCP(CP_GBK);
	ConTitleA("Test Window");
	ConSize(20,5);
	printf("永abcABC123");
	StartGetMouseInput();
	while(true)
	{
		ColorPosPrintf(0x07,1,1,"%d %d\t\t\t\t",GetMousexy().X,GetMousexy().Y);
	}
	return 0;
}