#pragma once
#define PCL_TUI_PCSS_TYPE

#include"../../Utility/StrUtils.hpp"
#include"../Util.hpp"

#include<string>
#include<vector>
#include<map>
using std::string;
using std::vector;

namespace PCSS{

namespace Unit{
	/**
	 * @brief Map a string to an enum type, 
	 * quickly looking up corresponding enum value for a string
	 * @warning The order of string must matches with enum declaration
	 * @note Use like this: `Unitmapping["name"]`;
	 * @tparam Enum The enum class
	 */
	template<typename Enum>
	class UnitNameLookup
	{
	private:
		vector<string> map;
		std::optional<Enum> get(string name) const
		{
			name=su::ToLowercase(name);
			int i=0;
			for(string s:map)
				if(name==s)
					return std::optional<Enum>(static_cast<Enum>(i));
				else	i++;
			return std::nullopt;
		}
	public:
		template<typename ...Tps>
		UnitNameLookup(Tps&& ...args){
			(map.push_back(std::forward<Tps>(string(args))),...);
		}
		Enum operator()(string name) const {
			return get(name).value_or(static_cast<Enum>(0));
		}
	};
	enum class Length{px,ch,vw,vh};
	UnitNameLookup<Length> LengthLookup("px","ch","vw","vh");
	enum class Angle{deg,rad};
	UnitNameLookup<Angle> AngleLookup("deg","rad");
} // namespace Unit

namespace Type{
	struct Number{
		double number;
		Number():number(0){}
	};
	struct Length: public Number{
		Unit::Length unit;
		Length(string a){
			number=su::ExtractDouble(a);
			unit=Unit::LengthLookup(su::ToLowercase(a));
		}
	};
	struct Angle: public Number{
		Unit::Angle unit;
		Angle(string a){
			number=su::ExtractDouble(a);
			unit=Unit::AngleLookup(su::ToLowercase(a));
		}
	};
} // namespace Type

}// namespace PCSS