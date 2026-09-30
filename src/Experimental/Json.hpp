#pragma once
#define PCL_JSON

#include<string>
#include<meta>
#include<ranges>
#include<format>
#include"Meta.hpp"

namespace pc{
/// @brief Json serialization utilities using reflection of C++26
namespace json{

/// @brief Serialize a value to JSON string
template<typename Tp>
std::string Make(const Tp& val);

/// @brief Built-in serializer for std::string_view
std::string MakeString(const std::string_view& str)
{
	std::string ans="\"";
	ans.reserve(str.size()+4);
	for(unsigned char c:str)
		switch(c){
			case '"':	ans += "\\\"";	break;
			case '\\':	ans += "\\\\";	break;
			case '\n':	ans += "\\n";	break;
			case '\r':	ans += "\\r";	break;
			case '\t':	ans += "\\t";	break;
			case '\b':	ans += "\\b";	break;
			case '\f':	ans += "\\f";	break;
			default:
				if(c<0x20)
					ans+=std::format("\\u{:04x}",static_cast<int>(c));
				else	ans+=static_cast<char>(c);
		};
	ans+="\"";
	return ans;
}
/// @brief Built-in serializer for enum types, returns the identifier of the enum value
template<typename Tp> requires std::is_enum_v<Tp>
std::string MakeEnum(const Tp& val){
	return std::format(R"("{}")",rfl::NameOfEnum(val));
}
/// @brief Built-in serializer for arithmetic types (int, float, bool, etc.)
template<typename Tp> requires std::is_arithmetic_v<Tp>
std::string MakeArithmetic(const Tp& val)
{
	using T=std::remove_cvref_t<Tp>;
	if constexpr(std::is_same_v<T,bool>)	// bool
		return val?"true":"false";
	else if constexpr(std::is_same_v<T,char>
					||std::is_same_v<T,char16_t>
					||std::is_same_v<T,char32_t>)
		return std::format("{}",static_cast<unsigned int>(val));
	else	return std::format("{}",val);
}
/// @brief Built-in serializer for pointer types, returns "null" for nullptr
template<typename Tp>
std::string MakePointer(const Tp* p){
	if(p==nullptr)
		return "null";
	return Make(*p);
}
/// @brief Alias for `std::ranges::range`
template<typename Tp>
concept RangeType=std::ranges::range<Tp>;
/**
 * @brief Built-in serializer for iterable ranges
 * (e.g., `std::vector`, `std::list`, etc.)
 */
template<RangeType Tp>
std::string MakeRange(const Tp& arr)
{
	std::string ans="[";
	for(auto i:arr)
		ans+=Make(i)+",";
	if(ans.size()>1)
		ans.back()=']';
	else	ans.push_back(']');
	return ans;
}
/**
 * @brief The types that are not iterable ranges but are aggregates
 * (like structs), which can be serialized by iterating over their members.
 */
template<typename Tp>
concept AggregateType=
    !std::ranges::range<std::remove_cvref_t<Tp>>&&
    std::is_aggregate_v<std::remove_cvref_t<Tp>>;
/// @brief Built-in serializer for aggregate types, serializes each member
template<AggregateType Tp>
std::string MakeAggregate(const Tp& obj)
{
	std::string ans="{";
	template for(constexpr auto member:
		std::define_static_array(
			std::meta::nonstatic_data_members_of(
				^^Tp,
				std::meta::access_context::current()
			)
		)
	)
		ans+=std::format(R"("{}":{},)",
			std::meta::identifier_of(member),
			Make(obj.[:member:]));
	if(ans.size()>1)
		ans.back()='}';
	else	ans.push_back('}');
	return ans;
}
/// @brief Fallback serializer for unsupported types, returns the type name
template<typename Tp>
std::string MakeUnsupported(const Tp& val){
	return std::format(R"("[{}]")",rfl::TypenameOf(val));
}
/// @brief Concept to check if a type has a member function `ToJson()`
template<typename Tp>
concept HasMemberToJson=requires(const Tp& v){
	{v.ToJson()}->std::convertible_to<std::string>;
};
/**
 * @brief The main serializer struct, which dispatches to the appropriate serialization function
 * based on the type of the value.
 * @note A `Serializer` must contains methods `encode` & `decode`,
 * and the signature are as follows:
 * ```cpp
 * std::string encode(const Tp& val);
 * Tp decode(const std::string& str);
 * ```
 */
template<typename Tp>
struct Serializer{
	/// @brief Serialize a value to JSON string 
	std::string encode(const Tp& val)
	{
		using T=std::remove_cvref_t<Tp>;
		if constexpr(std::is_arithmetic_v<T>)
			return MakeArithmetic(val);
		else if constexpr(std::is_same_v<T,std::string>
						||std::is_same_v<T,std::string_view>
						||std::is_same_v<Tp,char*>)
			return MakeString(val);
		else if constexpr(std::is_enum_v<T>)
			return MakeEnum(val);
		else if constexpr(std::is_pointer_v<T>)
			return MakePointer(val);
		else if constexpr(std::is_class_v<T>)
			if constexpr(HasMemberToJson<T>)
				return val.ToJson();
			else if constexpr(RangeType<T>)
				return MakeRange(val);
			else if constexpr(AggregateType<T>)
				return MakeAggregate(val);
			else	return MakeUnsupported(val);
		else
			return MakeUnsupported(val);
	}
};
/// @brief Serialize a value to JSON string
template<typename Tp>
inline std::string Make(const Tp& val){
	return json::Serializer<Tp>{}.encode(val);
}

} // namespace json

/// @brief Serialize a value to JSON string
template<typename Tp>
inline std::string ToJson(const Tp& val){
	return json::Serializer<Tp>{}.encode(val);
}

} // namespace pc


