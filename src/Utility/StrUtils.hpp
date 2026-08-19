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
	 * @brief Get a word breaking with delimiter from string
	 * @param isDelimiter Decide whether the char is a delimiter
	 * @param modify Whether delete the word form origin string, default `true`
	 * @return The gotten word
	 */
	string ExtractToken(string& a,bool (*isDelimiter)(char a),bool modify=true)
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
	/// @brief Break by space 
	inline string ExtractWord(string& a,bool modify=true)
	{
		return ExtractToken(a,
			[](char a){return bool(isspace(a));},
			modify);
	}
	/// @brief Name contains alphabets, numbers and underlines(`_`) 
	inline string ExtractName(string& a,bool modify=true)
	{
		return ExtractToken(a,
			[](char a){return !isalnum(a)&&a!='_';},
			modify);
	}
	/**
	 * @warning Inputing `1.23` will get `1` and `.23`!
	 * Consider using `ExtractDouble()` for that.
	 */
	inline int ExtractInt(string& a,bool modify=true)
	{
		string num=ExtractToken(a,[](char a){return !isdigit(a);},modify);
		return std::stoi(num);
	}
	/// @brief Works literally 
	inline double ExtractDouble(string& a,bool modify=true)
	{
		string num=ExtractToken(a,[](char a){return !isdigit(a)&&a!='.';},modify);
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
	std::vector<string> Split(std::string_view str,std::function<bool(char)> isDelimiter)
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
	std::vector<std::string> Split(std::string_view str, std::string_view delimiters)
	{
		return Split(str,[delimiters](char a){
			return delimiters.contains(a);
		});
	}
	/// @brief Split string with space
	std::vector<std::string> Split(std::string_view str)
		{return Split(str,[](char a){return bool(isspace(a));});}
	string ToLowercase(string a)
	{
		for(char &i:a)
			i=tolower(i);
		return a;
	}
	string ToUppercase(string a)
	{
		for(char &i:a)
			i=toupper(i);
		return a;
	}
	string Capitalize(string a)
	{
		if(a.length()>0)
			a[0]=toupper(a[0]);
		return a;
	}
}