#pragma once
#define PCL_JSON

#include<string>
#include<meta>
#include<ranges>
#include<format>
#include"Meta.hpp"
#include"Container/Json.hpp"

namespace pc{
/// @brief Json serialization utilities using reflection of C++26
namespace json{

/// @brief Alias for `std::ranges::range`
template<typename Tp>
concept RangeType=std::ranges::range<Tp>;
/**
 * @brief The types that are not iterable ranges but are aggregates
 * (like structs), which can be serialized by iterating over their members.
 */
template<typename Tp>
concept AggregateType=
	!std::ranges::range<std::remove_cvref_t<Tp>>&&
	std::is_aggregate_v<std::remove_cvref_t<Tp>>;

class Weak: public BasicValue<Weak>
{
public:
	using BasicValue::BasicValue;

	template<typename Tp>
	static Weak From(const Tp& any);

	template<typename Tp>
	Weak(const Tp& any){*this=From(any);}

	/// @brief Built-in Weakener for enum types, returns the identifier of the enum value
	template<typename Tp> requires std::is_enum_v<Tp>
	static Weak FromEnum(const Tp& val){
		return Weak::String(rfl::NameOfEnum(val));
	}
	/// @brief Built-in Weakener for pointer types, returns "null" for nullptr
	template<typename Tp>
	static Weak FromPointer(const Tp* p){
		if(p==nullptr)	return Weak::Null();
		else return From(*p);
	}
	/**
	 * @brief Built-in Weakener for iterable ranges
	 * (e.g., `std::vector`, `std::list`, etc.)
	 */
	template<RangeType Tp>
	static Weak FromRange(const Tp& arr){
		Weak ans=Weak::EmptyArray();
		for(const auto& i:arr)
			ans.PushBack(From(i));
		return ans;
	}

	template<AggregateType Tp>
	static Weak FromAggregate(const Tp& obj)
	{
		Weak ans=Weak::EmptyObject();
		template for(constexpr auto member:
			std::define_static_array(
				std::meta::nonstatic_data_members_of(
					^^std::remove_cvref_t<Tp>,std::meta::access_context::current()
		)))	ans[std::meta::identifier_of(member)]=From(obj.[:member:]);
		return ans;
	}
	/// @brief Fallback Weakener for unsupported types, returns the type name
	template<typename Tp>
	static Weak FromUnsupported(const Tp& val){
		return Weak::String(std::format("[{}]",rfl::TypenameOf(val)));
	}
};

template<typename Tp>
struct Weakener{
	Weak weaken(const Tp& val)const
	{
		using T=std::remove_cvref_t<Tp>;
		if constexpr(std::is_same_v<T,Weak>)
			return val;
		// arithmetic
		else if constexpr(std::is_arithmetic_v<T>)
			// bool
			if constexpr(std::is_same_v<T,bool>)
				return Weak::Bool(val);
			// char
			else if constexpr(std::is_same_v<T,char>
							||std::is_same_v<T,char16_t>
							||std::is_same_v<T,char32_t>)
				return Weak::String(std::format("{}",static_cast<unsigned int>(val)));
			// numbers
			else	return Weak::Number(static_cast<double>(val));
		// string
		else if constexpr(std::is_same_v<T,std::string>
						||std::is_same_v<T,std::string_view>)
			return Weak::String(val);
		// char[]
		else if constexpr(std::is_array_v<T>
			&&std::is_same_v<std::remove_extent_t<T>,char>)
			return Weak::String(val);
		// enum
		else if constexpr(std::is_enum_v<T>)
			return Weak::FromEnum(val);
		// pointer
		else if constexpr(std::is_pointer_v<T>)
			return Weak::FromPointer(val);
		// iterable
		else if constexpr(RangeType<T>)
			return Weak::FromRange(val);
		// aggregate
		else if constexpr(AggregateType<T>)
			return Weak::FromAggregate(val);
		// fallback
		else return Weak::FromUnsupported(val);
	}
};
template<typename Tp>
Weak Weak::From(const Tp& any){
	return json::Weakener<std::remove_cvref_t<Tp>>{}.weaken(any);
}

template<typename TpFrom,typename TpTo>
requires std::is_same_v<TpFrom,Value>&&std::is_same_v<TpTo,Weak>||
		 std::is_same_v<TpFrom,Weak>&&std::is_same_v<TpTo,Value>
inline TpTo ConvertBetween(const TpFrom& from)
{
    if(from.IsNull())		return TpTo::Null();
    if(from.IsBool())		return TpTo::Bool(from.AsBool());
    if(from.IsNumber())	return TpTo::Number(from.AsNumber());
    if(from.IsString())	return TpTo::String(from.AsString());
    if(from.IsArray()){
        TpTo arr=TpTo::EmptyArray();
        for(const auto& i:from.AsArray())
            arr.PushBack(ConvertBetween<TpFrom,TpTo>(i));
        return arr;
    }
    if(from.IsObject()){
        TpTo obj=TpTo::EmptyObject();
        for(const auto& [k,val]:from.AsObject())
            obj[k]=ConvertBetween<TpFrom,TpTo>(val);
        return obj;
    }
    return TpTo::Null();
}
inline Weak ToWeak(const Value& from){
	return ConvertBetween<Value,Weak>(from);
}
inline Value ToValue(const Weak& from){
	return ConvertBetween<Weak,Value>(from);
}

} // namespace json
using Weak=json::Weak;

} // namespace pc


