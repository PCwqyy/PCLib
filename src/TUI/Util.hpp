#pragma once
#define PCL_TUI_UTIL

#include<string>
#include<ctime>
#include<random>
#include<set>
#include<map>
using std::string;
using std::set;
using std::map;

#ifdef _WIN32
#include<windef.h>
#include<winbase.h>
#include<wingdi.h>
#include<wincon.h>
#elif defined(__linux__)||defined(__APPLE__)
#include<sys/ioctl.h>
#include<unistd.h>
#endif

/// @brief Utilitys for TUI
namespace util
{

#ifdef _WIN32
/// @brief Get how big the term is 
int GetTerminalWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if(GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),&info))
		return info.dwSize.X;
	return 80;
}
#elif defined(__linux__)||defined(__APPLE__)
/// @brief Get how big the term is 
int GetTerminalWidth()
{
	struct winsize w;
	if(ioctl(STDOUT_FILENO,TIOCGWINSZ,&w)!=-1)
		return w.ws_col;
	return 80;
}
#endif

/// @brief Generate an UUID 
string GenUUID()
{
	char uuid[37];
    std::mt19937 rng(
		std::random_device{}()^
		static_cast<unsigned>(std::time(nullptr))
	);
    std::uniform_int_distribution<unsigned char> dist(0, 255);
    unsigned char bytes[16];
    for (auto& byte : bytes) byte = dist(rng);
    bytes[6]=(bytes[6] & 0x0F)|0x40; //version 4
    bytes[8]=(bytes[8] & 0x3F)|0x80; //variant 1
    std::snprintf(uuid,sizeof(uuid),
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        bytes[0],bytes[1],bytes[2],bytes[3],
        bytes[4],bytes[5],bytes[6],bytes[7],
        bytes[8],bytes[9],bytes[10],bytes[11],
        bytes[12],bytes[13],bytes[14],bytes[15]);
    return string(uuid);
}

/**
 * @brief Get a word from string
 * @param modify whether delete the word form origin string
 * @return The gotten word
 */
string BreakWord(string& a,bool modify=true)
{
	string ans;
	int i=0,len=a.length();
	while(i<len&&isspace(a[i]))	i++;
	for(;i<len;i++)
		if(isspace(a[i]))	break;
		else	ans+=a[i];
	if(!modify)	return ans;
	while(i<len&&isspace(a[i]))	i++;
	a=a.substr(i);
	return ans;
}
/**
 * @brief Get a name (with only alphabets and numbers) from string
 * @param modify whether delete the word form origin string
 * @return The name
 */
string BreakName(string& a,bool modify=true)
{
	string ans;
	int i=0,len=a.length();
	while(i<len&&!isalnum(a[i]))	i++;
	for(;i<len;i++)
		if(isalnum(a[i]))
			ans+=a[i];
		else break;
	if(!modify)	return ans;
	while(i<len&&!isalnum(a[i]))	i++;
	a=a.substr(i);
	return ans;
}

/// @brief If is a valid var name 
bool CheckNameValid(string name)
{
	bool hasAlp=false;
	for(char i:name)
		if(isalpha(i))
			hasAlp=true;
		else if(isdigit(i));
		else if(i=='_');
		else return false;
	return hasAlp;
}
/// @brief If is a valid var name 
bool CheckTagValid(string name)
{
	for(char i:name)
		if(!isalpha(i))
			return false;
	return true;
}

/// @brief Contianer of `ClassList`
class ClassSet
{
private:
	set<string> c;
public:
	void Add(string arg)
	{
		if(CheckNameValid(arg))
			c.insert(arg);
	}
	template<typename ...Tps>
	void Add(string arg,Tps ...args)
	{
		Add(arg);
		Add(args...);
		return;
	}
	bool Has(string arg) const
	{
		if(!CheckNameValid(arg))	return false;
		return c.find(arg)!=c.end();
	}
	void Delete(string arg)
	{
		if(CheckNameValid(arg))
			c.erase(arg);
	}
	void Toggle(string arg)
	{
		if(!CheckNameValid(arg))	return;
		if(c.find(arg)==c.end())
			c.insert(arg);
		else
			c.erase(arg);
	}
	auto begin() const {return c.begin();}
	auto end() const {return c.end();}
	void Parse(string a)
	{
		string n;
		while(a.length()>0)
			n=BreakWord(a),
			Add(n);
		return;
	}
	ClassSet operator=(string a)
		{Parse(a);return *this;}
	ClassSet(){}
	ClassSet(string a){Parse(a);}
	ClassSet(const char* a){Parse(string(a));}
};

class ID: public string
{
public:
	ID operator=(string a)
	{
		if(CheckNameValid(a))
			return string::operator=(a);
		return *this;
	}
	ID(string a):string(a)
	{
		if(!CheckNameValid(a))
			string::operator=("");
	}
	ID():string(""){}
};

/// @brief Container of `Attribute`
class AttributeMap
{
private:
	map<string,string> m;
public:
	bool Has(string key)
		{return m.find(key)!=m.end();}
	void Set(string key,string val)
		{m[key]=val;}
	string Get(string key)
	{
		auto i=m.find(key);
		if(i==m.end())	return "undefined";
		else return i->second;
	}
	auto operator[](string key)
		{return m[key];}
};

}//namespace