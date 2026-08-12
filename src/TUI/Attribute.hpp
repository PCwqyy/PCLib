#pragma once
#define PCL_TUI_ATTRIBUTE

#include<map>
#include<string>
#include<set>
using std::string;

#include"../Util.hpp"

/// @brief Container of `Attribute`
class AttributeMap
{
private:
	std::map<string,string> m;
public:
	bool Has(string key)
		{return m.find(key)!=m.end();}
	/// @return Whether succeed or not
	bool Set(string key,string val="")
	{
		if(su::isEmpty(key)||!util::CheckNameValid(key))
			return false;
		m[key]=val;
		return true;
	}
	/// @brief Return empty string if not exist
	string Get(string key)
	{
		if(!util::CheckNameValid(key))
			return "Invalid key name";
		auto i=m.find(key);
		if(i==m.end())	return "";
		else return i->second;
	}
	/**
	 * @brief Format attribute like this:
	 * ```
	 * key1="value1" key2="value2" key3="value3"...
	 * ```
	 */
	string ToString()
	{
		string ans;
		for(auto i:m)
			if(i.second.empty())
				ans+=std::format("{} ",i.first);
			else
				ans+=std::format("{}=\"{}\" ",i.first,i.second);
		if(ans.empty())	return ans;
		ans.erase(ans.length()-1); // remove last space
		return ans;
	}
	auto operator[](string key) {return m[key];}
	auto begin() const {return m.begin();}
	auto end() const {return m.end();}
};

class AttributeTracer
{
protected:
	string key,val;
	AttributeMap *tar;
	virtual void pull(){val=tar->Get(key);}
	virtual void push(){tar->Set(key,val);}
public:
	string operator=(string a){val=a;push();return a;}
	/// @brief Get the value of the traced attribute
	string Val()
	{
		if(tar==nullptr)
			throw pc::Exception(pcXPT_INIT,"AttributeTracer not bound!");
		pull();
		return val;
	}
	/// @brief Bind the attribute tracer to a key and attribute map
	void Bind(string k,AttributeMap* a)
	{
		tar=a;key=k;
		pull();
	}
	AttributeTracer():tar(nullptr){}
};
/// @brief Contianer of `ClassList`
class ClassListTp: public AttributeTracer
{
private:
	set<string> c;
	void push(){
		val=ToString();
		AttributeTracer::push();
	}
	void pull(){
		AttributeTracer::pull();
		Parse(val);
	}
public:
	void Add(string arg)
	{
		pull();
		if(util::CheckNameValid(arg))
			c.insert(arg);
		push();
	}
	template<typename ...Tps>
	void Add(string arg,Tps ...args)
	{
		pull();
		Add(arg);
		Add(args...);
		push();
	}
	bool Has(string arg)
	{
		pull();
		if(!util::CheckNameValid(arg))	return false;
		return c.find(arg)!=c.end();
	}
	void Delete(string arg)
	{
		pull();
		if(util::CheckNameValid(arg))
			c.erase(arg);
		push();
	}
	void Toggle(string arg)
	{
		pull();
		if(!util::CheckNameValid(arg))	return;
		if(c.find(arg)==c.end())
			c.insert(arg);
		else
			c.erase(arg);
		push();
	}
	void Parse(string a)
	{
		c.clear();
		string n;
		while(a.length()>0)
		{
			n=su::ExtractWord(a);
			if(util::CheckNameValid(n))
				c.insert(n);
		}
	}
	string ToString()
	{
		string ans;
		for(auto i:c)
			ans+=' '+i;
		if(ans.empty())	return ans;
		else	return ans.substr(1);
	}
	ClassListTp operator=(string a)
		{Parse(a);return *this;}
	ClassListTp():AttributeTracer(){}
	auto begin() const {return c.begin();}
	auto end() const {return c.end();}
};