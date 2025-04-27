#pragma once
#define PCL_FILE

#include<cstdio>
#include<ctime>

#ifdef __cpp_lib_format
#include<format>
#endif

#define pcFL_MAX_PATH_LEN 1024

#define INSERTWRITE "r+"
#define OVERWRITE "w+"
#define ADDWRITE "a+"
#define READONLY "r"
#define OVERWRITEONLY "w"
#define ADDWRITEONLY "a"

namespace pcpri
{
	char LogStartFormat[1010]=
		"New log started in %04d/%02d/%02d %02d:%02d:%02d\n";
	char LogFormat[1010]=
		"[%04d/%02d/%02d %02d:%02d:%02d][%s]";
}

namespace pc
{

/// @brief File operation class
class File
{
protected:
	FILE* pointer;
	char FilePath[pcFL_MAX_PATH_LEN];
public:
	File(){}
	/** @param Mode Mode in `fopen`.
	 * Here are some macros you can use:
	 * ```
	 * INSERTWRITE
	 * OVERWRITE
	 * ADDWRITE
	 * READONLY
	 * OVERWRITEONLY
	 * ADDWRITEONLY
	 * ```
	 */
	File(const char* Mode,const char* Path)
		{pointer=fopen(Path,Mode);}
	~File(){fclose(pointer);}
	/** @param Mode Mode in `fopen`.
	 * Here are some macros you can use:
	 * ```
	 * INSERTWRITE
	 * OVERWRITE
	 * ADDWRITE
	 * READONLY
	 * OVERWRITEONLY
	 * ADDWRITEONLY
	 * ```
	 * @param Path Path to file. Formats in sprintf are acceptable.
	 */
	template<typename... Tps>
	void open(const char* Mode,const char* Path,Tps... args)
	{
		if(pointer!=NULL)
			fflush(pointer),
			fclose(pointer);
		sprintf(FilePath,Path,args...);
		pointer=fopen(FilePath,Mode);
		return;
	}
	/// @brief Check if no file is opened
	bool null(){return pointer==NULL;}
	/// @brief Check if the cursor is at the end of file 
	bool Eof(){return feof(pointer);}
	/// @brief Print to file 
	template<typename... Tps>
	int printf(const char* format,Tps... args)
	{
		int ret=fprintf(pointer,format,args...);
		fflush(pointer);
		return ret;
	}
	/// @brief Scan from file 
	template<typename... Tps>
	int scanf(const char* format,Tps... args)
	{
		int ret=fscanf(pointer,format,args...);
		fflush(pointer);
		return ret;
	}
	/// @brief get a character from file 
	char getchar(){return fgetc(pointer);}
	/**
	 * @brief Get a word the splitted by space
	 * @param dest [OUT] Destination to store the word
	 */
	void getword(char* dest)
	{
		while(true)
		{
			dest[0]=File::getchar();
			if(Eof()||dest[0]!=' '||dest[0]=='\n')
				break;
		}
		int i=1;
		while(true)
		{
			dest[i]=File::getchar();
			if(Eof()||dest[i]==' '||dest[i]=='\n')
				break;
			i++;
		}
		dest[i]='\0';
		return;
	}
	/**
	 * @brief Get a line
	 * @param dest [OUT] Destination to store the line
	 */
	void getline(char* dest)
	{
		int i=0;
		while(true)
		{
			dest[i]=File::getchar();
			if(Eof()||dest[i]=='\n')
				break;
			i++;
		}
		dest[i]='\0';
		return;
	}
	/// @brief Put char to file
	int putchar(char ch)
	{
		int ret=fputc(ch,pointer);
		fflush(pointer);
		return ret;
	}
#ifdef __cpp_lib_format
	/// @brief Print to file with `std::format`
	template<typename ...Tps>
	void print(std::string fmt,Tps ...args)
	{
		std::string res=std::vformat(fmt,std::make_format_args(args...));
		fwrite(res.c_str(),sizeof char,res.length(),pointer);
		return;
	}
#endif
	/// @brief Flush the file stream
	void flush(){fflush(pointer);return;}
	/// @brief cursor offset by `offset` 
	int CursorOffset(int offset)
		{return fseek(pointer,offset,SEEK_CUR);}
	/**
	 * @brief Seek the cursor
	 * @param origin The base position of the offset
	 * ```
	 * SEEK_SET	begin of the file
	 * SEEK_CUR	where the cursor current is
	 * SEEK_END	end of file
	 * ```
	 */
	int CursorSeek(int x,int origin=SEEK_SET)
		{return fseek(pointer,x,origin);}
	/// @brief Get current cursor position
	long long GetCursorPos()
	{
		long long ret;
		fgetpos(pointer,&ret);
		return ret;
	}
	/// @brief Set currert cursor position
	int SetCursorPos(long long x)
		{return fsetpos(pointer,&x);}
};

/**
 * @brief Automatically add time stamp to output
 * @note The format can be modified in `pcpri::LogStartFormat`
 * and `pcpri::LogFormat`
 */
class Log: public File
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
	/**
	 * @brief Whether the time stamp contains year, month and day
	 * @note `true`	contains
	 * @note `false`	excludes
	 */
	bool LongTime=true;
	Log(){pointer=NULL;}
	/// @param l Whether the time stamp contains year, month and day
	Log(bool l){LongTime=l;}
	Log(const char* Mode,const char* Path)
	{
		if(Mode[0]=='r')
			throw("Can't read a log!");
		pointer=fopen(Path,Mode);
		TimeLoc();
		fprintf(pointer,pcpri::LogStartFormat,Ye,Mo,Da,Ho,Mi,Se);
		fflush(pointer);
	}
	~Log(){fclose(pointer);}
	template<typename... Tps>
	void open(const char* Mode,const char* Path,Tps ...args)
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
	/// @brief Log line print
	template<typename...types>
	void lprintf(const char* LogType,const char* format,types... args)
	{
		TimeLoc();
		if(LongTime)
			fprintf(pointer,pcpri::LogFormat,Ye,Mo,Da,Ho,Mi,Se,LogType);
		else
			fprintf(pointer,pcpri::LogFormat,Ho,Mi,Se,LogType);
		fprintf(pointer,format,args...);
		fputc('\n',pointer);
		fflush(pointer);
		return;
	}
};

};//namespace

#include"../Multinclude.hpp"