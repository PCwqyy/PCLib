#pragma once
#define PCL_ENUMLOOKUP

#include<string>
#include<vector>
using std::string;
#include"../Exception.hpp"

/**
 * @brief Map a string to an enum type, 
 * quickly looking up corresponding enum value for a string
 * @warning The order of string must matches with enum declaration
 * @note Use like this: `EnumLookup(str)`
 * @tparam Enum The enum class
 */
template<typename Enum>
class EnumLookup
{
private:
	std::vector<string> map;
	Enum get(string name) const
	{
		name=su::ToLowercase(name);
		int i=0;
		for(string s:map)
			if(name==s)
				return static_cast<Enum>(i);
			else	i++;
		throw pc::Exception(pcXPT_INVALID_ARGUMENT,
			"Can't find corresponding enum for '{}'",name);
	}
public:
	template<typename ...Tps>
	EnumLookup(Tps&& ...args){
		(map.push_back(string(args)),...);
	}
	Enum operator()(string name) const {
		return get(name);
	}
};