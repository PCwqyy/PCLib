#pragma once
#define PCL_TUI_PCML

#include<cstdio>
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

const regex XMLDeclaration(R"(^\s*<\?xml([^<>]*?)\?>)");
const regex OpenTag(R"(^\s*<(\w+[^<>]*?)>)");
const regex SelfCloseTag(R"(^\s*<(\w+[^<>]*?)/>)");
const regex Comment(R"(^\s*<!--[\s\S]*?-->)");
const regex Attribute("(\\w+?)=\"(.*?)\"");

/// @brief Parse content inside `< >`
Element parseTag(string s)
{
	string tag=util::BreakWord(s),id;
	Element ans(tag);smatch res;
	while(regex_search(s,res,Attribute))
	{
		ans.Attribute.Set(res[1],res[2]);
		s=res.suffix();
	}
	return ans;
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
	std::optional<Element> tmp;
	while(true)
	{
		if(regex_search(pcml,res,
			regex(format(R"(^\s*</{}(| .*?)>)",ans.GetTag()))))
		{
			pcml=res.suffix();
			break;
		}
		tmp=Make(pcml);
		if(!tmp.has_value())	break;
		ans.AppendChild(tmp.value());
	}
	return ans;
}

#define pcML_ERR_XML "Can't find XML declaration of the Ducument"
/// @todo 解析整个文档 
Document Parse(string path)
{
	FILE* flPCML=fopen(path.c_str(),"r");
	string pcml;char in;
	while(true)
	{
		in=fgetc(flPCML);
		if(in==EOF)	break;
		pcml+=in;
	}
	smatch res;
	if(!regex_search(pcml,res,XMLDeclaration))
		throw pc::Exception(pcML_ERR_XML);
	pcml=res.suffix();
	Document ans=parseTag("xml "+string(res[1]));
	std::optional<Element> tmp;
	while(!util::EmptyString(pcml))
	{
		tmp=Make(pcml);
		if(tmp.has_value())
			ans.AppendChild(tmp.value());
	}
	return ans;
}

}//namespace