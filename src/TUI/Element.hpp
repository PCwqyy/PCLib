#pragma once
#define PCL_TUI_ELEMENT

#include<string>
#include<vector>
#include<set>
#include<memory>
#include<algorithm>

using std::vector;
using std::set;

#include"Buffer.hpp"
#include"Util.hpp"
#include"StyleSheet.hpp"

/// @brief Element node
class Element
{
protected:
	string Tag;
	Element* Parent;
	// store children as owning unique_ptrs -> stable addresses, no accidental slicing
	vector<std::unique_ptr<Element>> Children;
	string UUID;
	// find by UUID
	auto FindNodeByUUID(const string& uuid)
	{
		return std::find_if(Children.begin(),Children.end(),
			[&](const std::unique_ptr<Element>& p){return p&&p->UUID==uuid;});
	}
	Buffer view;
	StyleSheet style,inlineStyle;
	/**
	 * @brief Check ifthe element match the single element selector 
	 * like `#id.class1.class2[attr=value]`
	 */
	bool matchSingleSelector(string s)
	{
		util::ShrinkStringHead(s);
		if(util::EmptyString(s)) return false;
		if(s[0]=='*') return true;
		// parse tokens: `#id`, `.class`, `tag`, `[attr=value]`
		while(!util::EmptyString(s))
		{
			util::ShrinkStringHead(s);
			char ch=s[0];
			string now=util::BreakSelector(s);
			if(ch=='#') // #id
				if(ID.Val()!=now)	return false;
				else continue;
			else if(ch=='.') // .class
				if(!ClassList.Has(now))	return false;
				else continue;
			else if(ch=='[') // [attr=value]
			{
				string attr=util::BreakName(now);
				string val=now.substr(1);
				if(Attribute.Get(attr)!=val)	return false;
				else continue;
			}
			else if(isalnum(ch)) // tag
				if(Tag!=now)	return false;
				else continue;
		}
		return true;
	}
public:
	util::AttributeTracer ID;
	util::ClassSet ClassList;
	util::AttributeMap Attribute;
	/**
	 * @brief Append a node as a child
	 * @return `true` if succeed
	 * @return `false` if this node already is a child
	 */
	bool AppendChild(std::unique_ptr<Element> child)
	{
		if(!child)	return false;
		if(child->Parent!=nullptr)
			return false;
		if(FindNodeByUUID(child->UUID)!=Children.end())
			return false;
		child->Parent=this;
		Children.push_back(std::move(child));
		return true;
	}
	bool AppendChild(const Element& child)
		{return AppendChild(std::make_unique<Element>(child));}
	/**
	 * @brief Remove a ChildNode by element or UUID
	 * @return `true` if succeed
	 * @return `false` if there's no such node
	 */
	bool RemoveChild(const string& uuid)
	{
		auto it=FindNodeByUUID(uuid);
		if(it==Children.end()) return false;
		(*it)->Parent=nullptr;
		Children.erase(it);
		return true;
	}
	bool RemoveChild(const Element& n)
		{return RemoveChild(n.UUID);}
	string GetID(){return ID.Val();}
	bool SetID(string a)
	{
		if(!util::CheckNameValid(a)) return false;
		ID=a;
		Attribute.Set("id",a);
		return true;
	}
	string GetTag(){return Tag; }
	string OuterPCML(int tab=0)
	{
		string ans=std::string(tab*4,' ');
		string attrStr=Attribute.ToString();
		if(!attrStr.empty())
			ans+=std::format("<{} {}",Tag,attrStr);
		else
			ans+=std::format("<{}",Tag);
		if(Children.empty())	ans+="/>\n";
		else
		{
			ans+=">\n";
			for(auto& i:Children)
				ans+=i->OuterPCML(tab+1);
			ans+=std::format("{}</{}>\n",std::string(tab*4,' '),Tag);
		}
		return ans;
	}
	string ToString()
	{
		string ans=std::format("<{}",Tag);
		string attrStr=Attribute.ToString();
		if(!attrStr.empty())
			ans+=' '+attrStr;
		ans+='>';
		return ans;
	}
	/// @brief Work like what you think.
	vector<Element*> QuerySelectorAll(string s)
	{
		util::ShrinkStringHead(s);
		vector<Element*> ans;
		bool matched=false;
		bool childOnly=(!util::EmptyString(s)&&s[0]=='>');
		string thisSelect=util::BreakString(// 取出第一个选择器
			s,[](char a){return a=='>'||isspace(a);});
		if(matchSingleSelector(thisSelect))
		{
			matched=true;
			if(util::EmptyString(s))
				ans.push_back(this);
		}
		if(!childOnly) // 选择器以`>`开头时，仅匹配子元素
			for(auto& childPtr:Children)
			{ // 匹配原选择器
				string pass=thisSelect;
				if(!util::EmptyString(s)) pass+=' ',pass+=s;
				vector<Element*> tmp=childPtr->QuerySelectorAll(pass);
				ans.insert(ans.end(),tmp.begin(),tmp.end());
			}
		if(!matched||util::EmptyString(s)) return ans;
		for(auto& childPtr:Children)
		{ // 匹配新选择器
			vector<Element*> tmp=childPtr->QuerySelectorAll(s);
			ans.insert(ans.end(),tmp.begin(),tmp.end());
		}
		return ans;
	}
	///@todo optimize for single selector
	Element* QuerySelector(string s)
	{
		auto res=QuerySelectorAll(s);
		if(res.empty())	return nullptr;
		else return res[0];
	}
	/// @brief Render to buffer at (x,y)
	virtual Coord Render()
	{
		/* just print element name for debug use */
		Color bkg=RandomColor(50,30);
		view.SetBkgColor(bkg);
		string text=std::format("{}",ToString());
		Coord cur={0,0};
		cur=view.Render(cur,-1,bkg,text);
		cur.Set(1,cur.y+1);
		short lineHeight=0;
		for(auto& childPtr:Children)
		{
			Coord childSize=childPtr->Render();
			lineHeight=std::max(lineHeight,childSize.y);
			if(cur.x+childSize.x>60)// line break for debug
				cur.Set(1,cur.y+lineHeight),lineHeight=0;
			childPtr->view.RenderTo(view,cur);
			cur.Offset(childSize.x,0);
		}
		view.ShinkToFit();
		return view.Size();
	}
	string GetStyle(string attr){return inlineStyle[attr];}
	void SetStyle(string attr,string val)
	{
		inlineStyle.SetAttribute(attr,val);
	}
	void SetStyle(StyleSheet a){inlineStyle=a;}
	Element(string tag="",string id="",string classes="")
	{
		UUID=util::GenUUID();
		Parent=nullptr;
		style=inlineStyle;//debug
		Tag=tag;
		ID.Bind("id",&Attribute);
		ClassList.Bind("class",&Attribute);
		SetID(id);
		ClassList=classes;
	}
	/// @brief Deep-copy: clone children to keep ownership consistent
	Element(const Element& a)
	{
		UUID=a.UUID;
		Parent=nullptr; // copy should not keep same parent pointer
		Tag=a.Tag;
		style=a.style;
		inlineStyle=a.inlineStyle;
		Attribute=a.Attribute;
		ID.Bind("id",&Attribute);
		ClassList.Bind("class",&Attribute);
		// clone children
		Children.clear();
		for(const auto& ch:a.Children)
			if(ch)
			{
				auto cloned=std::make_unique<Element>(*ch);
				cloned->Parent=this;
				Children.push_back(std::move(cloned));
			}
	}
	/// @brief disabled copy-assignment to avoid accidental shallow copies
	Element& operator=(const Element&)=delete;
	virtual ~Element()
	{
		for(auto& childPtr:Children)
			childPtr->Parent=nullptr,
			childPtr.reset();
	}
};