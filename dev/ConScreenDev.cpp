#include"../src/ConScreen.hpp"
using namespace ConScr;
int main()
{
	SetConsoleOutputCP(437);
	PCML A;
	A.SetSize((COORD){80,25});
	TextBox B;
	Node* pB=(Node*)&B;
	A.AddChild(pB);
	B.Debug();
	while(true)
		A.Flush();
	return 0;
}