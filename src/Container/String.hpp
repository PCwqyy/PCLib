#pragma once
#define PCL_STRING

#include<cstring>
#include<string>
#include<format>
#include"../Exception.hpp"
#include"../Utility/Unicode.hpp"

#ifdef _WIN32
#include<stringapiset.h>
#endif

namespace pcpri{
	void strcpy(char16_t* dest,const char16_t* src)
	{
		while(*src)	*dest++=*src++;
		*dest=u'\0';
	}
};

#define pcSTR_EMPTY_CAPACITY 32
#define pcSTR_SAFE_MULTIPLE 0.8
class String
{
private:
	char16_t* str=nullptr;
	int cap=pcSTR_EMPTY_CAPACITY,lenCache=0;
	bool dirty=false;
	void applyCap(int s=-1)
	{
		if(s!=-1)	cap=s;
		char16_t* n=new char16_t[cap+5];
		n[0]=u'\0';
		if(str!=nullptr)
			pcpri::strcpy(n,str);
		delete[] str;
		str=n;
	}
	String& copyFrom(const String& f)
	{
		int len=f.Size();
		cap=f.cap;
		applyCap();
		pcpri::strcpy(str,f.str);
		dirty=true;
		return *this;
	}
	String& copyFrom(const char16_t* cstr)
	{
		int len=0;
		for(;cstr[len]!=u'\0';len++);
		ExtendTo(len);
		pcpri::strcpy(str,cstr);
		dirty=true;
		return *this;
	}
	String& copyFrom(const char* cstr)
	{
		int len=strlen(cstr);
		ExtendTo(len);
		for(int i=0;i<len;i++)
			str[i]=char16_t((unsigned char)cstr[i]);
		str[len]=u'\0';
		dirty=true;
		return *this;
	}
	String& copyFrom(const std::string& s)
	{
		int len=s.length();
		ExtendTo(len);
		for(int i=0;i<len;i++)
			str[i]=char16_t(s[i]);
		str[len]=u'\0';
		dirty=true;
		return *this;
	}
public:
	int Size() const
	{
		char16_t* s=str;
		if(s[0]==u'\0')	return 0;
		while(*s++!=u'\0');
		return s-str-1;
	}
	int Length()
	{
		if(!dirty)	return lenCache;
		int ret=cap;
		for(int i=0;i<cap;i++)
			if(str[i]==u'\0')
				dirty=false,
				lenCache=i,
				ret=i;
		return ret;
	}
	void ExtendBy(double multi)
	{
		if(multi<=1)	return;
		int tar=cap*multi;
		if(tar<=cap)	return;
		cap=tar;
		applyCap();
	}
	void ExtendTo(int len)
	{
		int tar=int(len/pcSTR_SAFE_MULTIPLE);
		if(tar<=cap&&str!=nullptr)	return;
		cap=tar;
		applyCap();
	}
	void Shrink()
	{
		cap=int(Length()/pcSTR_SAFE_MULTIPLE);
		applyCap();
	}
	String& Append(String a)
	{
		int i=Length(),len=a.Length()+Length();
		ExtendTo(len);
		for(int j=0;i<len;i++,j++)
			str[i]=a[j];
		str[len]=u'\0';
		dirty=true;
		return *this;
	}
	String& Append(char16_t c)
	{
		int len=Length();
		ExtendTo(len+1);
		str[len]=c;
		str[len+1]=u'\0';
		dirty=true;
		return *this;
	}
	String SubStr(int from,int to)
	{
		String ans(to-from);
		int i=0;
		for(;from<to;from++,i++)
			ans[i]=str[from];
		ans[i]=u'\0';
		return ans;
	}
	int Compare(String a)
	{
		int len=Length();
		if(len!=a.Length())
			return (len>a.Length())?1:-1;
		for(int i=0;i<len;i++)
			if(str[i]!=a[i])
				return (str[i]>a[i])?1:-1;
		return 0;
	}
	int Capacity(){return cap;}
	char16_t* CStr() const
	{
		char16_t* ret=new char16_t[Size()+5];
		pcpri::strcpy(ret,str);
		ret[Size()]=u'\0';
		return ret;
	}
	template<typename Tp>
	Tp To() const
	{
		if constexpr(std::is_same_v<Tp,int>)
			return std::stoi(std::format("{}",*this));
		else if constexpr(std::is_same_v<Tp,double>)
			return std::stod(std::format("{}",*this));
		else if constexpr(std::is_same_v<Tp,float>)
			return std::stof(std::format("{}",*this));
		else
			throw pc::Exception("Unsupported String::To<Tp>()");
	}
	String(){applyCap();}
	String(const String& a)
	{
		applyCap(a.cap);
		copyFrom(a);
	}
	String(const char* cstr)
	{
		applyCap();
		copyFrom(cstr);
	}
	String(const char16_t* cstr)
	{
		applyCap();
		copyFrom(cstr);
	}
	String(char16_t c,int cnt)
	{
		applyCap(cnt+1);
		for(int i=0;i<cnt;i++)
			str[i]=c;
		str[cnt]=u'\0';
		lenCache=cnt;
		dirty=false;
	}
	String(int capacity)
	{
		applyCap(capacity/pcSTR_SAFE_MULTIPLE);
	}
	String(const std::string& s)
	{
		applyCap(s.length()/pcSTR_SAFE_MULTIPLE);
		copyFrom(s);
	}
	String& operator=(String& from){return copyFrom(from);}
	String& operator=(String&& from){return copyFrom(std::move(from));}
	String& operator=(const char* from){return copyFrom(from);}
	String& operator=(const char16_t* from){return copyFrom(from);}
	String& operator=(const std::string& from){return copyFrom(from);}
	char16_t& operator[](int index){return str[index];}
	String friend operator+(String a,String b)
	{
		String ans=a;
		ans.Append(b);
		return ans;
	}
	String friend operator+(String a,char16_t b)
	{
		String ans=a;
		ans.Append(b);
		return ans;
	}
	String& operator+=(const String& other)
	{
		Append(other);
		return *this;
	}
	String& operator+=(char16_t c)
	{
		Append(c);
		return *this;
	}
	int friend operator<=>(String a,String b)
		{return a.Compare(b);}
	bool friend operator==(String a,String b)
		{return a.Compare(b)==0;}
	bool friend operator>(String a,String b)
		{return a.Compare(b)==1;}
	bool friend operator<(String a,String b)
		{return a.Compare(b)==-1;}
	bool friend operator>=(String a,String b)
		{return a.Compare(b)!=-1;}
	bool friend operator<=(String a,String b)
		{return a.Compare(b)!=1;}
	bool friend operator!=(String a,String b)
		{return a.Compare(b)!=0;} 
	operator bool()
		{return str[0]!=u'\0';}
};

#ifndef CP_GBK
#define CP_GBK 936
#endif

#ifdef __cpp_lib_format
template<>
struct std::formatter<String,char>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return ctx.begin();
	}
#ifdef _WIN32
	auto format(const String& s,std::format_context& ctx) const
	{
		char* gbkstr=new char[3*s.Size()+5];
		WideCharToMultiByte(CP_GBK,0,reinterpret_cast<wchar_t*>(s.CStr()),
			-1,gbkstr,3*s.Size()+5,nullptr,nullptr);
		auto result=std::format_to(ctx.out(),"{}",gbkstr);
		delete[] gbkstr;
		return result;
	}
#endif
#ifdef __linux__
	auto format(const String& s,std::format_context& ctx) const
	{
		char* u8str=new char[3*s.Size()+5];
		pcuni::uft16_to_uft8(s.CStr(),u8str);
		auto result=std::format_to(ctx.out(),"{}",u8str);
		delete[] u8str;
		return result;
	}
#endif
};
#endif