#include"../Console.hpp"
#include"../Log.hpp"
int main()
{
	StartGetMousexy();
	COORD Last={0,0};
	while(true)
	{
		ColorPrintf(pcpri::Mouse.X,pcpri::Mouse.Y,0x70," ");
		ColorPrintf(Last.X,Last.Y,0x00," ");
		Last=pcpri::Mouse;
	}
	return 0;
}