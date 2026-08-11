#pragma once
#define PCL_TUI_STYLESHEET

#include<string>
#include<vector>
#include<regex>
#include<map>
#include<set>
using std::string;
using std::vector;
using std::map;
using std::regex;
using std::set;

#include"Ansi.hpp"
#include"Util.hpp"

namespace PCSS{

enum class LengthUnit{px,ch,vw,vh};
const vector<string> LengthUnitName={"px","ch","vw","vh"};
enum class AngleUnit{deg,rad};
const vector<string> AngleUnitName={"deg","rad"};

namespace Value{

	struct Length{
		short number;
		LengthUnit unit;
		bool parse(string a)
		{
			if(!sscanf(util::BreakNumber(a).c_str(),"%d",&number))
				return false;
			std::optional res=util::StringToEnum<LengthUnit>(util::ToLowercase(a),LengthUnitName);
			unit=res.value_or(LengthUnit::px);
			return res.has_value();
		}
	};
}

// const map<string,> PCSSValue={
// 	{"<number>",}}
// };

const regex MatchAttr(R"(([^:\s]+?):\s*([^;]+?);)");

struct AttrInfo
{
	std::set<string> Valids;
	string Default;
	bool has(string q)const{return Valids.find(q)!=Valids.end();}
	bool match(string q)const
	{
		//TODO
		return false;
	}
};

const map<string,AttrInfo> Attributes=
{\
	{"width",{{"<length>"},"max"}},
	{"height",{{"<length>"},"fit"}},
	{"color",{{"<color>"},"lightgray"}},
	{"background-color",{{"<color>"},"black"}},
	{"border-color",{{"<color>"},"lightgray"}},
};

}// namespace PCSS

class StyleSheet
{
private:
	map<string,string> s;
	void parse(string t)
	{
		std::smatch res;
		auto begin=std::sregex_iterator(t.begin(),t.end(),PCSS::MatchAttr);
		auto end=std::sregex_iterator();
		string k,v;
		for(auto it=begin;it!=end;it++)
			k=it->operator[](1).str(),v=it->operator[](2).str(),
			SetAttribute(k,v);
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
		s[attr]=val;
		return true;
	}
	string GetAttribute(string attr)
	{
		auto res=s.find(attr);
		if(res==s.end()||res->second=="unset")
		{
			auto res2=PCSS::Attributes.find(attr);
			if(res2==PCSS::Attributes.end())
				throw "Invalid attribute!";
			return res2->second.Default;
		}
		else return res->second;
	}
	string operator[] (string k){return GetAttribute(k);}
	StyleSheet operator<< (StyleSheet other)
	{
		for(auto i:other.s)
			SetAttribute(i.first,i.second);
		return *this;
	}

};

