#pragma once
#define PCL_ASYNCDATA

#include<vector>
using std::vector;

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
	SyncedData* from;
	Tp data;
	SyncedData* root()
	{
		if(from!=this)
			from=from->root();
		return from;
	}
public:
	SyncedData(){from=this;}
	SyncedData(Tp d){from=this;data=d;}
	SyncedData(SyncedData& source)
		{SetSource(source);}
	/// @brief Check if the two `SyncedData` shared same source.
	friend bool SameSource(SyncedData a,SyncedData b)
		{return a.root()==b.root();}
	/**
	 * @brief Set source of **the whole synced group**,
	 * which means **merge** the two groups.
	 */
	void SetSource(SyncedData a)
		{root()->from=a.root();}
	/// @brief Set data and sync to the whole synced group.
	void SetData(Tp a)
		{root()->data=a;}
	/// @brief Get the data. 
	Tp GetData(){return root()->data;}
	/// @brief Set data and sync to the whole synced group. 
	Tp operator=(Tp d){SetData(d);return d;}
};