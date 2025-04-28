#pragma once
#define PCL_TUI_PCML

#include<regex>
#include<format>
using std::regex;
using std::smatch;
using std::format;
using std::regex_search;

#include"../Exception.hpp"
#include"Util.hpp"
#include"Element.hpp"
#include"Document.hpp"

namespace PCML
{

const regex XMLDeclaration(R"(^\s*<\?xml(.*?)\?>)");
const regex OpenTag(R"(^\s*<(\w+.*?)>)");
const regex SelfCloseTag(R"(^\s*<(\w+.*?)/>)");
const regex Comment(R"(^\s*<!--[\s\S]*?-->)");
const regex Attribute("(\\w+?)=\"(.*?)\"");

/// @brief Parse content inside `< >`
Element parseTag(string s)
{
	string tag=util::BreakWord(s),id;
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
/**
 * @brief Make a element with pcml
 * @param pcml The source string. Parsed part will be eraesed.
 * @return `std::nullopt` if there is no element left in pcml
 */
std::optional<Element> Make(string& pcml)
{
	smatch res;
	while(regex_search(pcml,res,Comment))
		pcml=res.suffix();
	if(regex_search(pcml,res,SelfCloseTag))
	{
		Element ans=parseTag(res[1]);
		pcml=res.suffix();
		return ans;
	}
	if(!regex_search(pcml,res,OpenTag))
		return std::nullopt;
	Element ans=parseTag(res[1]);
	pcml=res.suffix();
	if(!regex_search(pcml,res,regex(format("</{}(| .*?)>",ans.GetTag()))))
		throw pc::Exception("Unable to find a close tag </%d>",ans.GetTag());
	string work=res.prefix();
	pcml=res.suffix();
	std::optional<Element> tmp;
	while(true)
	{
		tmp=Make(work);
		if(!tmp.has_value())
			break;
		ans.AppendChild(tmp.value());
	}
	return ans;
}
/// @todo 解析整个文档 
Document Parse(string pcml);

}//namespace