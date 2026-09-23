#pragma once
#define PCL_UNICODES

#include<cstdint>
#include<string>
#include<format>
#include<functional>

namespace pc{
/// @brief Unicode utility functions and constants for PClib.
namespace unicode{

/// @brief Check if a UTF-16 code unit is a high surrogate (D800–DBFF).
inline bool isHighSurrogate(char16_t ch)
	{return ch>=0xD800&&ch<=0xDBFF;}
/// @brief Check if a UTF-16 code unit is a low surrogate (DC00–DFFF). 
inline bool isLowSurrogate(char16_t ch)
	{return ch>=0xDC00&&ch<=0xDFFF;}

#define pcUNI_REPLACEMENT_CODE 0xFFFD

/**
 * @brief Get the Unicode code point and the number of bytes read from a UTF-8 string at a given index.
 * @param utf8 The input UTF-8 encoded string.
 * @param i The index in the string to read from (default is 0).
 * @return A pair containing the Unicode code point and the number of bytes read.
 */
std::pair<uint32_t,int> Char8CodeAt(const std::string_view utf8,int i=0)
{
	int size=utf8.size();
	unsigned char c=static_cast<unsigned char>(utf8[i]);
	uint32_t codepoint=0;
	int bytes=0;
	bool valid=true;
	// 判断字节数
	if((c&0x80)==0)		// 0xxxxxxx
		codepoint=c,bytes=1;
	else if((c&0xE0)==0xC0)	// 110xxxxx
		codepoint=c&0x1F,bytes=2;
	else if((c&0xF0)==0xE0)	// 1110xxxx
		codepoint=c&0x0F,bytes=3;
	else if((c&0xF8)==0xF0)	// 11110xxx
		codepoint=c&0x07,bytes=4;
	else	goto tagInvalid;	// 无效首字节
	if(i+bytes>size)	goto tagInvalid;	// 截断
	// 累积码点
	for(int j=1;j<bytes;j++)
	{
		unsigned char next=static_cast<unsigned char>(utf8[i+j]);
		if((next&0xC0)!=0x80){	// 10xxxxxx
			valid=false;
			break;
		}
		codepoint=(codepoint<<6)|(next&0x3F);
	}
	if(!valid)	goto tagInvalid;
	// 合法性检查：过长编码、代理区、超出 Unicode 范围
	if((bytes==2&&codepoint<0x80)||
		(bytes==3&&codepoint<0x800)||
		(bytes==4&&codepoint<0x10000)||
		(codepoint>=0xD800&&codepoint<=0xDFFF)||
		codepoint>0x10FFFF
	)	goto tagInvalid;
	return std::make_pair(codepoint,bytes);
tagInvalid:
	return std::make_pair(pcUNI_REPLACEMENT_CODE,1);
}
/**
 * @brief Get the Unicode code point and the number of UTF-16 code units read from a UTF-16 string at a given index.
 * @param utf16 The input UTF-16 encoded string.
 * @param i The index in the string to read from (default is 0).
 * @return A pair containing the Unicode code point and the number of UTF-16 code units read.
 */
std::pair<uint32_t,int> Char16CodeAt(const std::u16string_view utf16,int i=0)
{
	char16_t w=utf16[i];
	uint32_t codepoint=0;
	if(isHighSurrogate(w))	// 高代理
		if(i+1>=utf16.size())
			codepoint=pcUNI_REPLACEMENT_CODE;
		else
		{
			char16_t low=utf16[i+1];
			if(isLowSurrogate(low))
			{
				uint32_t highVal=(w-0xD800)<<10;
				uint32_t lowVal=(low-0xDC00);
				codepoint=0x10000+highVal+lowVal;
			}else
				codepoint=pcUNI_REPLACEMENT_CODE;
		}
	else if(isLowSurrogate(w))	// 单独低代理不合法 
		codepoint=pcUNI_REPLACEMENT_CODE;
	else	codepoint=w;
	return std::make_pair(codepoint,isHighSurrogate(w)?2:1);
}

/// @brief Parse a Unicode code to UTF-16 and push into out string
std::u16string& PushCode16(std::u16string& out,uint32_t codepoint)
{
	if(codepoint<=0xFFFF)
		out.push_back(static_cast<char16_t>(codepoint));
	else{	// 代理对
		codepoint-=0x10000;
		uint32_t high=0xD800|((codepoint>>10)&0x3FF);
		uint32_t low =0xDC00|(codepoint&0x3FF);
		out.push_back(static_cast<char16_t>(high));
		out.push_back(static_cast<char16_t>(low));
	}
	return out;
}
/// @brief Parse a Unicode code to UTF-16 and return as string
inline std::u16string ParseCode16(uint32_t codepoint){
	std::u16string utf16;
	PushCode16(utf16,codepoint);
	return utf16;
}
/// @brief Parse a Unicode code to UTF-8 and push into out string
std::string& PushCode8(std::string& out,uint32_t codepoint)
{
	if(codepoint<=0x7F)
		out.push_back(static_cast<char>(codepoint));
	else if(codepoint<=0x7FF)
		out.push_back(static_cast<char>(0xC0|((codepoint>>6)&0x1F))),
		out.push_back(static_cast<char>(0x80|(codepoint&0x3F)));
	else if(codepoint<=0xFFFF)
		out.push_back(static_cast<char>(0xE0|((codepoint>>12)&0x0F))),
		out.push_back(static_cast<char>(0x80|((codepoint>>6)&0x3F))),
		out.push_back(static_cast<char>(0x80|(codepoint&0x3F)));
	else if(codepoint<=0x10FFFF)
		out.push_back(static_cast<char>(0xF0|((codepoint>>18)&0x07))),
		out.push_back(static_cast<char>(0x80|((codepoint>>12)&0x3F))),
		out.push_back(static_cast<char>(0x80|((codepoint>>6)&0x3F))),
		out.push_back(static_cast<char>(0x80|(codepoint&0x3F)));
	else	// 超出 U+10FFFF 用 U+FFFD 替换
		out.push_back(static_cast<char>(0xEF)),
		out.push_back(static_cast<char>(0xBF)),
		out.push_back(static_cast<char>(0xBD));
	return out;
}
/// @brief Parse a Unicode code to UTF-8 and return as string
inline std::string ParseCode8(uint32_t codepoint){
	std::string utf8;
	PushCode8(utf8,codepoint);
	return utf8;
}

/// @brief Iterate over each UTF-8 character in a string and apply a function to it.
/// @warning Read only!
void ForEachU8(std::string& utf8,std::function<void(std::string_view)> func){
	int size=utf8.size();
	std::string_view view=utf8;
	for(int i=0;i<size;){
		auto [codepoint,bytes]=Char8CodeAt(utf8,i);
		func(view.substr(i,bytes));
		i+=bytes;
	}
}
/// @brief Iterate over each UTF-16 character in a string and apply a function to it.
/// @warning Read only!
void ForEachU16(std::u16string& utf16,std::function<void(std::u16string_view)> func){
	int size=utf16.size();
	std::u16string_view view=utf16;
	for(int i=0;i<size;){
		auto [codepoint,bytes]=Char16CodeAt(utf16,i);
		func(view.substr(i,bytes));
		i+=bytes;
	}
}

/// @brief Convert a UTF-8 encoded string to a UTF-16 encoded string. 
std::u16string UTF8to16(const std::string_view utf8)
{
	std::u16string utf16;
	int size=utf8.size();
	utf16.reserve(size);
	for(int i=0;i<size;)
	{
		auto [codepoint,bytes]=Char8CodeAt(utf8,i);
		i+=bytes;
		PushCode16(utf16,codepoint);
	}
	return utf16;
}
/// @brief Convert a UTF-16 encoded string to a UTF-8 encoded string.
std::string UTF16to8(const std::u16string_view utf16)
{
	std::string utf8;
	int size=utf16.size();
	utf8.reserve(size*3);
	for(int i=0;i<size;)
	{
		auto [codepoint,read]=Char16CodeAt(utf16,i);
		i+=read;
		PushCode8(utf8,codepoint);
	}
	return utf8;
}
/// @brief Convert a UTF-8 encoded string to a UTF-32 encoded string.
std::u32string UTF8to32(const std::string_view utf8)
{
	std::u32string utf32;
	int size=utf8.size();
	utf32.reserve(size);
	for(int i=0;i<size;)
	{
		auto [codepoint,bytes]=Char8CodeAt(utf8,i);
		i+=bytes;
		utf32.push_back(static_cast<char32_t>(codepoint));
	}
	return utf32;
}
/// @brief Convert a UTF-16 encoded string to a UTF-32 encoded string.
std::u32string UTF16to32(const std::u16string_view utf16)
{
	std::u32string utf32;
	int size=utf16.size();
	utf32.reserve(size);
	for(int i=0;i<size;)
	{
		auto [codepoint,read]=Char16CodeAt(utf16,i);
		i+=read;
		utf32.push_back(static_cast<char32_t>(codepoint));
	}
	return utf32;
}
/// @brief Convert a UTF-32 encoded string to a UTF-8 encoded string.
std::string UTF32to8(const std::u32string_view utf32)
{
	std::string utf8;
	int size=utf32.size();
	utf8.reserve(size*4);
	for(int i=0;i<size;i++)
		PushCode8(utf8,static_cast<uint32_t>(utf32[i]));
	return utf8;
}
/// @brief Convert a UTF-32 encoded string to a UTF-16 encoded string.
std::u16string UTF32t16(const std::u32string_view utf32)
{
	std::u16string utf16;
	int size=utf32.size();
	utf16.reserve(size*2);
	for(int i=0;i<size;i++)
		PushCode16(utf16,static_cast<uint32_t>(utf32[i]));
	return utf16;
}

/**
 * @note The following code handles printing UTF-8&16 to console on
 * different operating systems with all functions named `PutUTF`.
 * On Windows, it uses WriteConsoleW to print UTF-16 strings,
 * while on Unix-like systems, it prints UTF-8 strings directly to std::cout.
 * // Windows sucks! I hate you!
 */
#ifdef _WIN32
namespace Win32{
#include<windef.h>
#include<winbase.h>
#include<wincon.h>
}
/// @brief Stardard input handle for Windows console.
const Win32::HANDLE hIn=Win32::GetStdHandle((unsigned long)-10);
/// @brief Stardard output handle for Windows console.
const Win32::HANDLE hOut=Win32::GetStdHandle((unsigned long)-11);
/// @brief Print a UTF-16 string to the console on Windows.
inline void PutUTF(std::u16string s){
	Win32::WriteConsoleW(hOut,s.c_str(),s.length(),NULL,NULL);
}
/// @brief Print a UTF-8 string to the console on Windows. 
inline void PutUTF(std::string s){
	PutUTF(UTF8to16(s));
}
/// @brief Print a UTF-32 string to the console on Windows. 
inline void PutUTF(std::u32string s){
	PutUTF(UTF32t16(s));
}

#define pcUNI_GETLINE_WIN_BUF 1024
/// @brief Getline from the console on Windows.
std::string GetUTF(){
	std::u16string buf(pcUNI_GETLINE_WIN_BUF,u'\0');
	unsigned long charRead=0;
	if(!Win32::ReadConsoleW(hIn,buf.data(),buf.size(),&charRead,NULL))
		return "";
	buf.resize(charRead);
	return UTF16to8(buf);
}

#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include<iostream>
/// @brief Print a UTF-8 string to the console on Unix-like systems.
inline void PutUTF(std::string s){
	std::cout<<s;
}
/// @brief Print a UTF-16 string to the console on Unix-like systems.
inline void PutUTF(std::u16string s){
	std::cout<<UTF16to8(s);
}
/// @brief Print a UTF-32 string to the console on Unix-like systems.
inline void PutUTF(std::u32string s){
	std::cout<<UTF32to8(s);
}

/// @brief Getline from the console on Unix-like systems.
std::string GetUTF(){
	std::string buf;
	std::getline(std::cin,buf);
	return buf;
}
#endif

template<typename ...Tps>
inline void PrintUTF(std::string_view fmt,Tps...args){
	PutUTF(std::vformat(fmt,std::make_format_args(args...)));
}
template<typename ...Tps>
inline void PrintLnUTF(std::string_view fmt,Tps...args){
	PutUTF(std::vformat(fmt,std::make_format_args(args...))+"\n");
}

/// @brief Namespace for Unicode constants and utility functions.
namespace constant{

struct range{unsigned short first,last;};
inline bool biSearch(unsigned short u,const range *table,int max)
{
	int l=0,r=max;
	while(l<r)
	{
		int m=(l+r)/2;
		if(u<table[m].first)		r=m;
		else if(u>table[m].last)	l=m+1;
		else return true;
	}
	return false;
}

/// @brief Combining characters (width 0) - from Markus Kuhn's wcwidth
const range combining[]=
{
	{0x0300,0x036F},{0x0483,0x0489},{0x0591,0x05BD},{0x05BF,0x05BF},
	{0x05C1,0x05C2},{0x05C4,0x05C5},{0x05C7,0x05C7},{0x0610,0x061A},
	{0x064B,0x065F},{0x0670,0x0670},{0x06D6,0x06DC},{0x06DF,0x06E4},
	{0x06E7,0x06E8},{0x06EA,0x06ED},{0x0711,0x0711},{0x0730,0x074A},
	{0x07A6,0x07B0},{0x07EB,0x07F3},{0x0816,0x0819},{0x081B,0x0823},
	{0x0825,0x0827},{0x0829,0x082D},{0x0859,0x085B},{0x08D3,0x08E1},
	{0x08E3,0x0903},{0x093A,0x093C},{0x093E,0x094F},{0x0951,0x0957},
	{0x0962,0x0963},{0x0981,0x0983},{0x09BC,0x09BC},{0x09BE,0x09C4},
	{0x09C7,0x09C8},{0x09CB,0x09CD},{0x09D7,0x09D7},{0x09E2,0x09E3},
	{0x0A01,0x0A03},{0x0A3C,0x0A3C},{0x0A3E,0x0A42},{0x0A47,0x0A48},
	{0x0A4B,0x0A4D},{0x0A51,0x0A51},{0x0A70,0x0A71},{0x0A75,0x0A75},
	{0x0A81,0x0A83},{0x0ABC,0x0ABC},{0x0ABE,0x0AC5},{0x0AC7,0x0AC9},
	{0x0ACB,0x0ACD},{0x0AE2,0x0AE3},{0x0B01,0x0B03},{0x0B3C,0x0B3C},
	{0x0B3E,0x0B44},{0x0B47,0x0B48},{0x0B4B,0x0B4D},{0x0B56,0x0B57},
	{0x0B62,0x0B63},{0x0B82,0x0B82},{0x0BBE,0x0BC2},{0x0BC6,0x0BC8},
	{0x0BCA,0x0BCD},{0x0BD7,0x0BD7},{0x0C00,0x0C04},{0x0C3E,0x0C44},
	{0x0C46,0x0C48},{0x0C4A,0x0C4D},{0x0C55,0x0C56},{0x0C62,0x0C63},
	{0x0C81,0x0C83},{0x0CBC,0x0CBC},{0x0CBE,0x0CC4},{0x0CC6,0x0CC8},
	{0x0CCA,0x0CCD},{0x0CD5,0x0CD6},{0x0CE2,0x0CE3},{0x0D01,0x0D03},
	{0x0D3B,0x0D3C},{0x0D3E,0x0D44},{0x0D46,0x0D48},{0x0D4A,0x0D4D},
	{0x0D57,0x0D57},{0x0D62,0x0D63},{0x0D81,0x0D83},{0x0DCA,0x0DCA},
	{0x0DCF,0x0DD4},{0x0DD6,0x0DD6},{0x0DD8,0x0DDF},{0x0DF2,0x0DF3},
	{0x0E31,0x0E31},{0x0E34,0x0E3A},{0x0E47,0x0E4E},{0x0EB1,0x0EB1},
	{0x0EB4,0x0EBC},{0x0EC8,0x0ECD},{0x0F18,0x0F19},{0x0F35,0x0F35},
	{0x0F37,0x0F37},{0x0F39,0x0F39},{0x0F3E,0x0F3F},{0x0F71,0x0F84},
	{0x0F86,0x0F8B},{0x0F90,0x0F95},{0x0F97,0x0F97},{0x0F99,0x0FAD},
	{0x0FB1,0x0FB7},{0x0FB9,0x0FB9},{0x20D0,0x20DC},{0x20E1,0x20E1},
	{0x302A,0x302F},{0x3099,0x309A},{0xFE20,0xFE2F}
};
/// @brief East Asian wide/fullwidth ranges (width 2)
const range wide[]=
{
	{0x1100,0x115F},{0x2329,0x232A},{0x2E80,0xA4CF},
	{0xAC00,0xD7A3},{0xF900,0xFAFF},{0xFE10,0xFE19},
	{0xFE30,0xFE6F},{0xFF00,0xFF60},{0xFFE0,0xFFE6}
};

} // namespace constant

#define pcUNI_SURROGATE_WIDTH 2

/**
 * @brief Get display width for a single UTF-16 code unit in a monospace console:
 *  - 0 for NUL, control codes and combining marks
 *  - 2 for East Asian wide/fullwidth characters, surrogates
 *  - 1 otherwise
 */
int charWidthInConsole(char16_t ch)
{
	const unsigned short u=static_cast<unsigned short>(ch);
	// C0/C1 control characters
	if(u==0)			return 0;
	if(u<0x20)			return 0;
	if(u>=0x7f&&u<0xa0)	return 0;
	// Surrogates
	if(u>=0xD800&&u<=0xDFFF)
		return pcUNI_SURROGATE_WIDTH;
	// Combining marks
	if(constant::biSearch(u,constant::combining,
		sizeof(constant::combining)/sizeof(constant::range)))
		return 0;
	// East Asian Wide/Fullwidth -> width 2
	if(constant::biSearch(u,constant::wide,
		sizeof(constant::wide)/sizeof(constant::range)))
		return 2;
	return 1; // default
}

} // namespace pc::uni
namespace uni=unicode;
} // namespace pc

