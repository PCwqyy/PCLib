#pragma once
#define PCL_TUI_ELEMENT

#include"SytleSheet.hpp"

class Element
{
protected:
	StyleSheet style;
public:
	pcpri::COORD Print(short left,short top,short visWid=-1)
		{return {left,top};}
};