#pragma once
#define PCL_TUI_PCML

#include<regex>
using std::regex;
using std::smatch;
using std::regex_search;

#include"Util.hpp"
#include"Element.hpp"

namespace PCML
{

const regex XMLDeclaration(R"(<\?xml(.*?)\?>)");
const regex OpenTag(R"(<(\w+)(.*?)>)");
const regex CloseTag(R"(</(\w+)(.*?)>)");
const regex SelfCloseTag(R"(<(\w+)(.*?)/>)");
const regex Comment(R"(<!--(.*?)-->)");

const regex Attribute("(\\w+?)=\"(.*?)\"");

/// @brief Parse content inside `< >`
Element parseTag(string s)
{
	string tag=util::BreakWord(s);
	util::ID id;
	util::ClassSet classes;
	util::AttributeMap attr;
	smatch res;
	while(regex_search(s,res,Attribute))
	{
		if(res[1]=="id")
			id=res[2];
		else if(res[1]=="class")
			classes=res[2];
		else
			attr.Set(res[1],res[2]);
		s=res.suffix();
	}
	return Element(tag,id,classes,attr);
}
/// @todo 解析一个元素
Element Make(string pcml)
{

}
/// @todo 解析整个文档 
Element Parse(string pcml)
{

}

}//namespace