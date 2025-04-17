#pragma once
#define PCL_TUI_ELEMENT

#include"SytleSheet.hpp"

class Element
{
protected:
	StyleSheet style;
	short left,top,height,width;
public:
	virtual pcpri::COORD Print(short x,short y,short visWid=-1)
	{
		left=x,top=y;
		return {left,top};
	}
	Element()
		{height=0,width=0,left=0,top=0;}
	virtual ~Element(){}
};