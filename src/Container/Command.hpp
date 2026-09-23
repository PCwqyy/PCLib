#pragma once
#define PCL_COMMAND

#include<string>
#include<vector>
#include<cstring>
using std::string;
using std::vector;
/**
 * @brief Command container
 * @note When declare:
 * ```cpp
 * Commands<> com=
 * {
 * 	{
 * 		{"com1",0,[](const char* msg){}},
 * 		{"com2",0,[](const char* msg){}},
 * 		...
 * 	},
 * 	[](const char* msg){}, //command not found
 * 	[](const char* msg){}, //permission denied
 * };
 * ```
 */
template<typename ...types>
struct Commands
{
	struct com
	{
		string name;
		int Permission;
		void (*func)(const char* msg,types ...args);
	};
	vector<com> command;
	/// @brief Function when command not found
	void (*unknown)(const char* msg,types ...args);
	/// @brief Function when permission denied
	void (*deny)(const char* msg,types ...args);
	char buff[1000];
	/**
	 * @brief Explain a command
	 * @param cmd Command string
	 * @param permit Permission level
	 * @param args Additional arguments
	 */
	void Explain(const char* cmd,int permit,types ...args)
	{
		sscanf(cmd,"%s",buff);
		for(auto i:command)
			if(buff==i.name)
			{
				if(permit<i.Permission)
				{
					deny(cmd,args...);
					return;
				}
				strcpy(buff,cmd+i.name.size()+1);
				i.func(buff,args...);
				return;
			}
		unknown(cmd,args...);
		return;
	}
};