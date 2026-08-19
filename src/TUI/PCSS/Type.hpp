#pragma once
#define PCL_TUI_PCSS_TYPE

#include"../../Utility/StrUtils.hpp"
#include"../../Utility/EnumLookup.hpp"
#include"../../Container/Color.hpp"
#include"../Util.hpp"

#include<string>
#include<vector>
#include<map>
using std::string;
using std::vector;

namespace PCSS{

namespace Unit{
	enum class Length{px,ch,vw,vh};
	EnumLookup<Length> LengthLookup("px","ch","vw","vh");
	enum class Angle{deg,rad};
	EnumLookup<Angle> AngleLookup("deg","rad");
} // namespace Unit

namespace Type{
	struct Value{};
	struct Identifier: public Value{
		string identifier;
		Identifier(string str){
			if(!util::CheckNameValid(str))
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"invalid PCSS identifier '{}'",str);
			identifier=su::Trim(str);
		}
	};
	struct Number{
		double number;
		Number(string str){
			str=su::Trim(str);
			try{
				number=su::ExtractDouble(str);
			}catch(std::invalid_argument){
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"trying to parse '{}' into a PCSS number value",str);
			}
		}
	};
	struct Length: public Value{
		double number;
		Unit::Length unit;
		Length(string a){
			number=su::ExtractDouble(a);
			unit=Unit::LengthLookup(su::ToLowercase(a));
		}
	};
	struct Angle: public Value{
		double number;
		Unit::Angle unit;
		Angle(string a){
			number=su::ExtractDouble(a);
			unit=Unit::AngleLookup(su::ToLowercase(a));
		}
	};
	struct Color: public Value{
		::Color color;
		Color(string a):color(a){}
	};

	enum class Types{Identifier,Number,Length,Angle,Color};
	EnumLookup<Types> TypesLookup("identifier","number","length","angle","color");
} // namespace Type

}// namespace PCSS