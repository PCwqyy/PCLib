#pragma once
#define PCL_TUI_STYLESHEET

#include"../Util.hpp"
#include"Type.hpp"

#include<string>
#include<vector>
#include<regex>
#include<map>
#include<set>
using std::string;
using std::vector;
using std::regex;
using std::set;

#include"Ansi.hpp"
#include"Util.hpp"

namespace PCSS{

const regex rgAttr(R"(([^:\s]+?):\s*([^;]+?);)");

struct AttrInfo
{
	std::set<string> Valids;
	string Default;
	AttrInfo(std::set<string> validVal,string defVal)
		:Valids(validVal),Default(defVal){}
	bool has(string q)const{return Valids.find(q)!=Valids.end();}
	bool match(string q)const
	{
		//TODO
		return false;
	}
};
struct AttrInfos
{
	std::map<string,AttrInfo> map;
	AttrInfo& operator[](string key){return map[key];}
	AttrInfos& Add(string key,std::set<string> validVal,string defVal)
	{
		map[key]=AttrInfo(validVal,defVal);
		return *this;
	}
	AttrInfos(){
		Add("width",{"<length>","max","fit"},"max");
		Add("height",{"<length>","max","fit"},"fit");
		Add("color",{"<color>"},"lightgray");
		Add("background-color",{"<color>"},"black");
		Add("border-color",{"<color>"},"lightgray");
	}
}Attributes;

}// namespace PCSS

class StyleSheet
{
private:
	std::map<string,string> map;
	void parse(string str)
	{
		std::smatch res;
		auto begin=std::sregex_iterator(str.begin(),str.end(),PCSS::rgAttr);
		auto end=std::sregex_iterator();
		string key,val;
		for(auto it=begin;it!=end;it++)
			key=it->operator[](1).str(),val=it->operator[](2).str(),
			SetAttribute(key,val);
		return;
	}
public:
	StyleSheet(){}
	StyleSheet(string t){parse(t);}
	StyleSheet(const char* s){parse(string(s));}
	bool SetAttribute(string attr,string val)
	{
		// if(!PCSS::ValidStyle(attr,val))
		// 	return false;
		map[attr]=val;
		return true;
	}
	string GetAttribute(string key){return map.find(key)->second;}
	string operator[](string key){return GetAttribute(key);}
	StyleSheet& operator<< (StyleSheet other)
	{
		for(auto i:other.map)
			SetAttribute(i.first,i.second);
		return *this;
	}

};