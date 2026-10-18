#pragma once
#define PCL_OBJECT

#include<vector>
#include<variant>
#include<format>
#include<map>
#include<memory>
#include<string>
#include<optional>
#include"Exception.hpp"
#include"Utility/StrUtils.hpp"

namespace pc{
namespace json{
		
const pc::Exceptioner xptJSON("Json");

/// @brief The style of JSON output, controlling whitespace and indentation
enum class Style{NoWrap,Tab,Space};
/// @brief Context for JSON serialization, managing the output string and formatting
class Context{
private:
	std::string str;
	Style style;
	int indent=0;
	// @brief Wraps the line according to the current style and indentation
	void wrapLine(){
		if(style==Style::NoWrap)
			return;
		str+='\n';
		if(style==Style::Tab)
			str+=std::string(indent,'\t');
		else if(style==Style::Space)
			str+=std::string(indent*2,' ');
	}
	/// @brief Removes trailing whitespace from the output string
	void shrinkEnd(){
		while(!str.empty()
			&&isspace(static_cast<unsigned char>(str.back())))
			str.pop_back();
	}
public:
	explicit Context(Style s):style(s){}
	/// @brief Appends a string to the output
	Context& Put(const std::string& in){
		str+=in;
		return *this;
	}
	/// @brief Appends a colon to the output, with optional space based on style
	Context& PutColon(){
		str.push_back(':');
		if(style!=Style::NoWrap)
			str.push_back(' ');
		return *this;
	}
	/// @brief Appends a comma to the output, with optional line wrapping based on style
	Context& PutComma(){
		str.push_back(',');
		wrapLine();
		return *this;
	}
	/// @brief Appends a character to the output
	Context& PutChar(char c){
		str.push_back(c);
		return *this;
	}
	/**
	 * @brief Appends a bracket (`{}[]`) to the output,
	 * and manages indentation and line wrapping accordingly
	 */
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
	/// @brief Returns the current output string
	std::string GetStr()const{return str;}
	/// @brief Returns the current style of the context
	Style GetStyle()const{return style;}
};


class Value;
using Object=std::map<std::string,Value>;
using Array=std::vector<Value>;
using RawType=std::variant<
	std::nullptr_t,
	bool,
	double,
	std::string,
	std::unique_ptr<Array>,
	std::unique_ptr<Object>
>;

class Value{
	RawType data;
public:
	Value():data(nullptr){}
	Value(std::nullptr_t):data(nullptr){}
	Value(bool boolean):data(boolean){}
	Value(int integer):data(double(integer)){}
	Value(long long longlong):data(double(longlong)){}
	Value(double number):data(number){}
	Value(std::string str):data(std::move(str)){}
	Value(const char* str):data(std::string(str)){}
	Value(std::unique_ptr<Array> array):data(std::move(array)){}
	Value(std::unique_ptr<Object> object):data(std::move(object)){}

	Value(const Value& other){
		std::visit([&](const auto& x){
			using T=std::remove_cvref_t<decltype(x)>;
			if constexpr (std::is_same_v<T,std::unique_ptr<Array>>)
				data=std::make_unique<Array>(*x);
			else if constexpr (std::is_same_v<T,std::unique_ptr<Object>>)
				data=std::make_unique<Object>(*x);
			else	data=x;
		},other.data);
	}
	Value(Value&&)=default;
	Value& operator=(Value&&)=default;
	Value& operator=(const Value& other){
		if(this!=&other){
			Value tmp(other);
			data=std::move(tmp.data);
		}
		return *this;
	}

	bool IsNull()	const{return std::holds_alternative<std::nullptr_t>(data);}
	bool IsBool()	const{return std::holds_alternative<bool>(data);}
	bool IsNumber()	const{return std::holds_alternative<double>(data);}
	bool IsString()	const{return std::holds_alternative<std::string>(data);}
	bool IsArray()	const{return std::holds_alternative<std::unique_ptr<Array>>(data);}
	bool IsObject()	const{return std::holds_alternative<std::unique_ptr<Object>>(data);}

	template<typename Tp>
	Tp& As(){
		try{
			return std::get<Tp>(data);
		}catch(std::bad_variant_access& xpt){
			xptJSON.Throw(xpt::BAD_CAST,
				"Invalid convertion from json value to target type\n"
				"std::bad_variant_access::what()= {}",xpt.what());
		}
	}
	template<typename Tp>
	const Tp& As()const{
		try{
			return std::get<Tp>(data);
		}catch(std::bad_variant_access& xpt){
			xptJSON.Throw(xpt::BAD_CAST,
				"Invalid convertion from json value to const target type\n"
				"std::bad_variant_access::what()= {}",xpt.what());
		}
	}
	bool& AsBool(){return As<bool>();}
	double& AsNumber(){return As<double>();}
	std::string& AsString(){return As<std::string>();}
	Array& AsArray(){return *As<std::unique_ptr<Array>>();}
	Object& AsObject(){return *As<std::unique_ptr<Object>>();}
	const bool& AsBool()const{return As<bool>();}
	const double& AsNumber()const{return As<double>();}
	const std::string& AsString()const{return As<std::string>();}
	const Array& AsArray()const{return *As<std::unique_ptr<Array>>();}
	const Object& AsObject()const{return *As<std::unique_ptr<Object>>();}

	Value& operator[](int index){
		if(IsArray())
			return AsArray()[index];
		if(IsObject())
			return operator[](std::format("{}",index));
		else
			xptJSON.Throw(xpt::BAD_CAST,
				"The json value isn't holding an array or an object, "
				"but operator[{}] is called",index
			);
	}
	Value& operator[](std::string_view key){
		if(IsObject()){
			Object& obj=AsObject();
			auto it=obj.find(std::string(key));
			if(it==obj.end())
				xptJSON.Throw(xpt::NOT_FOUND,
					"Key '{}' dose not exist in the json object",key);
			else
				return it->second;
		}
		else
			xptJSON.Throw(xpt::BAD_CAST,
				"The json value isn't holding an object, "
				"but operator[\"{}\"] is called",key
			);
	}
	const RawType& RawData()const{return data;}

	void FormatTo(Context& ctx)const{
		if(IsNull())	ctx.Put("null");
		else if(IsBool())
			ctx.Put(AsBool()?"true":"false");
		else if(IsNumber())
			ctx.Put(std::format("{}",AsNumber()));
		else if(IsString())
			ctx.Put(su::Escape(AsString()));
		else if(IsArray()){
			const Array& arr=AsArray();
			ctx.PutBracket('[');
			for(auto& i:arr)
				i.FormatTo(ctx),
				ctx.PutComma();
			ctx.PutBracket(']');
		}
		else if(IsObject()){
			const Object& obj=AsObject();
			ctx.PutBracket('{');
			for(auto& p:obj)
				ctx.Put(std::format("\"{}\"",p.first)).PutColon(),
				p.second.FormatTo(ctx),
				ctx.PutComma();
			ctx.PutBracket('}');
		}
	}
	std::string ToJson(Style style=Style::NoWrap)const{
		Context ctx(style);
		FormatTo(ctx);
		return ctx.GetStr();
	}
};


} // namespace json
} // namespace pc