#pragma once
#define PCL_STRUTILS

#include<string>
using std::string;

/**
 * @brief Utilitys for string processing
 * @note Added by PClib!
 */
namespace su{
	/// @brief Remove leading whitespace from a string
	void TrimHead(string &a)
	{
		int len=a.length(),i=0;
		while(i<len&&isspace(a[i]))	i++;
		a=a.substr(i);
	}
	/**
	 * @brief Get a word breaking with `isDelimiter()` from string
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