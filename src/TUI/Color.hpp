#pragma once
#define PCL_COLOR

#include<map>
#include<cstdio>
#include<string>
using std::string;
using std::map;

/// @brief The Color class.
struct Color
{
	int R,G,B;
	Color():
		R(0),G(0),B(0){}
	Color(int col,bool digit3=false)
	{
		if(col==-1)	R=-1;
		else if(digit3)
		{
			R=(col&0xF00)>>8;
			G=(col&0x0F0)>>4;
			B=(col&0x00F);
			R=(R<<4)+R;
			G=(G<<4)+G;
			B=(B<<4)+B;
		}
		else
		{
			R=(col&0xFF0000)>>16;
			G=(col&0x00FF00)>>8;
			B=(col&0x0000FF);
		}
	}
	int toInt(){return B|(G<<8)|(R<<16);}
	bool DontModify(){return R==-1;}
};

#undef RGB
#undef HSL

/**
 * @brief Make `Color` with RGB.
 * @param R Red
 * @param G Green
 * @param B Blue
 */
Color RGB(int R,int G,int B)
{
	Color ret;
	ret.R=R,ret.G=G,ret.B=B;
	return ret;
}
/**
 * @brief Make `Color` with HSL.
 * @param H Hue
 * @param S Saturation
 * @param L Lightness
 */
Color HSL(int H,int S,int L)
{
	double r,g,b;
	double h=H/360.0;
	double s=S/100.0;
	double l=L/100.0;
	auto hue2rgb=[](double p,double q,double t)
	{
		if(t<0)	t++;
		if(t>1)	t--;
		if(t<1.0/6)	return p+(q-p)*6*t;
		if(t<1.0/2)	return q;
		if(t<2.0/3)	return p+(q-p)*(2.0/3-t)*6;
		return p;
	};
	if(s==0)	r=g=b=l;
	else
	{
		double q=l<0.5?l*(1+s):l+s-l*s;
		double p=2*l-q;
		r=hue2rgb(p,q,h+1.0/3);
		g=hue2rgb(p,q,h);
		b=hue2rgb(p,q,h-1.0/3);
	}
	Color ret;
	ret.R=int(r*255);
	ret.G=int(g*255);
	ret.B=int(b*255);
	return ret;
}

/**
 * @brief Calculate the average color the two color.
 * @param index Position of the middle color.
 * ```
 * Col1 0 ---------+--------- 1 Col2
 * ```
 */
Color Gradient(Color Col1,Color Col2,double index=0.5)
{
	Color ret;
	ret.R=int(Col1.R*index+Col2.R*(1-index));
	ret.G=int(Col1.G*index+Col2.G*(1-index));
	ret.B=int(Col1.B*index+Col2.B*(1-index));
	return ret;
}

/** @brief Get highlight version of a color.
 *  The bigger `index` is, the whiter the result is. */
Color HighLightColor(Color col,double index=0.5)
	{return Gradient(col,0xFFFFFF,index);}
/// @brief Invert the color.
Color InvertColor(Color col)
	{return RGB(255-col.R,255-col.G,255-col.B);}

