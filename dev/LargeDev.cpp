#include"../Large.hpp"
#include<cstdio>
#include<cstring>
int main()
{
	Large<128> A,B,C;
	A="21474836472147483647";
	B="10000000000000000000";
	C=A+B;
	printf("%s",C.CStr());
	return 0;
}