#include"../ConScreen.hpp"
using namespace ConScr;
int main()
{
	PCML A;
	TextBox B;
	Node* pB=(Node*)&B;
	A.AddChild(pB);
	B.Debug();
	A.Flush();
	return 0;
}