/** @brief All the HTML named color.
 *  @note See https://developer.mozilla.org/en-US/docs/Web/CSS/named-color
*/
const map<string,Color> NamedColor=
{// cSpell: disable
	{"aliceblue",0xF0F8FF},
	{"antiquewhite",0xFAEBD7},
	{"aqua",0x00FFFF},
	{"aquamarine",0x7FFFD4},
	{"azure",0xF0FFFF},
	{"beige",0xF5F5DC},
	{"bisque",0xFFE4C4},
	{"black",0x000000},
	{"blanchedalmond",0xFFEBCD},
	{"blue",0x0000FF},
	{"blueviolet",0x8A2BE2},
	{"brown",0xA52A2A},
	{"burlywood",0xDEB887},
	{"cadetblue",0x5F9EA0},
	{"chartreuse",0x7FFF00},
	{"chocolate",0xD2691E},
	{"coral",0xFF7F50},
	{"cornflowerBlue",0x6495ED},
	{"cornsilk",0xFFF8DC},
	{"crimson",0xDC143C},
	{"cyan",0x00FFFF},
	{"darkblue",0x00008B},
	{"darkcyan",0x008B8B},
	{"darkgoldenrod",0xB8860B},
	{"darkgray",0xA9A9A9},
	{"darkgreen",0x006400},
	{"darkkhaki",0xBDB76B},
	{"darkmagenta",0x8B008B},
	{"darkolivegreen",0x556B2F},
	{"darkorange",0xFF8C00},
	{"darkorchid",0x9932CC},
	{"darkred",0x8B0000},
	{"darksalmon",0xE9967A},
	{"darkseagreen",0x8FBC8F},
	{"darkslateblue",0x483D8B},
	{"darkslategray",0x2F4F4F},
	{"darkturquoise",0x00CED1},
	{"darkviolet",0x9400D3},
	{"deepPink",0xFF1493},
	{"deepskyblue",0x00BFFF},
	{"dimgray",0x696969},
	{"dodgerblue",0x1E90FF},
	{"firebrick",0xB22222},
	{"floralwhite",0xFFFAF0},
	{"forestgreen",0x228B22},
	{"fuchsia",0xFF00FF},
	{"gainsboro",0xDCDCDC},
	{"ghostwhite",0xF8F8FF},
	{"gold",0xFFD700},
	{"goldenrod",0xDAA520},
	{"gray",0x808080},
	{"green",0x008000},
	{"greenyellow",0xADFF2F},
	{"honeydew",0xF0FFF0},
	{"hotpink",0xFF69B4},
	{"indianred ",0xCD5C5C},
	{"indigo",0x4B0082},
	{"ivory",0xFFFFF0},
	{"khaki",0xF0E68C},
	{"lavender",0xE6E6FA},
	{"lavenderblush",0xFFF0F5},
	{"lawngreen",0x7CFC00},
	{"lemonchiffon",0xFFFACD},
	{"lightblue",0xADD8E6},
	{"lightcoral",0xF08080},
	{"lightcyan",0xE0FFFF},
	{"lightgoldenrodyellow",0xFAFAD2},
	{"lightgray",0xD3D3D3},
	{"lightgreen",0x90EE90},
	{"lightpink",0xFFB6C1},
	{"lightsalmon",0xFFA07A},
	{"lightseaGreen",0x20B2AA},
	{"lightskyblue",0x87CEFA},
	{"lightslategray",0x778899},
	{"lightsteelblue",0xB0C4DE},
	{"lightyellow",0xFFFFE0},
	{"lime",0x00FF00},
	{"limegreen",0x32CD32},
	{"linen",0xFAF0E6},
	{"magenta",0xFF00FF},
	{"maroon",0x800000},
	{"mediumaquamarine",0x66CDAA},
	{"mediumblue",0x0000CD},
	{"mediumorchid",0xBA55D3},
	{"mediumpurple",0x9370DB},
	{"mediumseagreen",0x3CB371},
	{"mediumslateblue",0x7B68EE},
	{"mediumspringgreen",0x00FA9A},
	{"mediumturquoise",0x48D1CC},
	{"mediumvioletred",0xC71585},
	{"midnightblue",0x191970},
	{"mintcream",0xF5FFFA},
	{"mistyrose",0xFFE4E1},
	{"moccasin",0xFFE4B5},
	{"navajowhite",0xFFDEAD},
	{"navy",0x000080},
	{"oldLace",0xFDF5E6},
	{"olive",0x808000},
	{"olivedrab",0x6B8E23},
	{"orange",0xFFA500},
	{"orangered",0xFF4500},
	{"orchid",0xDA70D6},
	{"palegoldenRod",0xEEE8AA},
	{"palegreen",0x98FB98},
	{"paleturquoise",0xAFEEEE},
	{"palevioletred",0xDB7093},
	{"papayawhip",0xFFEFD5},
	{"peachpuff",0xFFDAB9},
	{"peru",0xCD853F},
	{"pink",0xFFC0CB},
	{"plum",0xDDA0DD},
	{"powderblue",0xB0E0E6},
	{"purple",0x800080},
	{"red",0xFF0000},
	{"rosybrown",0xBC8F8F},
	{"royalblue",0x4169E1},
	{"saddlebrown",0x8B4513},
	{"salmon",0xFA8072},
	{"sandyBrown",0xF4A460},
	{"seagreen",0x2E8B57},
	{"seashell",0xFFF5EE},
	{"sienna",0xA0522D},
	{"silver",0xC0C0C0},
	{"skyblue",0x87CEEB},
	{"slateblue",0x6A5ACD},
	{"slategray",0x708090},
	{"snow",0xFFFAFA},
	{"springgreen",0x00FF7F},
	{"steelblue",0x4682B4},
	{"tan",0xD2B48C},
	{"teal",0x008080},
	{"thistle",0xD8BFD8},
	{"tomato",0xFF6347},
	{"turquoise",0x40E0D0},
	{"violet",0xEE82EE},
	{"wheat",0xF5DEB3},
	{"white",0xFFFFFF},
	{"whitesmoke",0xF5F5F5},
	{"yellow",0xFFFF00},
	{"yellowgreen",0x9ACD32}
};// cSpell: enable
namespace pcpri
{
	string toLowerCase(string a)
	{
		string res;
		for(char i:a)
			res+=isupper(i)?i-'A'+'a':i;
		return res;
	}
};
/// @brief Get named color. Case insensitive. 
Color GetColorByName(string n)
{
	n=pcpri::toLowerCase(n);
	auto f=NamedColor.find(n);
	if(f!=NamedColor.end())
		return f->second;
	return Color(-1);
}

/**
 * @brief Convert a string to color.
 * Accepts 3-digit hex, 6-digit hex and named color.
 * @example
 * ```cpp
 * StrintToColor("#333");
 * StrintToColor("#123456");
 * StrintToColor("red");
 * ```
 */
Color StringToColor(string a)
{
	if(a[0]=='#')
	{
		int len=a.length();
		if(len!=4&&len!=7)
			return Color(-1);
		int col;
		sscanf(a.c_str()+1,"%x",&col);
		if(len==4)
			return Color(col,true);
		return Color(col);
	}
	return GetColorByName(a);
}

#include"../Multinclude.hpp"