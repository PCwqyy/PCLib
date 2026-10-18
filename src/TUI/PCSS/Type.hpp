#pragma once
#define PCL_TUI_PCSS_TYPE

/// @todo Fix Enum look up

#include"Utility/StrUtils.hpp"
#include"Utility/EnumLookup.hpp"
#include"Container/Color.hpp"
#include"Util.hpp"

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
				throw pc::Exception(pc::xpt::INVALID_ARGUMENT,
					"invalid PCSS identifier '{}'",str);
			identifier=pc::su::Trim(str);
		}
	};
	struct Number{
		double number;
		Number(string str){
			str=pc::su::Trim(str);
			try{
				number=pc::su::ExtractDouble(str);
			}catch(std::invalid_argument){
				throw pc::Exception(pc::xpt::INVALID_ARGUMENT,
					"trying to parse '{}' into a PCSS number value",str);
			}
		}
	};
	struct Length: public Value{
		double number;
		Unit::Length unit;
		Length(string a){
			number=pc::su::ExtractDouble(a);
			unit=Unit::LengthLookup(pc::su::ToLowercase(a));
		}
	};
	struct Angle: public Value{
		double number;
		Unit::Angle unit;
		Angle(string a){
			number=pc::su::ExtractDouble(a);
			unit=Unit::AngleLookup(pc::su::ToLowercase(a));
		}
	};
	struct Color: public Value{
		pc::Color color;
		Color(string a):color(a){}
	};

	enum class Types{Identifier,Number,Length,Angle,Color};
	EnumLookup<Types> TypesLookup("identifier","number","length","angle","color");
} // namespace Type

}// namespace PCSS