/** @brief std::formatter for u16string */
template<>
struct std::formatter<std::u16string_view>:std::formatter<std::string_view>{
    auto format(const std::u16string_view& s,std::format_context& ctx)const{
        return std::formatter<std::string_view>::format(pc::uni::UTF16to8(s),ctx);
    }
};
/** @brief std::formatter for u32string */
template<>
struct std::formatter<std::u32string_view>:std::formatter<std::string_view>{
    auto format(const std::u32string_view& s,std::format_context& ctx)const{
        return std::formatter<std::string_view>::format(pc::uni::UTF32to8(s),ctx);
    }
};
/** @brief std::formatter for char16 */
template<>
struct std::formatter<char16_t>:std::formatter<std::string_view>{
    auto format(const char16_t& c,std::format_context& ctx)const{
		std::u16string tmp;
		tmp.push_back(c);
        return std::formatter<std::string_view>::format(pc::uni::UTF16to8(tmp),ctx);
    }
};
/** @brief std::formatter for char32 */
template<>
struct std::formatter<char32_t>:std::formatter<std::string_view>{
    auto format(const char32_t& c,std::format_context& ctx)const{
		std::u32string tmp;
		tmp.push_back(c);
        return std::formatter<std::string_view>::format(pc::uni::UTF32to8(tmp),ctx);
    }
};
