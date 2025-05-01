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
	util::AttributeTracer ID;
	string Tag;
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
		int i;
		util::ShrinkStringHead(s);
		if(util::EmptyString(s))
			return false;
		if(s[0]=='*')
			return true;
		while(!util::EmptyString(s))
		{
			i=0;	while(isspace(s[i]))	i++;
			s=s.substr(i);
			if(s[0]=='#'&&ID.Val()!=util::BreakName(s))
				return false;
			else if(s[0]=='.'&&!ClassList.Has(util::BreakName(s)))
				return false;
			else if(isalnum(s[0])&&Tag!=util::BreakName(s))
				return false;
		}
		return true;
	}
public:
	util::ClassSet ClassList;
	util::AttributeMap Attribute;
	/**
	 * @brief Append a node as a child
	 * @return `true` if succeed \
	 * @return `false` if this node already is a child
	 */
	bool AppendChild(Element n)
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
	bool RemoveChild(Element n)
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
	string GetID(){return ID.Val();}
	bool SetID(string a)
	{
		if(!util::CheckNameValid(a))
			return false;
		ID=a;
		Attribute.Set("id",a);
		return true;
	}
	string GetTag(){return Tag;}
	/// @brief Work like what you think.
	/// @todo `>` 选择器，匹配仅下一级子元素
	vector<Element*> QuerySelectorAll(string s)
	{
		util::ShrinkStringHead(s);
		vector<Element*> ans;
		bool matched=false,child=(s[0]=='>');
		string thisSelect=util::BreakString(
			s,[](char a){return a=='>'||isspace(a);});
		if(matchSelector(thisSelect))
		{
			matched=true;
			if(util::EmptyString(s))
				ans.push_back(this);
		}
		if(!child) // 原选择器
			for(auto i=Children.begin();i!=Children.end();i++)
			{
				vector<Element*> tmp=i->QuerySelectorAll(thisSelect+' '+s);
				ans.insert(ans.end(),tmp.begin(),tmp.end());
			}
		if(!matched||util::EmptyString(s))	return ans;
		for(auto i=Children.begin();i!=Children.end();i++) // 子选择器
		{
			vector<Element*> tmp=i->QuerySelectorAll(s);
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
	Element(string tag="",string id="",string classes="")
	{
		UUID=util::GenUUID();
		Parent=nullptr;
		height=0,width=0,left=0,top=0;
		StyleMap=nullptr;
		Tag=tag;
		ID.Bind("id",&Attribute);
		ClassList.Bind("class",&Attribute);
		SetID(id);
		ClassList=classes;
	}
	Element(const Element& a)
	{
		UUID=a.UUID;
		Parent=a.Parent;
		StyleMap=a.StyleMap;
		Tag=a.Tag;
		Children=a.Children;
		Attribute=a.Attribute;
		ID.Bind("id",&Attribute);
		ClassList.Bind("class",&Attribute);
	}
	virtual ~Element(){}
};