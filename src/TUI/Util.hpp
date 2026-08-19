#pragma once
#define PCL_TUI_UTIL

#include<string>
#include<ctime>
#include<random>
#include<set>
#include<map>
using std::string;
using std::set;
using std::map;

#include"../Exception.hpp"

/// @brief Coordinate struct
struct Coord
{
	short x,y;
	Coord Offset(short dx,short dy)
	{
		x+=dx,y+=dy;
		return *this;
	}
	Coord Set(short nx,short ny)
	{
		x=nx,y=ny;
		return *this;
	}
	Coord operator+(const Coord& a)
		{return {short(x+a.x),short(y+a.y)};}
	Coord operator-(const Coord& a)
		{return {short(x-a.x),short(y-a.y)};}
	bool operator==(const Coord& a) const
		{return x==a.x&&y==a.y;}
	bool operator!=(const Coord& a) const
		{return x!=a.x||y!=a.y;}
};

/// @brief Move cursor to horizontal `x` and vertical `y`
inline void CursorGoto(Coord pos)
	{std::print("\e[{};{}H",pos.y+1,pos.x+1);return;}
/// @brief Offset the cursor horizontally by `x` and vertically by `y`
inline void CursorOffset(Coord delta)
{
	char op;
	if(delta.x!=0)
	{
		if(delta.x>0)			op='C';
		else if(delta.x<0)	op='D',delta.x=-delta.x;
		std::print("\e[{}{}",delta.x,op);
	}
	if(delta.y!=0)
	{
		if(delta.y>0)			op='B';
		else if(delta.y<0)	op='A',delta.y=-delta.y;
		std::print("\e[{}{}",delta.y,op);
	}
}

/// @brief Utilitys for TUI
namespace util
{

/// @brief Generate an UUID 
string GenUUID()
{
	char uuid[37];
    std::mt19937 rng(
		std::random_device{}()^
		static_cast<unsigned>(std::time(nullptr))
	);
    std::uniform_int_distribution<unsigned char> dist(0, 255);
    unsigned char bytes[16];
    for (auto& byte : bytes) byte = dist(rng);
    bytes[6]=(bytes[6] & 0x0F)|0x40; //version 4
    bytes[8]=(bytes[8] & 0x3F)|0x80; //variant 1
    std::snprintf(uuid,sizeof(uuid),
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        bytes[0],bytes[1],bytes[2],bytes[3],
        bytes[4],bytes[5],bytes[6],bytes[7],
        bytes[8],bytes[9],bytes[10],bytes[11],
        bytes[12],bytes[13],bytes[14],bytes[15]);
    return string(uuid);
}

/// @brief If is a valid var name 
inline bool CheckNameValid(string name)
{
	if(isdigit(name[0]))	return false;
	bool hasAlp=false;
	for(char i:name)
		if(isalpha(i)||i=='_')
			hasAlp=true;
		else if(isdigit(i));
		else return false;
	return hasAlp;
}
/// @brief If is a valid tag name 
inline bool CheckTagValid(string name)
{
	for(char i:name)
		if(!isalpha(i))
			return false;
	return true;
}

}//namespace