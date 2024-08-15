#include<ctime>
#include"../Large.hpp"
#include<cstdio>
#include<cstring>
int main()
{
	Large<128> A,B,C;
	A="21474836472147483647";
	B="10";
	printf("[s]<%d>\n",clock());
	C=A/B;
	printf("%s\n",C.CStr());
	printf("[e]<%d>\n",clock());
	return 0;
}