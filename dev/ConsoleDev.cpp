#include"../Console.hpp"
int main()
{
	CursorSize(0);
	StartGetMouseInput();
	while(true)
	{
		lkInput.lock();
		ColorPrintf(1,1,ConDefaultColor,"%d",MouseRClick());
		lkInput.unlock();
	}
	return 0;
}