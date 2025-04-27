#pragma once
#define PCL_EXCEPTION

#include<cstring>
#include<cstdio>
#include<exception>

namespace pc
{

#define pcXPT_LENGTH 1024
#define pcXPT_DEFAULT_CONTENT \
"An uncatched exception from PClib thrown"

/**
 * @brief This is the exception class thrown by PCLib.
 * @warning The length of message **can not** be greater than the macro
 * `pcXPT_LENGTH` (default to 1024) */
class Exception:public std::runtime_error
{
protected:
	char message[pcXPT_LENGTH];
public:
	explicit Exception():std::runtime_error(pcXPT_DEFAULT_CONTENT)
		{strcpy(message,pcXPT_DEFAULT_CONTENT);}
	template<typename ...Tps>
	explicit Exception(const char* fmt,Tps ...args)
		:std::runtime_error(pcXPT_DEFAULT_CONTENT)
		{sprintf(message,fmt,args...);}
	const char* what() const noexcept override {return message;}
};

};//namespace