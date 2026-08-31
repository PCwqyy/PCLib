#pragma once
#define PCL_ENUMLOOKUP

#include<map>
#include<algorithm>
#include<tuple>
#include"../Exception.hpp"

/**
 * @brief Map a constant to an enum type, 
 * quickly looking up corresponding enum value for a value
 * @note value to key: `EnumLookup(value)`, key to value: `EnumLookup[key]`
 * @tparam Enum The enum class
 * @tparam Tp The type of the constant
 */
template<typename Enum,typename Tp>
class EnumLookup
{
private:
	std::map<Enum,Tp> map;
	Enum find(Tp val) const
	{
		auto it=std::ranges::find(map,val,&std::pair<Enum,Tp>::second);
		if(it!=map.end())	return it->first;
		throw pc::Exception("EnumLookup"+pcXPT_INVALID_ARGUMENT,
			"Can't find corresponding enum for a constant");
	}
	Tp get(Enum val) const
	{
		try{
			return map.at(val);
		}catch(std::out_of_range& xpt){
			throw pc::Exception("EnumLookup"+pcXPT_INVALID_ARGUMENT,
				"Can't find corresponding constant for an enum, what(): {}",xpt.what());
		}
	}
	template<typename Tuple,std::size_t... I>
	void makeWithTuples(const Tuple& tup,std::index_sequence<I...>){
		(Map(std::get<2*I>(tup),std::get<2*I+1>(tup)),...);
	}
public:
	/**
	 * @brief Construct a EnumLookup with a list of (key,constant) pairs
	 * @note Each enum value followed by its corresponding constant. 
	 * Use like this:
	 * ```cpp
	 * EnumLookup<EnumType,ConstantType> lookup{
	 * 	Enum::Key1,Constant1,
	 * 	Enum::Key2,Constant2,
	 * 	// ... 
	 * };
	 */
	template<typename ...Tps>
	EnumLookup(Tps&& ...args)
	{
		static_assert(sizeof...(args) % 2 == 0,
			"The number of arguments must be even");
		auto tup=std::forward_as_tuple(std::forward<Tps>(args)...);
		constexpr int N=sizeof...(args)/2;
		makeWithTuples(tup,std::make_index_sequence<N>{});
	}
	Enum operator()(Tp val) const {return find(val);}
	Enum operator[](Tp val) const {return find(val);}
	Tp operator()(Enum key) const {return get(key);}
	Tp operator[](Enum key) const {return get(key);}
	void Map(Enum key,Tp val){
		map.emplace(key,val);
	}
};