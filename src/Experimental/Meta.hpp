#pragma once
#define PCL_META

#include<meta>
#include<string>

namespace pc{
namespace reflection{

/// @brief Get the typename `std::string_view` of a variable statically 
template<typename Tp> 
inline std::string_view TypenameOf(Tp value){
	return std::meta::display_string_of(std::meta::type_of(^^value));
}
/// @brief Get the name `std::string_view` of a type statically
template<typename Tp>
constexpr std::string_view Typename=std::meta::display_string_of(^^Tp);

/// @brief Get the name (identifier) of an enum value
template<typename Enum> requires std::is_enum_v<Enum>
std::string NameOfEnum(Enum value){
	template for(constexpr auto e:std::define_static_array(std::meta::enumerators_of(^^Enum)))
		if(value==[:e:])
			return std::string(std::meta::identifier_of(e));
	return std::to_string(static_cast<std::underlying_type_t<Enum>>(value));
}
/// @brief Get the full name of an enum value, including the enum type name
template<typename Enum> requires std::is_enum_v<Enum>
std::string FullNameOfEnum(Enum value){
	std::string name(Typename<Enum>);
	return name+"::"+NameOfEnum(value);
}

} // namespace reflection
namespace rfl=reflection; 
} // namespace pc
