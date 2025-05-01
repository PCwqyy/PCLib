#pragma once
#define PCL_ASYNCDATA

#include<set>
using std::set;

#include"../Exception.hpp"

/**
 * @brief A data container that synchronizes with each other
 * in real time. Based on union-find set algorithm.
 * @details All the reads and writes to the data will redirected to
 * the data of a specific "root" of the SyncedData group.
 */
template<typename Tp>
class SyncedData
{
private:
	SyncedData* from=this;
	set<SyncedData*> to;
	Tp data;
	SyncedData* root()
	{
		if(from!=this)
			from=from->root(),
			from->to.insert(this);
		return from;
	}
	/// @brief Find the root without modify
	SyncedData* trace() const
	{
		SyncedData* i=from;
		while(i!=i->from)
			i=i->from;
		return i;
	}
public:
	SyncedData(){from=this;}
	SyncedData(Tp& d):data(d){from=this;}
	SyncedData(const SyncedData& s)
	{
		from=s.trace();
		from->to.insert(this);
		data=s.data;
	}
	/// @brief Check if the two `SyncedData` shared same source.
	friend bool SameSource(SyncedData &a,SyncedData &b)
		{return a.root()==b.root();}
	/**
	 * @brief Set source of **the whole synced group**,
	 * which means **merge** the two groups.
	 */
	void SetSource(SyncedData &a)
	{
		if(&a==this)	return;
		SyncedData *cur=root();
		cur->from=a.root();
		a.root()->to.insert(cur);
	}
	/// @brief Set data and sync to the whole synced group.
	void SetData(Tp a)
		{root()->data=a;}
	/// @brief Get the data. 
	Tp GetData(){return root()->data;}
	/// @brief Set data and sync to the whole synced group. 
	Tp operator=(Tp d){SetData(d);return d;}
	~SyncedData()
	{
		if(from==this)
			if(!to.empty())
			{
				SyncedData* newRoot=(*to.begin());
				to.erase(to.begin());
				newRoot->from=newRoot;
				for(auto i:to)
					i->from=newRoot,
					newRoot->to.insert(i);
			}
			else;
		else
		{
			from->to.erase(this);
			for(auto i:to)
				i->from=from,
				from->to.insert(i);
		}
	}
};