#pragma once
#define PCL_JSON

#include<string>
#include<meta>
#include<ranges>
#include<format>
#include<cctype>
#include"Meta.hpp"

namespace pc{
/// @brief Json serialization utilities using reflection of C++26
namespace json{

enum class Style{NoWrap,Tab,Space};
class Context{
private:
	std::string str;
	Style style;
	int indent=0;
	void wrapLine(){
		if(style==Style::NoWrap)
			return;
		str+='\n';
		if(style==Style::Tab)
			str+=std::string(indent,'\t');
		else if(style==Style::Space)
			str+=std::string(indent*2,' ');
	}
	void shrinkEnd(){
		while(!str.empty()
			&&isspace(static_cast<unsigned char>(str.back())))
			str.pop_back();
	}
public:
	explicit Context(Style s):style(s){}
	Context& Put(const std::string& in){
		str+=in;
		return *this;
	}
	Context& PutColon(){
		str.push_back(':');
		if(style!=Style::NoWrap)
			str.push_back(' ');
		return *this;
	}
	Context& PutComma(){
		str.push_back(',');
		wrapLine();
		return *this;
	}
	Context& PutChar(char c){
		str.push_back(c);
		return *this;
	}
	Context& PutBracket(char c){
		if(c=='{'||c=='[')
			str.push_back(c),
			indent++,
			wrapLine();
		else if(c=='}'||c==']'){
			shrinkEnd();
			if(!str.empty()&&str.ends_with(',')){
				str.pop_back();
				indent--;
				wrapLine();
				str.push_back(c);
			}else{
				str.push_back(c);
				indent--;
			}
		}
		return *this;
	}
	std::string GetStr()const{return str;}
	Style GetStyle()const{return style;}
};

/// @brief Serialize a value to JSON string
template<typename Tp>
void Serialize(const Tp& val,Context& ctx);

/// @brief Built-in serializer for std::string_view
void DoString(const std::string_view& str,Context& ctx)
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
	ctx.Put(ans);
}
/// @brief Built-in serializer for enum types, returns the identifier of the enum value
template<typename Tp> requires std::is_enum_v<Tp>
void DoEnum(const Tp& val,Context& ctx){
	ctx.Put(std::format(R"("{}")",rfl::NameOfEnum(val)));
}
/// @brief Built-in serializer for arithmetic types (int, float, bool, etc.)
template<typename Tp> requires std::is_arithmetic_v<Tp>
void DoArithmetic(const Tp& val,Context& ctx)
{
	using T=std::remove_cvref_t<Tp>;
	if constexpr(std::is_same_v<T,bool>)	// bool
		ctx.Put(val?"true":"false");
	else if constexpr(std::is_same_v<T,char>
					||std::is_same_v<T,char16_t>
					||std::is_same_v<T,char32_t>)
		ctx.Put(std::format("{}",static_cast<unsigned int>(val)));
	else	ctx.Put(std::format("{}",val));
}
/// @brief Built-in serializer for pointer types, returns "null" for nullptr
template<typename Tp>
void DoPointer(const Tp* p,Context& ctx){
	if(p==nullptr)	ctx.Put("null");
	else	Serialize(*p,ctx);
}
/// @brief Alias for `std::ranges::range`
template<typename Tp>
concept RangeType=std::ranges::range<Tp>;
/**
 * @brief Built-in serializer for iterable ranges
 * (e.g., `std::vector`, `std::list`, etc.)
 */
template<RangeType Tp>
void DoRange(const Tp& arr,Context& ctx)
{
	ctx.PutBracket('[');
	for(const auto& i:arr)
		Serialize(i,ctx),
		ctx.PutComma();
	ctx.PutBracket(']');
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
void DoAggregate(const Tp& obj,Context& ctx)
{
	ctx.PutBracket('{');
	template for(constexpr auto member:
		std::define_static_array(
			std::meta::nonstatic_data_members_of(
				^^std::remove_cvref_t<Tp>,std::meta::access_context::current()
	))){		
		ctx.Put(std::format("\"{}\"",std::meta::identifier_of(member))).PutColon();
		Serialize(obj.[:member:],ctx);
		ctx.PutComma();
	}
	ctx.PutBracket('}');
}
/// @brief Fallback serializer for unsupported types, returns the type name
template<typename Tp>
void DoUnsupported(const Tp& val,Context& ctx){
	ctx.Put(std::format(R"("[{}]")",rfl::TypenameOf(val)));
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
	void encode(const Tp& val,Context& ctx)const
	{
		using T=std::remove_cvref_t<Tp>;
		if constexpr(std::is_arithmetic_v<T>)
			DoArithmetic(val,ctx);
		else if constexpr(std::is_same_v<T,std::string>
						||std::is_same_v<T,std::string_view>)
			DoString(val,ctx);
		else if constexpr(std::is_array_v<T>
			&&std::is_same_v<std::remove_extent_t<T>,char>)
			DoString(std::string_view(val), ctx);
		else if constexpr(std::is_enum_v<T>)
			DoEnum(val,ctx);
		else if constexpr(std::is_pointer_v<T>)
			DoPointer(val,ctx);
		else if constexpr(RangeType<T>)
			DoRange(val,ctx);
		else if constexpr(std::is_class_v<T>)
			if constexpr(HasMemberToJson<T>)
				ctx.Put(val.ToJson());
			else if constexpr(AggregateType<T>)
				DoAggregate(val,ctx);
			else	DoUnsupported(val,ctx);
		else
			DoUnsupported(val,ctx);
	}
};
/// @brief Serialize a value to JSON string
template<typename Tp>
inline void Serialize(const Tp& val,Context& ctx){
	json::Serializer<std::remove_cvref_t<Tp>>{}.encode(val,ctx);
}
template<typename Tp>
inline std::string Make(const Tp& val,Style style){
	Context ctx(style);
	Serialize(val,ctx);
	return ctx.GetStr();
}

} // namespace json

/// @brief Serialize a value to JSON string
template<typename Tp>
inline std::string ToJson(const Tp& val,json::Style style=json::Style::NoWrap){
	return json::Make(val,style);
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
	void encode(const std::optional<Tp>& opt,Context& ctx)const{
		if(opt.has_value())
			Serialize(opt.value(),ctx);
		else	ctx.Put("null");
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
	void encode(const std::map<std::string,TpV>& map,Context& ctx)const{
		ctx.PutBracket('{');
		for(const auto& p:map)
			ctx.Put(std::format("\"{}\"",p.first)).PutColon(),
			Serialize(p.second,ctx),
			ctx.PutComma();
		ctx.PutBracket('}');
	}
};
/**
 * @brief Specialization of `Serializer` for `std::map<int,TpV>`,
 * serializes the map as a JSON object
 */
template<typename TpV>
struct Serializer<std::map<int,TpV>>{
	void encode(const std::map<int,TpV>& map,Context& ctx)const{
		ctx.PutBracket('{');
		for(const auto& p:map)
			ctx.Put(std::format("\"{}\"",p.first)).PutColon(),
			Serialize(p.second,ctx),
			ctx.PutComma();
		ctx.PutBracket('}');
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
	void encode(const std::variant<Tps...>& var,Context& ctx)const{
		std::visit([&](const auto& visit){
			using T=std::remove_cvref_t<decltype(visit)>;
			if constexpr(std::is_same_v<T,std::monostate>)
				ctx.Put("null");
			else	Serialize(visit,ctx);
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
	void encode(const std::pair<Tp1,Tp2>& p,Context& ctx)const{
		ctx.PutBracket('{').Put("\"first\"").PutColon();
		Serialize(p.first,ctx);
		ctx.PutComma().Put("\"second\"").PutColon();
		Serialize(p.second,ctx);
		ctx.PutBracket('}');
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
	void encode(const std::tuple<Tps...>& tup,Context& ctx)const{
		ctx.PutBracket('[');
		std::apply([&](const auto&... items){
			auto append=[&](const auto& item){
				Serialize(item,ctx);
				ctx.PutComma();
			};
			(append(items),...);
		},tup);
		ctx.PutBracket(']');
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
	void encode(const std::unique_ptr<Tp>& p,Context& ctx)const{
		if(p==nullptr)	ctx.Put("null");
		else	Serialize(*p,ctx);
	}
};
/**
 * @brief Specialization of `Serializer` for `std::shared_ptr`,
 * serializes the pointed value or "null" if empty
 */
template<typename Tp>
struct Serializer<std::shared_ptr<Tp>>{
	void encode(const std::shared_ptr<Tp>& p,Context& ctx)const{
		if(p==nullptr)	ctx.Put("null");
		else	Serialize(*p,ctx);
	}
};
} // namespace pc::json
#endif