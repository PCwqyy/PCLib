#pragma once
#define PCL_COMMAND

#include<string>
#include<vector>
#include<cstring>
using std::string;
using std::vector;
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
	void (*unknown)(const char* msg,types ...args);
	void (*deny)(const char* msg,types ...args);
	char buff[1000];
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