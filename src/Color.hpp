#pragma once
#define PCL_COLOR

#include<map>
#include<cstdio>
using std::string;
using std::map;

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

Color RGB(int R,int G,int B)
{
	Color ret;
	ret.R=R,ret.G=G,ret.B=B;
	return ret;
}
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

Color Gradient(Color Col1,Color Col2,double index=0.5)
{
	Color ret;
	ret.R=int(Col1.R*index+Col2.R*(1-index));
	ret.G=int(Col1.G*index+Col2.G*(1-index));
	ret.B=int(Col1.B*index+Col2.B*(1-index));
	return ret;
}
Color HighContrust(Color col,double index=0.5)
	{return Gradient(col,0xFFFFFF,index);}

Color InvertColor(Color col)
	{return RGB(255-col.R,255-col.G,255-col.B);}

const map<string,Color> NamedColor={
	{"AliceBlue",0xF0F8FF},
	{"AntiqueWhite",0xFAEBD7},
	{"Aqua",0x00FFFF},
	{"Aquamarine",0x7FFFD4},
	{"Azure",0xF0FFFF},
	{"Beige",0xF5F5DC},
	{"Bisque",0xFFE4C4},
	{"Black",0x000000},
	{"BlanchedAlmond",0xFFEBCD},
	{"Blue",0x0000FF},
	{"BlueViolet",0x8A2BE2},
	{"Brown",0xA52A2A},
	{"BurlyWood",0xDEB887},
	{"CadetBlue",0x5F9EA0},
	{"Chartreuse",0x7FFF00},
	{"Chocolate",0xD2691E},
	{"Coral",0xFF7F50},
	{"CornflowerBlue",0x6495ED},
	{"Cornsilk",0xFFF8DC},
	{"Crimson",0xDC143C},
	{"Cyan",0x00FFFF},
	{"DarkBlue",0x00008B},
	{"DarkCyan",0x008B8B},
	{"DarkGoldenRod",0xB8860B},
	{"DarkGray",0xA9A9A9},
	{"DarkGreen",0x006400},
	{"DarkKhaki",0xBDB76B},
	{"DarkMagenta",0x8B008B},
	{"DarkOliveGreen",0x556B2F},
	{"DarkOrange",0xFF8C00},
	{"DarkOrchid",0x9932CC},
	{"DarkRed",0x8B0000},
	{"DarkSalmon",0xE9967A},
	{"DarkSeaGreen",0x8FBC8F},
	{"DarkSlateBlue",0x483D8B},
	{"DarkSlateGray",0x2F4F4F},
	{"DarkTurquoise",0x00CED1},
	{"DarkViolet",0x9400D3},
	{"DeepPink",0xFF1493},
	{"DeepSkyBlue",0x00BFFF},
	{"DimGray",0x696969},
	{"DodgerBlue",0x1E90FF},
	{"FireBrick",0xB22222},
	{"FloralWhite",0xFFFAF0},
	{"ForestGreen",0x228B22},
	{"Fuchsia",0xFF00FF},
	{"Gainsboro",0xDCDCDC},
	{"GhostWhite",0xF8F8FF},
	{"Gold",0xFFD700},
	{"GoldenRod",0xDAA520},
	{"Gray",0x808080},
	{"Green",0x008000},
	{"GreenYellow",0xADFF2F},
	{"HoneyDew",0xF0FFF0},
	{"HotPink",0xFF69B4},
	{"IndianRed ",0xCD5C5C},
	{"Indigo ",0x4B0082},
	{"Ivory",0xFFFFF0},
	{"Khaki",0xF0E68C},
	{"Lavender",0xE6E6FA},
	{"LavenderBlush",0xFFF0F5},
	{"LawnGreen",0x7CFC00},
	{"LemonChiffon",0xFFFACD},
	{"LightBlue",0xADD8E6},
	{"LightCoral",0xF08080},
	{"LightCyan",0xE0FFFF},
	{"LightGoldenRodYellow",0xFAFAD2},
	{"LightGray",0xD3D3D3},
	{"LightGreen",0x90EE90},
	{"LightPink",0xFFB6C1},
	{"LightSalmon",0xFFA07A},
	{"LightSeaGreen",0x20B2AA},
	{"LightSkyBlue",0x87CEFA},
	{"LightSlateGray",0x778899},
	{"LightSteelBlue",0xB0C4DE},
	{"LightYellow",0xFFFFE0},
	{"Lime",0x00FF00},
	{"LimeGreen",0x32CD32},
	{"Linen",0xFAF0E6},
	{"Magenta",0xFF00FF},
	{"Maroon",0x800000},
	{"MediumAquaMarine",0x66CDAA},
	{"MediumBlue",0x0000CD},
	{"MediumOrchid",0xBA55D3},
	{"MediumPurple",0x9370DB},
	{"MediumSeaGreen",0x3CB371},
	{"MediumSlateBlue",0x7B68EE},
	{"MediumSpringGreen",0x00FA9A},
	{"MediumTurquoise",0x48D1CC},
	{"MediumVioletRed",0xC71585},
	{"MidnightBlue",0x191970},
	{"MintCream",0xF5FFFA},
	{"MistyRose",0xFFE4E1},
	{"Moccasin",0xFFE4B5},
	{"NavajoWhite",0xFFDEAD},
	{"Navy",0x000080},
	{"OldLace",0xFDF5E6},
	{"Olive",0x808000},
	{"OliveDrab",0x6B8E23},
	{"Orange",0xFFA500},
	{"OrangeRed",0xFF4500},
	{"Orchid",0xDA70D6},
	{"PaleGoldenRod",0xEEE8AA},
	{"PaleGreen",0x98FB98},
	{"PaleTurquoise",0xAFEEEE},
	{"PaleVioletRed",0xDB7093},
	{"PapayaWhip",0xFFEFD5},
	{"PeachPuff",0xFFDAB9},
	{"Peru",0xCD853F},
	{"Pink",0xFFC0CB},
	{"Plum",0xDDA0DD},
	{"PowderBlue",0xB0E0E6},
	{"Purple",0x800080},
	{"Red",0xFF0000},
	{"RosyBrown",0xBC8F8F},
	{"RoyalBlue",0x4169E1},
	{"SaddleBrown",0x8B4513},
	{"Salmon",0xFA8072},
	{"SandyBrown",0xF4A460},
	{"SeaGreen",0x2E8B57},
	{"SeaShell",0xFFF5EE},
	{"Sienna",0xA0522D},
	{"Silver",0xC0C0C0},
	{"SkyBlue",0x87CEEB},
	{"SlateBlue",0x6A5ACD},
	{"SlateGray",0x708090},
	{"Snow",0xFFFAFA},
	{"SpringGreen",0x00FF7F},
	{"SteelBlue",0x4682B4},
	{"Tan",0xD2B48C},
	{"Teal",0x008080},
	{"Thistle",0xD8BFD8},
	{"Tomato",0xFF6347},
	{"Turquoise",0x40E0D0},
	{"Violet",0xEE82EE},
	{"Wheat",0xF5DEB3},
	{"White",0xFFFFFF},
	{"WhiteSmoke",0xF5F5F5},
	{"Yellow",0xFFFF00},
	{"YellowGreen",0x9ACD32}
};

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
	auto f=NamedColor.find(a);
	if(f!=NamedColor.end())
		return f->second;
	return Color(-1);
}


