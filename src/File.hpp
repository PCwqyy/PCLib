#pragma once
#define PCL_FILE

#include<cstdio>
#include<ctime>

#define FL_MAX_PATH_LEN 1000

#define INSERTWRITE "r+"
#define OVERWRITE "w+"
#define ADDWRITE "a+"
#define READONLY "r"
#define OVERWRITEONLY "w"
#define ADDWRITEONLY "a"

class FileOp
{
protected:
	FILE* pointer;
	char FilePath[1000];
public:
	FileOp(){}
	FileOp(const char* Path,const char* Mode)
		{pointer=fopen(Path,Mode);}
	~FileOp(){fclose(pointer);}
	template<typename... Tps>
	void open(const char* Path,const char* Mode,Tps... args)
	{
		if(pointer!=NULL)
			fflush(pointer),
			fclose(pointer);
		sprintf(FilePath,Path,args...);
		pointer=fopen(FilePath,Mode);
		return;
	}
	bool null(){return pointer==NULL;}
	bool Eof(){return feof(pointer);}
	template<typename... Tps>
	int printf(const char* format,Tps... args)
	{
		int ret=fprintf(pointer,format,args...);
		fflush(pointer);
		return ret;
	}
	template<typename... Tps>
	int scanf(const char* format,Tps... args)
	{
		int ret=fscanf(pointer,format,args...);
		fflush(pointer);
		return ret;
	}
	char getchar(){return fgetc(pointer);}
	void getword(char* dest)
	{
		while(true)
		{
			dest[0]=FileOp::getchar();
			if(Eof()||dest[0]!=' '||dest[0]=='\n')
				break;
		}
		int i=1;
		while(true)
		{
			dest[i]=FileOp::getchar();
			if(Eof()||dest[i]==' '||dest[i]=='\n')
				break;
			i++;
		}
		dest[i]='\0';
		return;
	}
	void getline(char* dest)
	{
		int i=0;
		while(true)
		{
			dest[i]=FileOp::getchar();
			if(Eof()||dest[i]=='\n')
				break;
			i++;
		}
		dest[i]='\0';
		return;
	}
	int putchar(char ch)
	{
		int ret=fputc(ch,pointer);
		fflush(pointer);
		return ret;
	}
	void flush(){fflush(pointer);return;}
	int CursorMove(int offset)
		{return fseek(pointer,offset,SEEK_CUR);}
	int CursorSeek(int x,int origin=SEEK_SET)
		{return fseek(pointer,x,origin);}
	long long GetCursorPos()
	{
		long long ret;
		fgetpos(pointer,&ret);
		return ret;
	}
	int SetCursorPos(long long x)
		{return fsetpos(pointer,&x);}
};

char LogStartFormat[1010]="New log started in %04d/%02d/%02d %02d:%02d:%02d\n";
char LogFormat[1010]="[%04d/%02d/%02d %02d:%02d:%02d][%s]";

class Log: public FileOp
{
protected:
	time_t Ti;tm* T;
	int Se,Mi,Ho,Da,Mo,Ye;
	void TimeLoc()
	{
		Ti=time(0);
		T=localtime(&Ti);
		Se=T->tm_sec,
		Mi=T->tm_min,
		Ho=T->tm_hour,
		Da=T->tm_mday,
		Mo=T->tm_mon+1,
		Ye=T->tm_year+1900;
		return;
	}
public:
	bool LongTime=true;
	Log(){pointer=NULL;}
	Log(bool l){LongTime=l;}
	Log(const char* Path,const char* Mode)
	{
		if(Mode[0]=='r')
			throw("Can't read a log!");
		pointer=fopen(Path,Mode);
		TimeLoc();
		fprintf(pointer,LogStartFormat,Ye,Mo,Da,Ho,Mi,Se);\
		fflush(pointer);
	}
	~Log(){fclose(pointer);}
	template<typename... Tps>
	void open(const char* Path,const char* Mode,Tps ...args)
	{
		if(Mode[0]=='r')
			throw("Can't read a log!");
		if(pointer!=NULL)
			fflush(pointer),
			fclose(pointer);
		sprintf(FilePath,Path,args...);
		pointer=fopen(FilePath,Mode);
		TimeLoc();
		fprintf(pointer,LogStartFormat,Ye,Mo,Da,Ho,Mi,Se);
		fflush(pointer);
		return;
	}
	template<typename...types>
	void lprintf(const char* LogType,const char* format,types... args)
	{
		TimeLoc();
		if(LongTime)
			fprintf(pointer,LogFormat,Ye,Mo,Da,Ho,Mi,Se,LogType);
		else
			fprintf(pointer,LogFormat,Ho,Mi,Se,LogType);
		fprintf(pointer,format,args...);
		fputc('\n',pointer);
		fflush(pointer);
		return;
	}
};