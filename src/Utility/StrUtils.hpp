#pragma once
#define PCL_STRUTILS

#include<string>
#include<vector>
#include<set>
#include<functional>
using std::string;

/**
 * @brief Utilitys for string processing
 * @note Added by PClib!
 */
namespace su{
	/// @brief Function to decide whether a char is something
	typedef std::function<bool(char)> CharType;
	/// @brief Pre-defined delimiter functions
	namespace Delimiters{
		const CharType Word=	[](char a){return bool(isspace(a));};
		const CharType Name=	[](char a){return !isalnum(a)&&a!='_';};
		const CharType Line=	[](char a){return a=='\r'||a=='\n';};
		const CharType Int=	[](char a){return bool(isspace(a));};
		const CharType Double=	[](char a){return bool(isspace(a));};
		CharType List(std::string_view str){
			return [str](char a){return str.contains(a);};
		}
	}
	/// @brief Remove leading and trailing whitespace from a string
	string Trim(string a)
	{
		int j=a.length(),i=0;
		while(i<j&&isspace(a[i]))	i++;
		while(j>i&&isspace(a[j-1]))	j--;
		return a.substr(i,j-i);
	}
	/// @brief Remove leading and trailing whitespace from a string
	string Trim(string a,bool (*isSpace)(char a))
	{
		int j=a.length(),i=0;
		while(i<j&&isSpace(a[i]))	i++;
		while(j>i&&isSpace(a[j-1]))	j--;
		return a.substr(i,j-i);
	}
	/**
	 * @brief Get a token breaking with delimiter from string
	 * @param isDelimiter Decide whether the char is a delimiter
	 * @param modify Whether delete the word form origin string, default `true`
	 * @return The gotten token
	 */
	string ExtractToken(string& a,CharType isDelimiter,bool modify=true)
	{
		string ans;
		int i=0,len=a.length();
		while(i<len&&isDelimiter(a[i]))	i++;
		for(;i<len;i++)
			if(isDelimiter(a[i]))	break;
			else	ans+=a[i];
		if(!modify)	return ans;
		a=a.substr(i);
		return ans;
	}
	inline string ExtractToken(string& a,std::string_view delimiters,bool modify=true){
		return ExtractToken(a,[delimiters](char a){
			return delimiters.contains(a);
		},modify);
	}
	/// @brief Break by space 
	inline string ExtractWord(string& a,bool modify=true){
		return ExtractToken(a,Delimiters::Word,modify);
	}
	/// @brief Name contains alphabets, numbers and underlines(`_`) 
	inline string ExtractName(string& a,bool modify=true){
		return ExtractToken(a,Delimiters::Name,modify);
	}
	/// @brief Break by enter
	inline string ExtractLine(string& a,bool modify=true){
		return ExtractToken(a,Delimiters::Line,modify);
	}
	/**
	 * @warning Inputing `1.23` will get `1` and `.23`!
	 * Consider using `ExtractDouble()` for that.
	 */
	inline int ExtractInt(string& a,bool modify=true){
		string num=ExtractToken(a,Delimiters::Int,modify);
		return std::stoi(num);
	}
	/// @brief Works literally 
	inline double ExtractDouble(string& a,bool modify=true){
		string num=ExtractToken(a,Delimiters::Double,modify);
		return std::stod(num);
	}
	/// @brief Check if a string is empty (contains only whitespace)
	inline bool isEmpty(string a)
	{
		for(char i:a)
			if(!isspace(i))
				return false;
		return true;
	}

	/**
	 * @brief Split string with delimiters
	 * @param isDelimiter Decide whether the char is a delimiter
	 */
	std::vector<string> Split(std::string_view str,CharType isDelimiter)
	{
		std::vector<string> ans;
		string one;
		for(char i:str)
			if(isDelimiter(i))
				if(!one.empty())
					ans.push_back(one),
					one="";
				else;
			else
				one.push_back(i);
		if(!one.empty())
			ans.push_back(one);
		return ans;
	}
	/// @brief Split string with delimiters
	std::vector<std::string> Split(std::string_view str, std::string_view delimiters){
		return Split(str,Delimiters::List(delimiters));
	}
	/// @brief Split string with space
	std::vector<std::string> Split(std::string_view str)
		{return Split(str,Delimiters::Word);}
	string ToLowercase(string a){
		for(char &i:a)
			i=tolower(i);
		return a;
	}
	string ToUppercase(string a){
		for(char &i:a)
			i=toupper(i);
		return a;
	}
	string Capitalize(string a){
		if(a.length()>0)
			a[0]=toupper(a[0]);
		return a;
	}
}