#define clAliceBlue Color(0xF0F8FF)
#define clAntiqueWhite Color(0xFAEBD7)
#define clAqua Color(0x00FFFF)
#define clAquamarine Color(0x7FFFD4)
#define clAzure Color(0xF0FFFF)
#define clBeige Color(0xF5F5DC)
#define clBisque Color(0xFFE4C4)
#define clBlack Color(0x000000)
#define clBlanchedAlmond Color(0xFFEBCD)
#define clBlue Color(0x0000FF)
#define clBlueViolet Color(0x8A2BE2)
#define clBrown Color(0xA52A2A)
#define clBurlyWood Color(0xDEB887)
#define clCadetBlue Color(0x5F9EA0)
#define clChartreuse Color(0x7FFF00)
#define clChocolate Color(0xD2691E)
#define clCoral Color(0xFF7F50)
#define clCornflowerBlue Color(0x6495ED)
#define clCornsilk Color(0xFFF8DC)
#define clCrimson Color(0xDC143C)
#define clCyan Color(0x00FFFF)
#define clDarkBlue Color(0x00008B)
#define clDarkCyan Color(0x008B8B)
#define clDarkGoldenRod Color(0xB8860B)
#define clDarkGray Color(0xA9A9A9)
#define clDarkGreen Color(0x006400)
#define clDarkKhaki Color(0xBDB76B)
#define clDarkMagenta Color(0x8B008B)
#define clDarkOliveGreen Color(0x556B2F)
#define clDarkOrange Color(0xFF8C00)
#define clDarkOrchid Color(0x9932CC)
#define clDarkRed Color(0x8B0000)
#define clDarkSalmon Color(0xE9967A)
#define clDarkSeaGreen Color(0x8FBC8F)
#define clDarkSlateBlue Color(0x483D8B)
#define clDarkSlateGray Color(0x2F4F4F)
#define clDarkTurquoise Color(0x00CED1)
#define clDarkViolet Color(0x9400D3)
#define clDeepPink Color(0xFF1493)
#define clDeepSkyBlue Color(0x00BFFF)
#define clDimGray Color(0x696969)
#define clDodgerBlue Color(0x1E90FF)
#define clFireBrick Color(0xB22222)
#define clFloralWhite Color(0xFFFAF0)
#define clForestGreen Color(0x228B22)
#define clFuchsia Color(0xFF00FF)
#define clGainsboro Color(0xDCDCDC)
#define clGhostWhite Color(0xF8F8FF)
#define clGold Color(0xFFD700)
#define clGoldenRod Color(0xDAA520)
#define clGray Color(0x808080)
#define clGreen Color(0x008000)
#define clGreenYellow Color(0xADFF2F)
#define clHoneyDew Color(0xF0FFF0)
#define clHotPink Color(0xFF69B4)
#define clIndianRed  Color(0xCD5C5C)
#define clIndigo  Color(0x4B0082)
#define clIvory Color(0xFFFFF0)
#define clKhaki Color(0xF0E68C)
#define clLavender Color(0xE6E6FA)
#define clLavenderBlush Color(0xFFF0F5)
#define clLawnGreen Color(0x7CFC00)
#define clLemonChiffon Color(0xFFFACD)
#define clLightBlue Color(0xADD8E6)
#define clLightCoral Color(0xF08080)
#define clLightCyan Color(0xE0FFFF)
#define clLightGoldenRodYellow Color(0xFAFAD2)
#define clLightGray Color(0xD3D3D3)
#define clLightGreen Color(0x90EE90)
#define clLightPink Color(0xFFB6C1)
#define clLightSalmon Color(0xFFA07A)
#define clLightSeaGreen Color(0x20B2AA)
#define clLightSkyBlue Color(0x87CEFA)
#define clLightSlateGray Color(0x778899)
#define clLightSteelBlue Color(0xB0C4DE)
#define clLightYellow Color(0xFFFFE0)
#define clLime Color(0x00FF00)
#define clLimeGreen Color(0x32CD32)
#define clLinen Color(0xFAF0E6)
#define clMagenta Color(0xFF00FF)
#define clMaroon Color(0x800000)
#define clMediumAquaMarine Color(0x66CDAA)
#define clMediumBlue Color(0x0000CD)
#define clMediumOrchid Color(0xBA55D3)
#define clMediumPurple Color(0x9370DB)
#define clMediumSeaGreen Color(0x3CB371)
#define clMediumSlateBlue Color(0x7B68EE)
#define clMediumSpringGreen Color(0x00FA9A)
#define clMediumTurquoise Color(0x48D1CC)
#define clMediumVioletRed Color(0xC71585)
#define clMidnightBlue Color(0x191970)
#define clMintCream Color(0xF5FFFA)
#define clMistyRose Color(0xFFE4E1)
#define clMoccasin Color(0xFFE4B5)
#define clNavajoWhite Color(0xFFDEAD)
#define clNavy Color(0x000080)
#define clOldLace Color(0xFDF5E6)
#define clOlive Color(0x808000)
#define clOliveDrab Color(0x6B8E23)
#define clOrange Color(0xFFA500)
#define clOrangeRed Color(0xFF4500)
#define clOrchid Color(0xDA70D6)
#define clPaleGoldenRod Color(0xEEE8AA)
#define clPaleGreen Color(0x98FB98)
#define clPaleTurquoise Color(0xAFEEEE)
#define clPaleVioletRed Color(0xDB7093)
#define clPapayaWhip Color(0xFFEFD5)
#define clPeachPuff Color(0xFFDAB9)
#define clPeru Color(0xCD853F)
#define clPink Color(0xFFC0CB)
#define clPlum Color(0xDDA0DD)
#define clPowderBlue Color(0xB0E0E6)
#define clPurple Color(0x800080)
#define clRed Color(0xFF0000)
#define clRosyBrown Color(0xBC8F8F)
#define clRoyalBlue Color(0x4169E1)
#define clSaddleBrown Color(0x8B4513)
#define clSalmon Color(0xFA8072)
#define clSandyBrown Color(0xF4A460)
#define clSeaGreen Color(0x2E8B57)
#define clSeaShell Color(0xFFF5EE)
#define clSienna Color(0xA0522D)
#define clSilver Color(0xC0C0C0)
#define clSkyBlue Color(0x87CEEB)
#define clSlateBlue Color(0x6A5ACD)
#define clSlateGray Color(0x708090)
#define clSnow Color(0xFFFAFA)
#define clSpringGreen Color(0x00FF7F)
#define clSteelBlue Color(0x4682B4)
#define clTan Color(0xD2B48C)
#define clTeal Color(0x008080)
#define clThistle Color(0xD8BFD8)
#define clTomato Color(0xFF6347)
#define clTurquoise Color(0x40E0D0)
#define clViolet Color(0xEE82EE)
#define clWheat Color(0xF5DEB3)
#define clWhite Color(0xFFFFFF)
#define clWhiteSmoke Color(0xF5F5F5)
#define clYellow Color(0xFFFF00)
#define clYellowGreen Color(0x9ACD32)

#include"Multinclude.hpp"