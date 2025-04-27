#pragma once
#define PCL_IO

#ifndef _INC_STDIO
#warning Please include stdio!
#endif

#include<cstdio>
#include<cstring>
#include<cctype>

#define pcPF_MAX_PRINT 100000
#ifndef pcB_DOUBLEACC
#define pcB_DOUBLEACC 1e8
#endif

namespace pcpri
{
	char buff[pcPF_MAX_PRINT],temp[pcPF_MAX_PRINT];
	template<typename Tp=int>
	Tp abs(Tp a)
	{
		if(a<0)	return -a;
		else return a;
	}
#ifdef _GLIBCXX_ANY
	template<typename Tp>
	struct any_scanner
	{
		std::any* tar;
		Tp temp;
		void GiveBack()
			{*tar=std::forward<Tp>(temp);}
	};
#endif
}

namespace pc
{

int sscanner(const char* Src,int& Th)
{
	int now=0;
	bool neg=false;
	Th=0;
	while(isspace(Src[now]))
		now++;
	if(Src[now]=='-')
		neg=true,now++;
	while(Src[now]>='0'&&Src[now]<='9')
		Th*=10,Th+=Src[now++]-'0';
	if(neg)
		Th=-Th;
	return now;
}
int sscanner(const char* Src,long long& Th)
{
	int now=0;
	bool neg=false;
	Th=0;
	while(isspace(Src[now]))
		now++;
	if(Src[now]=='-')
		neg=true,now++;
	while(Src[now]>='0'&&Src[now]<='9')
		Th*=10,Th+=Src[now++]-'0';
	if(neg)
		Th=-Th;
	return now;
}
int sscanner(const char* Src,double& Th)
{
	int now=0,iFing=0,fFing=0;
	long long iTh=0,fTh=0;
	bool neg=false;
	while(isspace(Src[now]))
		now++;
	if(Src[now]=='-')
		neg=true,now++;
	while(Src[now]>='0'&&Src[now]<='9')
		iTh*=10,iTh+=Src[now++]-'0';
	if(Src[now]=='.')
	{
		now++;
		while(Src[now]>='0'&&Src[now]<='9')
			fTh*=10,fTh+=Src[now++]-'0',fFing++;
	}
	Th=fTh;
	while(fFing--)	Th/=10;
	Th+=iTh;
	if(Src[now]=='E'||Src[now]=='e')
		now+=sscanner(Src+(++now),iFing);
	while(iFing--)	Th*=10;
	if(neg)	Th=-Th;
	return now;
}
int sscanner(const char* Src,char& Th)
{
	Th=Src[0];
	return 1;
}
int sscanner(const char* Src,char* Th)
{
	int now=0,i=0;
	while(Src[now]!='\0'&&isspace(Src[now]))
		now++;
	while(Src[now]!='\0'&&!isspace(Src[now]))
		Th[i++]=Src[now++];
	return now;
}

int sprinter(char* Dest,double Th)
{
	int tlen=0;
	if(Th==0)	pcpri::temp[tlen++]='0';
	else
	{
		if(Th<0)
			Th=pcpri::abs(Th),
			pcpri::temp[tlen++]='-';
		int nTh=Th;
		int dTh=(Th-nTh+0.1/pcB_DOUBLEACC)*pcB_DOUBLEACC;
		while(nTh>0)
			pcpri::temp[tlen++]='0'+nTh%10,
			nTh/=10;
		if(tlen==0)
			pcpri::temp[tlen++]='0';
		pcpri::temp[tlen]='\0';
		strrev(pcpri::temp);
		pcpri::temp[tlen++]='.';
		int dlen=tlen;
		while(dTh>0)
			pcpri::temp[tlen++]='0'+dTh%10,
			dTh/=10;
		pcpri::temp[tlen]='\0';
		strrev(pcpri::temp+dlen);
		while(pcpri::temp[--tlen]=='0');
		tlen++;
	}
	pcpri::temp[tlen]='\0';
	strcpy(Dest,pcpri::temp);
	Dest[tlen]='\0';
	return tlen;
}
int sprinter(char* Dest,int Th)
{
	int tlen=0;
	if(Th==0)	pcpri::temp[tlen++]='0';
	else
	{
		if(Th<0)
			Th=pcpri::abs(Th),
			pcpri::temp[tlen++]='-';
		while(Th>0)
			pcpri::temp[tlen++]='0'+Th%10,
			Th/=10;
	}
	pcpri::temp[tlen]='\0';
	strrev(pcpri::temp);
	pcpri::temp[tlen]='\0';
	strcpy(Dest,pcpri::temp);
	Dest[tlen]='\0';
	return tlen;
}
int sprinter(char* Dest,long long Th)
{
	int tlen=0;
	if(Th==0)	pcpri::temp[tlen++]='0';
	else
	{
		if(Th<0)
			Th=pcpri::abs(Th),
			pcpri::temp[tlen++]='-';
		while(Th>0)
			pcpri::temp[tlen++]='0'+Th%10,
			Th/=10;
	}
	pcpri::temp[tlen]='\0';
	strrev(pcpri::temp);
	pcpri::temp[tlen]='\0';
	strcpy(Dest,pcpri::temp);
	Dest[tlen]='\0';
	return tlen;
}
int sprinter(char* Dest,const char* Th)
{
	int tlen=strlen(Th);
	strcpy(Dest,Th);
	Dest[tlen]='\0';
	return tlen;
}
int sprinter(char* Dest,char Th)
{
	Dest[0]=Th;
	Dest[1]='\0';
	return 1;
}

#ifdef _GLIBCXX_STRING
int sscanner(const char* Src,std::string& Th)
{
	int ret=pc::sscanner(Src,pcpri::temp);
	Th=pcpri::temp;
	return ret;
}
int sprinter(char* Dest,std::string Th)
{
	return sprinter(Dest,Th.c_str());
}
#endif

#ifdef _GLIBCXX_ANY
/// @brief Get a input handle to use in `pc::scan()`
template<typename Tp>
pcpri::any_scanner<Tp>& ScanAnyIn(std::any& Dest)
{
	pcpri::any_scanner<Tp>* a=new pcpri::any_scanner<Tp>;
	a->tar=&Dest;
	return *a;
}
template<typename Tp>
int sscanner(const char* Src,pcpri::any_scanner<Tp> Th)
{
	int ret=pc::sscanner(Src,Th.temp);
	Th.GiveBack();
	return ret;
}
#endif

/// @brief Scan from string
void sscan(const char* Src){return;}
template<typename Tp,typename... Tps>
/// @brief Scan form string
void sscan(const char* Src,Tp& Th,Tps& ...args)
{
	sscan(Src+pc::sscanner(Src,Th),args...);
	return;
}
/**
 * @brief Scan from console
 * @note ##### How to make a custom class compatible with this function
 * @note Define a `pc::sscanner()` function like this:
 * ```cpp
 * int sscanner(const char* Src,YourClass& Th)
 * {// Scan from Src and save to Th
 * 	// Do scan logics...
 * 	// Do scan logics...
 * 	return CursorPosition;
 * 	// remember to return how many characters you've read
 * }
 * ```
 * Don't forget to put it under `namespace pc`!
 */
template<typename... Tps>
void scan(Tps& ...args)
{
	int now=0;
	while(true)
	{
		pcpri::buff[now]=getchar();
		if(pcpri::buff[now]=='\n')
			if(!now==0)	break;
		now++;
	}
	pcpri::buff[now]='\0';
	sscan(pcpri::buff,args...);
}

/// @brief Print to string
void sprint(char* Dest){return;}
/// @brief Print to string
template<typename Tp,typename... Tps>
void sprint(char *Dest,Tp Th,Tps ...args)
{
	sprint(Dest+pc::sprinter(Dest,Th),args...);
	return;
}
/**
 * @brief Print to console
 * @note ##### How to make a custom class compatible with this function
 * @note Define a `pc::sprinter()` function like this:
 * ```cpp
 * int sprinter(char* Dest,YourClass Th)
 * {// Print Th to Dest
 * 	// Do print logics...
 * 	// Do print logics...
 * 	return CursorPosition;
 * 	// remember to return how many characters you've put
 * }
 * ```
 * Don't forget to put it under `namespace pc`!
 */
template<typename... Tps>
void print(Tps... args)
{
	sprint(pcpri::buff,args...);
	int len=strlen(pcpri::buff);
	for(int i=0;i<len;i++)
		putchar(pcpri::buff[i]);
	return;
}

}//namespace

#include"../Multinclude.hpp"