#if __has_include(<optional>)
#include<optional>
namespace pc::json{
/**
 * @brief Specialization of `Weakener` for `std::optional`,
 * serializes the contained value or "null" if empty
 */
template<typename Tp>
struct Weakener<std::optional<Tp>>{
	Weak weaken(const std::optional<Tp>& opt)const{
		if(opt.has_value())
			return Weak::From(opt.value());
		else	return Weak::Null();
	}
};
} // namespace pc::json
#endif

#if __has_include(<map>)
#include<map>
namespace pc::json{
/**
 * @brief Specialization of `Weakener` for `std::map<std::string,TpV>`,
 * serializes the map as a JSON object
 */
template<typename TpV>
struct Weakener<std::map<std::string,TpV>>{
	Weak weaken(const std::map<std::string,TpV>& map)const{
		Weak ans=Weak::EmptyObject();
		for(const auto& p:map)
			ans[p.first]=Weak::From(p.second);
		return ans;
	}
};
/**
 * @brief Specialization of `Weakener` for `std::map<int,TpV>`,
 * serializes the map as a JSON object
 */
template<typename TpV>
struct Weakener<std::map<int,TpV>>{
	Weak weaken(const std::map<int,TpV>& map)const{
		Weak ans=Weak::EmptyObject();
		for(const auto& p:map)
			ans[std::format("{}",p.first)]=Weak::From(p.second);
		return ans;
	}
};
} // namespace pc::json
#endif

#if __has_include(<variant>)
#include<variant>
namespace pc::json{
/**
 * @brief Specialization of `Weakener` for `std::variant`,
 * serializes the currently held value
 */
template<typename... Tps>
struct Weakener<std::variant<Tps...>>{
	Weak weaken(const std::variant<Tps...>& var)const{
		Weak ans;
		std::visit([&](const auto& visit){
			using T=std::remove_cvref_t<decltype(visit)>;
			if constexpr(std::is_same_v<T,std::monostate>)
				ans=Weak::Null();
			else ans=Weak::From(visit);
		},var);
		return ans;
	}
};
} // namespace pc::json
#endif

#if __has_include(<utility>)
#include<utility>
namespace pc::json{
/**
 * @brief Specialization of `Weakener` for `std::pair`,
 * serializes the pair as a JSON object
 */
template<typename Tp1,typename Tp2>
struct Weakener<std::pair<Tp1,Tp2>>{
	Weak weaken(const std::pair<Tp1,Tp2>& p)const{
		Weak ans=Weak::EmptyObject();
		ans["first"]=Weak::From(p.first);
		ans["second"]=Weak::From(p.second);
		return ans;
	}
};
} // namespace pc::json
#endif

#if __has_include(<tuple>)
#include<tuple>
namespace pc::json{
/**
 * @brief Specialization of `Weakener` for `std::tuple`,
 * serializes the tuple as a JSON array
 */
template<typename... Tps>
struct Weakener<std::tuple<Tps...>>{
	Weak weaken(const std::tuple<Tps...>& tup)const{
		Weak ans=Weak::EmptyArray();
		std::apply([&](const auto&... items){
			auto append=[&](const auto& item){
				ans.PushBack(Weak::From(item));
			};
			(append(items),...);
		},tup);
		return ans;
	}
};
} // namespace pc::json
#endif

#if __has_include(<memory>)
#include<memory>
namespace pc::json{
/**
 * @brief Specialization of `Weakener` for `std::unique_ptr`,
 * serializes the pointed value or "null" if empty
 */
template<typename Tp>
struct Weakener<std::unique_ptr<Tp>>{
	Weak weaken(const std::unique_ptr<Tp>& p)const{
		if(p==nullptr)	return Weak::Null();
		else return Weak::From(*p);
	}
};
/**
 * @brief Specialization of `Weakener` for `std::shared_ptr`,
 * serializes the pointed value or "null" if empty
 */
template<typename Tp>
struct Weakener<std::shared_ptr<Tp>>{
	Weak weaken(const std::shared_ptr<Tp>& p)const{
		if(p==nullptr)	return Weak::Null();
		else return Weak::From(*p);
	}
};
} // namespace pc::json
#endif