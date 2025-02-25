#ifndef PCL_IO
#define PCL_IO

#include<cstdio>
#include<cstring>

#define pcPF_MAX_PRINT 100000
#define pcPF_DOUBLEACC 1e8

namespace pcpri
{
	char buff[pcPF_MAX_PRINT],temp[pcPF_MAX_PRINT];
	template<typename Tp=int>
	Tp abs(Tp a)
	{
		if(a<0)	return -a;
		else return a;
	}
}

int ssprintpc(char* Dest,double Th)
{
	int tlen=0;
	if(Th==0)	pcpri::temp[tlen++]='0';
	else
	{
		if(Th<0)
			Th=pcpri::abs(Th),
			pcpri::temp[tlen++]='-';
		int nTh=Th;
		int dTh=(Th-nTh)*pcPF_DOUBLEACC;
		while(nTh>0)
			pcpri::temp[tlen++]='0'+nTh%10,
			nTh/=10;
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
	return tlen;
}
int ssprintpc(char* Dest,int Th)
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
	return tlen;
}
int ssprintpc(char* Dest,const char* Th)
{
	int tlen=strlen(Th);
	strcpy(Dest,Th);
	return tlen;
}
int ssprintpc(char* Dest,char Th)
{
	Dest[0]=Th;
	return 1;
}

void sprintpc(char* Dest){return;}
template<typename Tp,typename... Tps>
void sprintpc(char *Dest,Tp Th,Tps ...args)
{
	sprintpc(Dest+ssprintpc(Dest,Th),args...);
	return;
}

template<typename... Tps>
void printpc(Tps... args)
{
	sprintpc(pcpri::buff,args...);
	int len=strlen(pcpri::buff);
	for(int i=0;i<len;i++)
		putchar(pcpri::buff[i]);
	return;
}

#endif