#pragma once
#define PCL_FILE

#include<cstdio>
#include<ctime>
#include<string>
#include<format>
#include<filesystem>
#include<fstream>
#include<functional>
#include<array>
#include<cstddef>
using std::string;
using std::string_view;

#include"../Exception.hpp"
#include"StrUtils.hpp"
#include"EnumLookup.hpp"

namespace fs=std::filesystem;

namespace pc{
namespace File{

const pc::Exceptioner xptFile("File");

enum class Mode{
	Insert,Overwrite,Attach,Binary,
	ReadOnly,OverwriteOnly,AttachOnly
};
EnumLookup<Mode,std::ios::openmode> ModeLookup{
	Mode::Insert,		std::ios::in|std::ios::out,
	Mode::Overwrite,	std::ios::in|std::ios::out|std::ios::trunc,
	Mode::Attach,		std::ios::in|std::ios::out|std::ios::app,
	Mode::Binary,		std::ios::in|std::ios::out|std::ios::binary,
	Mode::ReadOnly,		std::ios::in,
	Mode::OverwriteOnly,std::ios::out|std::ios::trunc,
	Mode::AttachOnly,	std::ios::out|std::ios::app
};

class File
{
protected:
	std::fstream stream;
	fs::path path;
	std::ios::openmode openmode;
	/// @brief guard unexpected read
	void tryRead(){
		if(!HasMode(std::ios::in))
			xptFile.Throw(pcXPT_ACCESS_DENIED,
				"Reading file '{}' which was opened in unreadable mode",
				path.string()
			);
	}
	/// @brief guard unexpected write
	void tryWrite(){
		if(!HasMode(std::ios::out))
			xptFile.Throw(pcXPT_ACCESS_DENIED,
				"Writing to file '{}' which was opened in readonly mode",
				path.string()
			);
	}
public:
	/// @brief Open a file
	virtual void Open(Mode mode,fs::path filePath)
	{
		std::ios::openmode openmode=ModeLookup(mode);
		if(openmode&std::ios::in&&!fs::exists(filePath))
			xptFile.Throw(pcXPT_FILE_NOT_FOUND,
				"No such a file '{}'",filePath.string());
		stream.open(filePath,openmode);
		if(!stream.is_open())
			xptFile.Throw(pcXPT_FILE,
				"Fail to open file '{}' (errno: {})",
				filePath.string(),std::to_string(errno)
			);
	}
	/// @brief Close the file
	void Close(){
		if(!IsOpen())	return;
		stream.flush();
		stream.close();
	}
	/// @brief Default constructor with no file opened
	File(){}
	/// @brief Open a file with specified mode and path
	File(Mode mode,fs::path filePath){
		Open(mode,filePath);
	}
	/**
	 * @brief Open a file with specified mode and path
	 * @param fmt The format of the path, can be used with `std::format`
	 * @param args The arguments for the format
	 */
	template<typename ...Tps>
	File(Mode mode,string_view fmt,Tps...args){
		fs::path filePath;
		string str;
		try{
			str=std::vformat(fmt,std::make_format_args(args...));
			filePath=str;
		}catch(...){
			xptFile.Throw(pcXPT_INVALID_ARGUMENT,
				"Invalid file path '{}'",str);
		}
		Open(mode,filePath);
	}
	~File(){Close();}
	/// @brief Check if the file is opened 
	bool IsOpen(){return stream.is_open();}
	/// @brief Check if reached end of file 
	bool Eof(){return stream.eof();}
	/// @brief Sync with native device
	void Flush(){stream.flush();}
	/// @brief check if the file opened in this mode
	bool HasMode(std::ios::openmode m){return m&openmode;}
};

/// @brief Text file operation class
class Text: public File
{
public:
	using File::File;
	/// @brief get a character from file 
	char GetChar(){
		tryRead();
		return stream.get();
	}
	/// @brief ignore chars in the stream
	void Ignore(su::CharType shouldIgnore){
		tryRead();
		if(Eof())	return;
		while(shouldIgnore(stream.peek()))
			stream.get();
	}
	/// @brief ignore chars in the stream
	inline void Ignore(std::string_view chars){
		Ignore(su::Delimiters::List(chars));
	}
	/**
	 * @brief Get a token breaking with delimiter from file
	 * @param isDelimiter Decide whether the char is a delimiter
	 * @return the gotten token
	 * @note More `isDelimiter` decleration are in `su::Delimiters`
	 */
	string GetToken(su::CharType isDelimiter)
	{
		tryRead();
		string ans;
		Ignore(isDelimiter);
		while(true){
			if(Eof())	break;
			char in=stream.get();
			if(isDelimiter(in))	break;
			else	ans+=in;
		}
		return ans;
	}
	/// @brief Get a line
	inline string GetWord(){
		return GetToken(su::Delimiters::Word);
	}
	/// @brief Get a line
	inline string GetLine(){
		return GetToken(su::Delimiters::Line);
	}
	/// @brief Put char to file
	void PutChar(char ch){
		tryWrite();
		stream.put(ch);
	}
	/// @brief Print to file with `std::format`
	template<typename ...Tps>
	void Print(std::string fmt,Tps ...args)
	{
		std::string res=std::vformat(fmt,std::make_format_args(args...));
		stream<<res;
		return;
	}
private:
	/// @brief Basic seek function
	inline void seekReadBase(int offset,std::ios_base::seekdir mode){
		tryRead();
		stream.seekg(offset,mode);
	}
	inline void seekWriteBase(int offset,std::ios_base::seekdir mode){
		tryWrite();
		stream.seekp(offset,mode);
	}
public:
	/// @brief read cursor offset by `x`
	inline void SeekReadCur(int x)
		{seekReadBase(x,std::ios_base::cur);}
	/// @brief read cursor goto position `x`
	inline void SeekReadHome(int x)
		{seekReadBase(std::abs(x),std::ios_base::cur);}
	/// @brief read cursor goto `x` count from end
	inline void SeekReadEnd(int x)
		{seekReadBase(-std::abs(x),std::ios_base::end);}
	/// @brief write cursor offset by `x`
	inline void SeekWriteCur(int x)
		{seekWriteBase(x,std::ios_base::cur);}
	/// @brief write cursor goto position `x`
	inline void SeekWriteHome(int x)
		{seekWriteBase(std::abs(x),std::ios_base::cur);}
	/// @brief write cursor goto `x` count from end
	inline void SeekWriteEnd(int x)
		{seekWriteBase(-std::abs(x),std::ios_base::end);}
	/// @brief Get current read cursor position
	inline int GetReadPos(){return stream.tellg();}
	/// @brief Get current read cursor position
	inline int GetWritePos(){return stream.tellp();}
	/// @brief Get file content as string 
	std::string ReadAll()
	{
		tryRead();
		std::string ans;
		char ch;
		while(true){
			ch=stream.get();
			if(Eof())	break;
			ans+=ch;
		}
		return ans;
	}
	/// @brief Write string to file
	void WriteAll(std::string str){stream<<str;}
};

/**
 * @brief Automatically add time stamp to output
 * @note The format can be modified in `pcpri::LogStartFormat`
 * and `pcpri::LogFormat`
 */
class Log: protected Text
{
protected:
	time_t Ti;tm* T;
	int Se,Mi,Ho,Da,Mo,Ye,We;
	void timeLoc()
	{
		Ti=time(0);
		T=localtime(&Ti);
		Se=T->tm_sec,
		Mi=T->tm_min,
		Ho=T->tm_hour,
		We=T->tm_wday,
		Da=T->tm_mday,
		Mo=T->tm_mon+1,
		Ye=T->tm_year+1900;
		return;
	}
public:
	enum class Levels{Trace,Debug,Info,Warning,Error,Fatal};
protected:
	inline static const EnumLookup<Levels,std::string> LevelLookup{
		Levels::Trace,	"Trace",
		Levels::Debug,	"Debug",
		Levels::Info,	"Info",
		Levels::Warning,	"Warning",
		Levels::Error,	"Error",
		Levels::Fatal,	"Fatal"
	};
	Levels level=Levels::Info;
public:
	/** @note index of the args are:
	 * ```
	 * 0 Year    | 1 Month  | 2 Day
	 * 3 Hour    | 4 Minute | 5 Second
	 * 6 WeekDay
	 * ```
	 */
	string HeadFormat="New log started in {0:04}/{1:02}/{2:02} {3:02}:{4:02}:{5:02}";
	/** @note index of the args are:
	 * ```
	 * 0 Year    | 1 Month  | 2 Day
	 * 3 Hour    | 4 Minute | 5 Second
	 * 6 WeekDay | 7 Type
	 * ```
	 */
	string LineFormat="{0:04}/{1:02}/{2:02} {3:02}:{4:02}:{5:02}[{7}]";
	using Text::Text;
	/// @brief Open a log
	void Open(Mode mode,fs::path filePath)
	{
		if(mode!=Mode::OverwriteOnly&&mode!=Mode::AttachOnly)
			xptFile.Throw(pcXPT_INVALID_ARGUMENT,
				"Log file can only be opened in attach only or overwrite only mode");
		File::Open(mode,filePath);
		timeLoc();
		string head=std::vformat(HeadFormat,std::make_format_args(Ye,Mo,Da,Ho,Mi,Se));
		stream<<head;
		Flush();
	}
	void Close(){File::Close();}
	bool IsOpen(){return File::IsOpen();}
	void Flush(){File::Flush();}
	~Log(){Close();}
	/// @brief Log line print
	template<typename...types>
	void PrintLn(Levels logLevel,string format,types... args){
		if(logLevel<level)	return;
		timeLoc();
		Text::Print(LineFormat,Ye,Mo,Da,Ho,Mi,Se,We,LevelLookup(logLevel));
		Text::Print(format,...args);
		Text::PutChar('\n');
	}
	template<typename...types>
	inline void Trace(string format,types... args)
		{PrintLn(Levels::Trace,format,...args);}
	template<typename...types>
	inline void Debug(string format,types... args)
		{PrintLn(Levels::Debug,format,...args);}
	template<typename...types>
	inline void Info(string format,types... args)
		{PrintLn(Levels::Info,format,...args);}
	template<typename...types>
	inline void Warning(string format,types... args)
		{PrintLn(Levels::Warning,format,...args);}
	template<typename...types>
	inline void Error(string format,types... args)
		{PrintLn(Levels::Error,format,...args);}
	template<typename...types>
	inline void Fatal(string format,types... args)
		{PrintLn(Levels::Fatal,format,...args);}
	void SetLevel(Levels logLevel){level=logLevel;}
	Levels GetLevel(){return level;}
	string GetLevelString(){return LevelLookup(level);}
};

class Binary: public File
{
public:
	using File::File;
	typedef std::vector<std::byte> Bytes;
	void Open(const Mode mode,fs::path filePath){
		if(mode!=Mode::Binary)
			xptFile.Throw(pcXPT_INVALID_ARGUMENT,
				"the mode of bin file must be Mode::Binary");
		File::Open(mode,filePath);
	}
	void Open(fs::path filePath){
		Open(Mode::Binary,filePath);
	}
	/**
	 * @brief Directly dump the memory of the data in to file.
	 * @warning The structure of data must be trivially copyable,
	 * or undefined actions will occured
	 */
	template<typename Tp>
	void Dump(const Tp& data){
		tryWrite();
		stream.write(reinterpret_cast<char*>(&data),sizeof(data));
	}
	/// @brief Write bytes
	void Write(Bytes bytes){
		tryWrite();
		stream.write(reinterpret_cast<char*>(bytes.data()),bytes.size());
	}
	/// @brief Call serializer and write bytes
	template<typename Tp>
	void Write(Tp data,std::function<Bytes(Tp)> serializer){
		static_assert(std::is_trivially_copyable_v<Tp>,
        	"Tp must be trivially copyable for Dump");
		Write(serializer(data));
	}
	/// @brief Directly load memory from file
	template<typename Tp>
	Tp Load()
	{
		static_assert(std::is_trivially_copyable_v<Tp>,
        	"Tp must be trivially copyable for Load");
		tryRead();
		Tp data;
		stream.read(reinterpret_cast<char*>(&data),sizeof(data));
		if(stream.gcount()!=sizeof(data))
			xptFile.Throw(pcXPT_FILE,
				"Fail to load bin from file {}",path);
		return data;
	}
	Bytes Read(unsigned int count)
	{
		tryRead();
		Bytes bytes;
		bytes.resize(count);
		stream.read(reinterpret_cast<char*>(bytes.data()),count);
		if(stream.gcount()!=count)
			xptFile.Throw(pcXPT_FILE,
				"Fail to read bin from file {}",path);
		return bytes;
	}
	template<typename Tp>
	Tp Read(std::function<Tp(std::fstream&)> deserializer){
		return deserializer(stream);
	}
};

};//namespace File
};//namespace pc