#if __has_include(<optional>)
#include<optional>
namespace pc::json{
/**
 * @brief Specialization of `Serializer` for `std::optional`,
 * serializes the contained value or "null" if empty
 */
template<typename Tp>
struct Serializer<std::optional<Tp>>{
	std::string encode(const std::optional<Tp>& opt){
		if(opt.has_value())
			return pc::json::Make(opt.value());
		else	return "null";
	}
};
} // namespace pc::json
#endif

#if __has_include(<map>)
#include<map>
namespace pc::json{
/**
 * @brief Specialization of `Serializer` for `std::map<std::string,TpV>`,
 * serializes the map as a JSON object
 */
template<typename TpV>
struct Serializer<std::map<std::string,TpV>>{
	std::string encode(const std::map<std::string,TpV>& map){
		std::string ans="{";
		for(std::pair<std::string,TpV> p:map)
			ans+=std::format(R"("{}":{},)",p.first,pc::json::Make(p.second));
		if(ans.size()>1)	ans.back()='}';
		else	ans.push_back('}');
		return ans;
	}
};
/**
 * @brief Specialization of `Serializer` for `std::map<int,TpV>`,
 * serializes the map as a JSON object
 */
template<typename TpV>
struct Serializer<std::map<int,TpV>>{
	std::string encode(const std::map<int,TpV>& map){
		std::string ans="{";
		for(std::pair<int,TpV> p:map)
			ans+=std::format(R"("{}":{},)",p.first,pc::json::Make(p.second));
		if(ans.size()>1)	ans.back()='}';
		else	ans.push_back('}');
		return ans;
	}
};
} // namespace pc::json
#endif

#if __has_include(<variant>)
#include<variant>
namespace pc::json{
/**
 * @brief Specialization of `Serializer` for `std::variant`,
 * serializes the currently held value
 */
template<typename... Tps>
struct Serializer<std::variant<Tps...>>{
	std::string encode(const std::variant<Tps...>& var){
		return std::visit([](const auto& visit){
			using T=std::remove_cvref_t<decltype(visit)>;
			if constexpr(std::is_same_v<T,std::monostate>)
				return std::string("null");
			else	return pc::json::Make(visit);
		},var);
	}
};
} // namespace pc::json
#endif

#if __has_include(<utility>)
#include<utility>
namespace pc::json{
/**
 * @brief Specialization of `Serializer` for `std::pair`,
 * serializes the pair as a JSON object
 */
template<typename Tp1,typename Tp2>
struct Serializer<std::pair<Tp1,Tp2>>{
	std::string encode(const std::pair<Tp1,Tp2>& p){
		return std::format(R"({{"first":{},"second":{}}})",
			pc::json::Make(p.first),
			pc::json::Make(p.second));
	}
};
} // namespace pc::json
#endif

#if __has_include(<tuple>)
#include<tuple>
namespace pc::json{
/**
 * @brief Specialization of `Serializer` for `std::tuple`,
 * serializes the tuple as a JSON array
 */
template<typename... Tps>
struct Serializer<std::tuple<Tps...>>{
    std::string encode(const std::tuple<Tps...>& tup){
        std::string ans="[";
        std::apply([&](const auto&... items){
            auto append=[&](const auto& item){
				ans+=Make(item)+",";
            };
            (append(items),...);
        },tup);
		if(ans.size()>1)	ans.back()=']';
		else	ans.push_back(']');
        return ans;
    }
};
} // namespace pc::json
#endif

#if __has_include(<memory>)
#include<memory>
namespace pc::json{
/**
 * @brief Specialization of `Serializer` for `std::unique_ptr`,
 * serializes the pointed value or "null" if empty
 */
template<typename Tp>
struct Serializer<std::unique_ptr<Tp>>{
	std::string encode(const std::unique_ptr<Tp>& p){
		if(p==nullptr)	return "null";
		return pc::json::Make(*p);
	}
};
/**
 * @brief Specialization of `Serializer` for `std::shared_ptr`,
 * serializes the pointed value or "null" if empty
 */
template<typename Tp>
struct Serializer<std::shared_ptr<Tp>>{
	std::string encode(const std::shared_ptr<Tp>& p){
		if(p==nullptr)	return "null";
		return pc::json::Make(*p);
	}
};
} // namespace pc::json
#endif