#pragma once
#define PCL_JSON

#include<string>
#include<span>
#include<meta>
#include<ranges>
#include<format>
#include"Meta.hpp"

namespace pc{
namespace json{

template<typename Tp>
std::string Make(const Tp& val);

std::string String(const std::string_view& str)
{
	std::string ans="\"";
	ans.reserve(str.size()+4);
	for(char c:str)
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
					ans+=std::format("\\u{:04x}",c);
				else	ans+=c;
		};
	ans+="\"";
	return ans;
}
template<typename Tp> requires std::is_enum_v<Tp>
std::string Enum(const Tp& val){
	return std::format(R"("{}")",rfl::NameOfEnum(val));
}
template<typename Tp> requires std::is_arithmetic_v<Tp>
std::string Arithmetic(const Tp& val)
{
	using T=std::remove_cvref_t<Tp>;
	if constexpr(std::is_same_v<T,bool>)	// bool
		return val?"true":"false";
	else	return std::to_string(val);
}
template<std::ranges::range Tp>
std::string Iterable(const Tp& arr)
{
	std::string ans="[";
	for(auto i:arr)
		ans+=Make(i)+",";
	if(ans.size()>1)
		ans.back()=']';
	else	ans.push_back(']');
	return ans;
}
template<typename Tp> requires std::is_aggregate_v<Tp>
std::string Object(const Tp& obj)
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
template<typename Tp>
std::string Unsupported(const Tp& val){
	return std::format(R"("[{}]")",rfl::TypenameOf(val));
}

template<typename Tp>
std::string Make(const Tp& val)
{
	using T=std::remove_cvref_t<Tp>;
	if constexpr(std::is_arithmetic_v<T>)
		return Arithmetic(val);
	else if constexpr(std::is_same_v<T,std::string>
					||std::is_same_v<T,std::string_view>)
		return String(val);
	else if constexpr(std::is_enum_v<T>)
		return Enum(val);
	else if constexpr(std::is_class_v<T>)
		if constexpr(std::ranges::range<T>)
			return Iterable(val);
		else if constexpr(std::is_aggregate_v<T>)
			return Object(val);
		else	return Unsupported(val);
	else
		return Unsupported(val);
}
} // namespace json
template<typename Tp>
inline std::string ToJson(const Tp& val){
	return json::Make(val);
}

} // namespace pc
