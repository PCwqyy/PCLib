#pragma once
#define PCL_TUI_ELEMENT

#include<string>
#include<vector>
#include<set>
using std::vector;
using std::set;

#include"SytleSheet.hpp"
#include"Util.hpp"

/// @brief Element node
class Element
{
protected:
	string tag;
	Element* Parent;
	vector<Element> Children;
	string UUID;
	auto findNode(Element n)
	{
		for(auto i=Children.begin();i!=Children.end();i++)
			if(i->UUID==n.UUID)
				return i;
		return Children.end();
	}
	StyleSheet style,eleStyle;
	short left,top,height,width;
	map<string,StyleSheet>* StyleMap;
	void applyStyleByClass()
	{
		style=eleStyle;
		if(StyleMap==nullptr)	return;
		for(string i:ClassList)
		{
			auto a=StyleMap->find(i);
			if(a==StyleMap->end())	continue;
			style=style+a->second;
		}
	}
	pcpri::COORD printInit(short x,short y,
		map<string,StyleSheet>* c=nullptr)
	{
		left=x,top=y;
		if(c!=nullptr)	StyleMap=c;
		applyStyleByClass();
		return {left,top};
	}
	/**
	 * @brief Check if the element match the single element selector 
	 * like `#id.class1.class2` 
	 */
	bool matchSelector(string s)
	{
		bool match=true;
		if(s[0]=='#')
			if(Id!=util::BreakName(s))
				return false;
			else;
		else if(s[0]=='.')
			if(!ClassList.Has(util::BreakName(s)))
				return false;
			else;
		else return false;
		return true;
	}
public:
	util::ClassSet ClassList;
	util::AttributeMap Attribute;
	string Id;
	/**
	 * @brief Append a node as a child
	 * @return `true` if succeed \
	 * @return `false` if this node already is a child
	 */
	bool AppendChild(Element& n)
	{
		n.Parent=this;
		if(findNode(n)!=Children.end())
			return false;
		Children.push_back(n);
		return true;
	}
	/**
	 * @brief Remove a ChildNode
	 * @return `true` if succeed \
	 * @return `false` if no such node
	 */
	bool RemoveChild(Element& n)
	{
		n.Parent=nullptr;
		auto i=findNode(n);
		if(i==Children.end())
			return false;
		Children.erase(i);
		return true;
	}
	bool Append(Element&n){return n.AppendChild(*this);}
	bool Remove(Element&n){return n.RemoveChild(*this);}
	/// @brief Work like what you think. 
	vector<Element> QuerySelectorAll(string s)
	{
		vector<Element> ans;
		bool matched=false;
		string thisSelect=util::BreakWord(s);
		if(matchSelector(thisSelect))
		{
			matched=true;
			if(s.length()==0)
				ans.push_back(*this);
		}
		if(!matched)	s=thisSelect+' '+s;
		for(auto i:Children)
		{
			vector<Element> tmp=i.QuerySelectorAll(s);
			ans.insert(ans.end(),tmp.begin(),tmp.end());
		}
		return ans;
	} 
	virtual pcpri::COORD Print(short x,short y,
		short visWidth,map<string,StyleSheet>* c=nullptr)
		{return printInit(x,y,c);}
	string GetStyle(string attr){return style[attr];}
	void SetStyle(string attr,string val){style.SetAttribute(attr,val);}
	void SetStyle(StyleSheet a){style=a;}
	Element(string id="",vector<string> classes={})
	{
		UUID=util::GenUUID();
		height=0,width=0,left=0,top=0;
		StyleMap=nullptr;
		Id=id;
		for(auto i:classes)
			ClassList.Add(i);
	}
	virtual ~Element(){}
};