#include<ctime>
#include"../Large.hpp"
#include<cstdio>
#include<cstring>
int main()
{
	Large<128> A,B,C;
	C=1;
	printf("%s\n",(C<<95).CStr());
	return 0;
}