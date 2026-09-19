#pragma once
#define PCL_META

#include<meta>
#include<string>

namespace pc{
namespace meta{

template<typename Tp> 
inline std::string_view TypenameOf(Tp value){
	return std::meta::display_string_of(std::meta::type_of(^^value));
}
template<typename Tp>
constexpr std::string_view Typename=std::meta::display_string_of(^^Tp);
template<typename Enum> requires std::is_enum_v<Enum>
std::string NameOfEnum(Enum value){
    template for(constexpr auto e:std::define_static_array(std::meta::enumerators_of(^^Enum)))
        if(value==[:e:])
            return std::string(std::meta::identifier_of(e));
    return std::to_string(static_cast<std::underlying_type_t<Enum>>(value));
}

} // namespace meta
} // namespace pc
