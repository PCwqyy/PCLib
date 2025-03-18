#pragma once
#define PCL_VARSET

#include<vector>
#include<any>
using std::type_info;
using std::any_cast;
using std::any;
using std::vector;

class VarSet
{
private:
	vector<any> data;
public:
	struct iterator
	{
		int index;
	};
	template<typename Tp>
	void Push(Tp&& val)
		{data.push_back(std::forward<Tp>(val));}
	template<typename Tp,typename ...Tps>
	void Push(Tp&& val,Tps... args)
	{
		Push(val);
		Push(args...);
		return;
	}
	template<typename Tp>
	void New()
	{
		Tp* tar=new Tp;
		Push(*tar);
	}
	VarSet(){}
	template<typename Tp>
	VarSet(int size,Tp arg)
	{
		for(int i=0;i<size;i++)
			Push(arg);
	}
	template<typename ...Tps>
	VarSet(Tps ...args){Push(args...);}
	template<typename Tp>
	Tp Get(int index)
	{
		if(index>=data.size())
			throw "bad_alloc";
		return any_cast<Tp>(data[index]);
	}
	template<typename Tp>
	Tp Set(int index,Tp&& val)
	{
		if(index>=data.size())
			throw "bad_alloc";
		data[index]=std::forward<Tp>(val);
	}
	any& At(int index)
		{return data[index];}
	void Clear(){data.clear();}
	bool Empty(){return data.empty();}
	int Size(){return data.size();}
	iterator begin(){return iterator(0);}
	iterator end(){return iterator(data.size());}
};

#include"Multinclude.hpp"