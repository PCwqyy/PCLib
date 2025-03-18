#pragma once
#define PCL_COMMAND

#include<string>
#include<vector>
#include<cstring>
#include<cstdarg>
#include"VarSet.hpp"
#include"IO.hpp"
using std::string;
using std::vector;

#define pcCMD_TAG_CHAR 'c'
#define pcCMD_TAG_INT 'd'
#define pcCMD_TAG_LL 'l'
#define pcCMD_TAG_DOUBLE 'f'
#define pcCMD_TAG_STRING 's'

template<typename ...PubTp>
class Commands
{
public:
	struct com
	{
		string name,format;
		int permission,size=-1;
		VarSet arguments;
		void (*func)(VarSet PrivArg,PubTp ...PubArg);
		bool init()
		{
			size=format.length();
			for(int i=0;i<size;i++)
			switch(format[i])
			{
			case pcCMD_TAG_INT:
				arguments.New<int>();
			break;
			case pcCMD_TAG_LL:
				arguments.New<long long>();
			break;
			case pcCMD_TAG_DOUBLE:
				arguments.New<double>();
			break;
			case pcCMD_TAG_CHAR:
				arguments.New<char>();
			break;
			case pcCMD_TAG_STRING:
				arguments.New<string>();
			break;
			default:
				return false;
			break;
			}
			return true;
		}
		bool execute(const char* arg,PubTp ...args)
		{
			if(size==-1)
				return false;
			for(int i=0;i<size;i++)
			switch(format[i])
			{
			case pcCMD_TAG_INT:
				sscanpc(arg,ScanAnyIn<int>(arguments.At(i)));
			break;
			case pcCMD_TAG_LL:
				sscanpc(arg,ScanAnyIn<long long>(arguments.At(i)));
			break;
			case pcCMD_TAG_DOUBLE:
				sscanpc(arg,ScanAnyIn<double>(arguments.At(i)));
			break;
			case pcCMD_TAG_CHAR:
				sscanpc(arg,ScanAnyIn<char>(arguments.At(i)));
			break;
			case pcCMD_TAG_STRING:
				sscanpc(arg,ScanAnyIn<string>(arguments.At(i)));
			break;
			default:
				throw "Unkown tag";
			break;
			}
			func(arguments,args...);
			return true;
		}
		com(string n,string fo,int p,void (*fu)(VarSet pva,PubTp ...pba))
		{
			name=n,format=fo;
			permission=p;
			func=fu;
			if(!init())
				throw "Unknown format";
		}
	};
private:
	vector<com> command;
	void (*unknown)(const char* arg,PubTp ...args);
	void (*deny)(const char* arg,PubTp ...args);
	char buff[1000];
public:
	void Explain(const char* cmd,int permit,PubTp ...args)
	{
		sscanf(cmd,"%s",buff);
		for(auto i:command)
			if(buff==i.name)
			{
				if(permit<i.permission)
				{
					deny(cmd,args...);
					return;
				}
				strcpy(buff,cmd+i.name.size()+1);
				i.execute(buff,args...);
				return;
			}
		unknown(cmd,args...);
		return;
	}
	Commands(vector<com> v,void (*u)(const char* arg,PubTp ...args),
		void (*d)(const char* arg,PubTp ...args)):
		command(v),unknown(u),deny(d){}
};

#include"Multinclude.hpp"