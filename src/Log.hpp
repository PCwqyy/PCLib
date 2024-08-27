#ifndef PCL_LOG
#define PCL_LOG

#include<cstdio>
#include<ctime>

int _UntitleCount=0;
char LogStartFromat[1010]="New log started in %04d/%02d/%02d %02d:%02d:%02d\n";
char LogFromat[1010]="[%04d/%02d/%02d %02d:%02d:%02d][%s]%s\n";

#define OVERWRITE "w+"
#define ADDWRITE "a+"

template<int maxl>
class Log
{
	protected:
		FILE* Lout;
		time_t Ti;tm* T;
		char LogOut[maxl+20];
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
		void OpenFile(const char* filename,const char* mode)
		{
			fclose(Lout);
			Lout=fopen(filename,mode);
			TimeLoc();
			fprintf(Lout,LogStartFromat,Ye,Mo,Da,Ho,Mi,Se);
			fflush(Lout);
			return;
		}
		Log(const char* filename,const char* mode)
		{
			Lout=fopen(filename,mode);
			TimeLoc();
			fprintf(Lout,LogStartFromat,Ye,Mo,Da,Ho,Mi,Se);
			fflush(Lout);
		}
		~Log(){fclose(Lout);}
		template<typename...types>
		void lprintf(char* LogType,char* format,types... args)
		{
			sprintf(LogOut,format,args...);
			TimeLoc();
			fprintf(Lout,LogFromat,Ye,Mo,Da,Ho,Mi,Se,LogType,LogOut);
			fflush(Lout);
			return;
		}
		template<typename...types>
		int aprintf(char* format,types... args)
			{return fprintf(Lout,format,args...);}
		void flush(){fflush(Lout);return;}
};

#ifdef _GLIBCXX_QUEUE
using std::queue;
template<int maxl>
class QueueLog:public Log<maxl>
{
	using Log<maxl>::Lout;
	using Log<maxl>::TimeLoc;
	using Log<maxl>::LogOut;
	using Log<maxl>::Ye;
	using Log<maxl>::Mo;
	using Log<maxl>::Da;
	using Log<maxl>::Ho;
	using Log<maxl>::Mi;
	using Log<maxl>::Se;
	using Log<maxl>::Log;
	private:
		queue<char*> Buf;
		char* ThisOut;
	public:
		template<typename... types>
		void qprintf(char* LogType,char* format,types... args)
		{
			sprintf(LogOut,format,args...);
			TimeLoc();
			ThisOut=new char[maxl+20];
			sprintf(ThisOut,LogFromat,Ye,Mo,Da,Ho,Mi,Se,LogType,LogOut);
			Buf.push(ThisOut);
			return;
		}
		void flush()
		{
			while(!Buf.empty())
			{
				ThisOut=Buf.front();
				fprintf(Lout,"%s",ThisOut);
				Buf.pop();
			}
			fflush(Lout);
		}
	
};
#endif

#endif