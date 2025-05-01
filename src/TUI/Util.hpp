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

#include"../Exception.hpp"

// #ifdef _WIN32
// #include<windef.h>
// #include<winbase.h>
// #include<wingdi.h>
// #include<wincon.h>
// #elif defined(__linux__)||defined(__APPLE__)
// #include<sys/ioctl.h>
// #include<unistd.h>
// #endif

/// @brief Utilitys for TUI
namespace util
{

// #ifdef _WIN32
// /// @brief Get how big the term is 
// int GetTerminalWidth()
// {
// 	CONSOLE_SCREEN_BUFFER_INFO info;
// 	if(GetConsoleScreenBufferInfo(
// 		GetStdHandle(STD_OUTPUT_HANDLE),&info))
// 		return info.dwSize.X;
// 	return 80;
// }
// #elif defined(__linux__)||defined(__APPLE__)
// /// @brief Get how big the term is 
// int GetTerminalWidth()
// {
// 	struct winsize w;
// 	if(ioctl(STDOUT_FILENO,TIOCGWINSZ,&w)!=-1)
// 		return w.ws_col;
// 	return 80;
// }
// #endif

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

void ShrinkStringHead(string&a)
{
	int len=a.length(),i=0;
	while(i<len&&isspace(a[i]))	i++;
	a=a.substr(i);
}
/**
 * @brief Get a word breaking with `breaker()` from string
 * @param modify Whether delete the word form origin string
 * @param breaker Determine whether the char is a breaker
 * @return The gotten word
 */
string BreakString(string& a,bool (*breaker)(char a),bool modify=true)
{
	string ans;
	int i=0,len=a.length();
	while(i<len&&breaker(a[i]))	i++;
	for(;i<len;i++)
		if(breaker(a[i]))	break;
		else	ans+=a[i];
	if(!modify)	return ans;
	a=a.substr(i);
	return ans;
}
string BreakWord(string& a,bool modify=true)
{
	return BreakString(a,
		[](char a){return bool(isspace(a));},
		modify);
}
string BreakName(string& a,bool modify=true)
{
	return BreakString(a,
		[](char a){return !isalnum(a)&&a!='_';},
		modify);
}
bool EmptyString(string a)
{
	for(char i:a)
		if(!isspace(i))
			return false;
	return true;
}

/// @brief If is a valid var name 
bool CheckNameValid(string name)
{
	bool hasAlp=false;
	for(char i:name)
		if(isalpha(i)||i=='_')
			hasAlp=true;
		else if(isdigit(i));
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

/// @brief Container of `Attribute`
class AttributeMap
{
private:
	map<string,string> m;
public:
	bool Has(string key)
		{return m.find(key)!=m.end();}
	/// @return Whether succeed or not
	bool Set(string key,string val)
	{
		if(EmptyString(key)||EmptyString(val))
			return false;
		if(!CheckNameValid(key))
			return false;
		m[key]=val;
		return true;
	}
	string Get(string key)
	{
		if(!CheckNameValid(key))
			return "Invalid key name";
		auto i=m.find(key);
		if(i==m.end())	return "";
		else return i->second;
	}
	/**
	 * @brief Format attribute like this:
	 * ```
	 * key1="value1" key2="value2" key3="value3"...
	 * ```
	 * @param ansi If `true`, it will add color to string with ansi
	 */
	string ToString(bool ansi=false)
	{
		string ans;
		if(ansi) for(auto i:m)
			ans+=std::format(
				" %Cf[skyblue]{}%/=%Cf[chocolate]\"{}\"%/",
				i.first,i.second);
		else for(auto i:m)
			ans+=std::format(" {}=\"{}\"",i.first,i.second);
		if(ans.empty())	return ans;
		else	return ans.substr(1);
	}
	auto operator[](string key)
		{return m[key];}
};

class AttributeTracer
{
protected:
	string key,val;
	AttributeMap *tar;
	virtual void pull(){val=tar->Get(key);}
	virtual void push(){tar->Set(key,val);}
public:
	string operator=(string a){val=a;push();return a;}
	string Val()
	{
		if(tar==nullptr)
			throw pc::Exception("AttributeTracer not bound!");
		pull();
		return val;
	}
	void Bind(string k,AttributeMap* a)
	{
		tar=a;key=k;
		pull();
	}
	AttributeTracer():tar(nullptr){}
};
/// @brief Contianer of `ClassList`
class ClassSet: public AttributeTracer
{
private:
	set<string> c;
	void push()
	{
		val=ToString();
		AttributeTracer::push();
	}
	void pull()
	{
		AttributeTracer::pull();
		Parse(val);
	}
public:
	void Add(string arg)
	{
		pull();
		if(CheckNameValid(arg))
			c.insert(arg);
		push();
	}
	template<typename ...Tps>
	void Add(string arg,Tps ...args)
	{
		pull();
		Add(arg);
		Add(args...);
		push();
	}
	bool Has(string arg)
	{
		pull();
		if(!CheckNameValid(arg))	return false;
		return c.find(arg)!=c.end();
	}
	void Delete(string arg)
	{
		pull();
		if(CheckNameValid(arg))
			c.erase(arg);
		push();
	}
	void Toggle(string arg)
	{
		pull();
		if(!CheckNameValid(arg))	return;
		if(c.find(arg)==c.end())
			c.insert(arg);
		else
			c.erase(arg);
		push();
	}
	void Parse(string a)
	{
		c.clear();
		string n;
		while(a.length()>0)
		{
			n=BreakWord(a);
			if(CheckNameValid(n))
				c.insert(n);
		}
	}
	string ToString()
	{
		string ans;
		for(auto i:c)
			ans+=' '+i;
		if(ans.empty())	return ans;
		else	return ans.substr(1);
	}
	ClassSet operator=(string a)
		{Parse(a);return *this;}
	ClassSet():AttributeTracer(){}
	auto begin() const {return c.begin();}
	auto end() const {return c.end();}
};

}//namespace