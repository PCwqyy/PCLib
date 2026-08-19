#pragma once
#define PCL_COLOR

#include<map>
#include<cstdio>
#include<string>
#include<tuple>
#include<cmath>
#include<algorithm>
using std::string;
using std::tuple;

#include"../Exception.hpp"
#include"../Utility/StrUtils.hpp"


/// @brief The Color class.
class Color
{
public:
	int R,G,B;
private:
	class NamedColorTp{
		std::map<string,int> map;
	public:
		NamedColorTp()
		{// cSpell: disable
			Add("transparent",-1);
			Add("aliceblue",0xF0F8FF);
			Add("antiquewhite",0xFAEBD7);
			Add("aqua",0x00FFFF);
			Add("aquamarine",0x7FFFD4);
			Add("azure",0xF0FFFF);
			Add("beige",0xF5F5DC);
			Add("bisque",0xFFE4C4);
			Add("black",0x000000);
			Add("blanchedalmond",0xFFEBCD);
			Add("blue",0x0000FF);
			Add("blueviolet",0x8A2BE2);
			Add("brown",0xA52A2A);
			Add("burlywood",0xDEB887);
			Add("cadetblue",0x5F9EA0);
			Add("chartreuse",0x7FFF00);
			Add("chocolate",0xD2691E);
			Add("coral",0xFF7F50);
			Add("cornflowerBlue",0x6495ED);
			Add("cornsilk",0xFFF8DC);
			Add("crimson",0xDC143C);
			Add("cyan",0x00FFFF);
			Add("darkblue",0x00008B);
			Add("darkcyan",0x008B8B);
			Add("darkgoldenrod",0xB8860B);
			Add("darkgray",0xA9A9A9);
			Add("darkgreen",0x006400);
			Add("darkkhaki",0xBDB76B);
			Add("darkmagenta",0x8B008B);
			Add("darkolivegreen",0x556B2F);
			Add("darkorange",0xFF8C00);
			Add("darkorchid",0x9932CC);
			Add("darkred",0x8B0000);
			Add("darksalmon",0xE9967A);
			Add("darkseagreen",0x8FBC8F);
			Add("darkslateblue",0x483D8B);
			Add("darkslategray",0x2F4F4F);
			Add("darkturquoise",0x00CED1);
			Add("darkviolet",0x9400D3);
			Add("deepPink",0xFF1493);
			Add("deepskyblue",0x00BFFF);
			Add("dimgray",0x696969);
			Add("dodgerblue",0x1E90FF);
			Add("firebrick",0xB22222);
			Add("floralwhite",0xFFFAF0);
			Add("forestgreen",0x228B22);
			Add("fuchsia",0xFF00FF);
			Add("gainsboro",0xDCDCDC);
			Add("ghostwhite",0xF8F8FF);
			Add("gold",0xFFD700);
			Add("goldenrod",0xDAA520);
			Add("gray",0x808080);
			Add("green",0x008000);
			Add("greenyellow",0xADFF2F);
			Add("honeydew",0xF0FFF0);
			Add("hotpink",0xFF69B4);
			Add("indianred ",0xCD5C5C);
			Add("indigo",0x4B0082);
			Add("ivory",0xFFFFF0);
			Add("khaki",0xF0E68C);
			Add("lavender",0xE6E6FA);
			Add("lavenderblush",0xFFF0F5);
			Add("lawngreen",0x7CFC00);
			Add("lemonchiffon",0xFFFACD);
			Add("lightblue",0xADD8E6);
			Add("lightcoral",0xF08080);
			Add("lightcyan",0xE0FFFF);
			Add("lightgoldenrodyellow",0xFAFAD2);
			Add("lightgray",0xD3D3D3);
			Add("lightgreen",0x90EE90);
			Add("lightpink",0xFFB6C1);
			Add("lightsalmon",0xFFA07A);
			Add("lightseaGreen",0x20B2AA);
			Add("lightskyblue",0x87CEFA);
			Add("lightslategray",0x778899);
			Add("lightsteelblue",0xB0C4DE);
			Add("lightyellow",0xFFFFE0);
			Add("lime",0x00FF00);
			Add("limegreen",0x32CD32);
			Add("linen",0xFAF0E6);
			Add("magenta",0xFF00FF);
			Add("maroon",0x800000);
			Add("mediumaquamarine",0x66CDAA);
			Add("mediumblue",0x0000CD);
			Add("mediumorchid",0xBA55D3);
			Add("mediumpurple",0x9370DB);
			Add("mediumseagreen",0x3CB371);
			Add("mediumslateblue",0x7B68EE);
			Add("mediumspringgreen",0x00FA9A);
			Add("mediumturquoise",0x48D1CC);
			Add("mediumvioletred",0xC71585);
			Add("midnightblue",0x191970);
			Add("mintcream",0xF5FFFA);
			Add("mistyrose",0xFFE4E1);
			Add("moccasin",0xFFE4B5);
			Add("navajowhite",0xFFDEAD);
			Add("navy",0x000080);
			Add("oldLace",0xFDF5E6);
			Add("olive",0x808000);
			Add("olivedrab",0x6B8E23);
			Add("orange",0xFFA500);
			Add("orangered",0xFF4500);
			Add("orchid",0xDA70D6);
			Add("palegoldenRod",0xEEE8AA);
			Add("palegreen",0x98FB98);
			Add("paleturquoise",0xAFEEEE);
			Add("palevioletred",0xDB7093);
			Add("papayawhip",0xFFEFD5);
			Add("peachpuff",0xFFDAB9);
			Add("peru",0xCD853F);
			Add("pink",0xFFC0CB);
			Add("plum",0xDDA0DD);
			Add("powderblue",0xB0E0E6);
			Add("purple",0x800080);
			Add("red",0xFF0000);
			Add("rosybrown",0xBC8F8F);
			Add("royalblue",0x4169E1);
			Add("saddlebrown",0x8B4513);
			Add("salmon",0xFA8072);
			Add("sandyBrown",0xF4A460);
			Add("seagreen",0x2E8B57);
			Add("seashell",0xFFF5EE);
			Add("sienna",0xA0522D);
			Add("silver",0xC0C0C0);
			Add("skyblue",0x87CEEB);
			Add("slateblue",0x6A5ACD);
			Add("slategray",0x708090);
			Add("snow",0xFFFAFA);
			Add("springgreen",0x00FF7F);
			Add("steelblue",0x4682B4);
			Add("tan",0xD2B48C);
			Add("teal",0x008080);
			Add("thistle",0xD8BFD8);
			Add("tomato",0xFF6347);
			Add("turquoise",0x40E0D0);
			Add("violet",0xEE82EE);
			Add("wheat",0xF5DEB3);
			Add("white",0xFFFFFF);
			Add("whitesmoke",0xF5F5F5);
			Add("yellow",0xFFFF00);
			Add("yellowgreen",0x9ACD3);
		};// cSpell: enable
		void Add(const string key,int val){
			map.insert(std::make_pair(key,val));
		}
		int operator[] (string n)
		{
			n=su::ToLowercase(n);
			auto f=map.find(n);
			if(f!=map.end())
				return f->second;
			throw pc::Exception(pcXPT_NOT_FOUND,
				"No such a named color '{}'",n);
		}
	};
	static double hue2rgb(double p,double q,double t)
	{
		if(t<0)	t++;
		if(t>1)	t--;
		if(t<1.0/6)	return p+(q-p)*6*t;
		if(t<1.0/2)	return q;
		if(t<2.0/3)	return p+(q-p)*(2.0/3-t)*6;
		return p;
	};
	void makeWithInt(int col,bool digit3=false)
	{
		if(col==-1)	R=-1;
		else if(digit3)
		{
			if(col>0xFFF||col<0)
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"Invalid color value #{:x}",col);
			R=(col&0xF00)>>8;
			G=(col&0x0F0)>>4;
			B=(col&0x00F);
			R=(R<<4)+R;
			G=(G<<4)+G;
			B=(B<<4)+B;
		}
		else
		{
			if(col>0xFFFFFF||col<0)
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"Invalid color value #{:x}",col);
			R=(col&0xFF0000)>>16;
			G=(col&0x00FF00)>>8;
			B=(col&0x0000FF);
		}
	}
	void makeWithHSL(int H,int S,int L)
	{
		double r,g,b;
		double h=H/360.0;
		double s=S/100.0;
		double l=L/100.0;
		if(h>1||h<0||s>1||s<0||l>1||l<0)
			throw pc::Exception(pcXPT_INVALID_ARGUMENT,
				"invalid HSL value ({},{}%,{}%)",H,S,L);
		if(s==0)	r=g=b=l;
		else
		{
			double q=l<0.5?l*(1+s):l+s-l*s;
			double p=2*l-q;
			r=hue2rgb(p,q,h+1.0/3);
			g=hue2rgb(p,q,h);
			b=hue2rgb(p,q,h-1.0/3);
		}
		R=int(r*255);
		G=int(g*255);
		B=int(b*255);
	}
	void makeWithRGB(int r,int g,int b)
	{
		if(r>255||r<0||g>255||g<0||b>255||b<0)
			throw pc::Exception(pcXPT_INVALID_ARGUMENT,
				"invalid RGB value {} {} {}",r,g,b);
		R=r,G=g,B=b;
	}
public:
	/** @brief All the HTML named color.
	 *  @note See https://developer.mozilla.org/en-US/docs/Web/CSS/named-color
	 */
	NamedColorTp NamedColors;
	Color():
		R(0),G(0),B(0){}
	/// @brief Make with integer (in hex).
	Color(int col,bool digit3=false)
		{makeWithInt(col,digit3);}
	/// @brief Make with RGB.
	Color(int r,int g,int b)
		{makeWithRGB(r,g,b);}
	/**
	 * @brief Convert a string to color.
	 * The string could be:
	 * ```
	 * #RGB
	 * #RRGGBB
	 * <named-color>
	 * rgb(R G B)
	 * hsl(H S L)
	 * ```
	 */
	Color(std::string str)
	{
		str=su::Trim(str);
		if(str[0]=='#')
		{
			int len=str.length();
			if(len!=4&&len!=7)
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"invalid hex color length({}) for '{}'",len-1,str);
			int col;
			try{
				col=std::stoi(str.substr(1),0,16);
			}catch(std::invalid_argument){
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"'{}' does not seems to be a color",str);
			}
			makeWithInt(col,len==4);
			return;
		}
		string lead=su::ExtractToken(str,[](char a){return a=='(';});
		lead=su::ToLowercase(lead);
		if(lead=="rgb"||lead=="hsl")
		{
			if(!str.starts_with('(')||!str.ends_with(')'))
				throw pc::Exception(pcXPT_PARSING,
					"invalid color format '{}{}'",lead,str);
			std::vector<int> res;
			try{
				std::ranges::transform(
					su::Split(str,"(, )"),
					std::back_inserter(res),
					[](string in){return std::stoi(in);}
				);
			}catch(std::invalid_argument){
				throw pc::Exception(pcXPT_PARSING,
					"invalid color format '{}{}'",lead,str);
			}
			if(lead=="rgb")
				makeWithRGB(res[0],res[1],res[2]);
			else if(lead=="hsl")
				makeWithHSL(res[0],res[1],res[2]);
		}
		else
			makeWithInt(NamedColors[lead]);
	}
	Color(const char* str)
		{*this=Color(string(str));}
	int toHex()const{return B|(G<<8)|(R<<16);}
	tuple<int,int,int> toRGB()const{return std::make_tuple(R,G,B);}
	tuple<int,int,int> toHSL()const
	{
		double r=R/255.0;
		double g=G/255.0;
		double b=B/255.0;
		double maxv=std::max(r,std::max(g,b));
		double minv=std::min(r,std::min(g,b));
		double h,s,l;
		l=(maxv+minv)/2;
		if(maxv==minv)
			h=s=0;
		else
		{
			double d=maxv-minv;
			s=l>0.5?d/(2-maxv-minv):d/(maxv+minv);
			if(maxv==r)
				h=(g-b)/d+(g<b?6:0);
			else if(maxv==g)
				h=(b-r)/d+2;
			else if(maxv==b)
				h=(r-g)/d+4;
			h/=6;
		}
		return std::make_tuple(int(h*360),int(s*100),int(l*100));
	}
	void HueRotate(int degree)
	{
		auto [h,s,l]=toHSL();
		h=(h+degree)%360;
		makeWithHSL(h,s,l);
	}
	void RGBOffset(int r,int g,int b)
	{
		R+=r;R%=256;
		G+=g;G%=256;
		B+=b;B%=256;
	}
	bool Transparent() const {return R==-1;}
	int operator<=>(const Color& other)const
	{
		int val1=toHex();
		int val2=other.toHex();
		if(val1<val2)	return -1;
		if(val1>val2)	return 1;
		return 0;
	}
	bool operator==(const Color& other)const
		{return operator<=>(other)==0;}
	bool operator!=(const Color& other)const
		{return operator<=>(other)!=0;}
	/// @brief if a is transparent, return b
	friend Color operator|(Color a,Color b)
	{
		if(a.Transparent())	return b;
		return a;
	}
	static inline Color RGB(int r,int g,int b)
		{return Color(r,g,b);}
	static inline Color HSL(int h,int s,int l)
		{Color a;a.makeWithHSL(h,s,l);return a;}
	/**
	 * @brief Calculate the average color the two color.
	 * @param index Position of the middle color.
	 * ```
	 * Col1 0 ---------+--------- 1 Col2
	 * ```
	 */
	static inline Color Gradient(Color Col1,Color Col2,double index=0.5)
	{
		Color ret;
		ret.R=int(Col1.R*index+Col2.R*(1-index));
		ret.G=int(Col1.G*index+Col2.G*(1-index));
		ret.B=int(Col1.B*index+Col2.B*(1-index));
		return ret;
	}
	/** @brief Get highlight version of a color.
	 *  The bigger `index` is, the whiter the result is. */
	static inline Color HighLight(Color col,double index=0.5)
		{return Gradient(col,0xFFFFFF,index);}
	/// @brief Invert the color.
	static inline Color Invert(Color col)
		{return Color(255-col.R,255-col.G,255-col.B);}
	static inline Color Random(int s,int l)
		{return Color::HSL(rand()%360,s,l);}
private:
	/// @brief Linearize luminace of a pixel based on RGB value 
	static inline double linearizeBrightness(int val)
	{
		double v=val/255.0;
		if(v<=0.03928)
			return v/12.92;
		return std::pow((v+0.055)/1.055,2.4);
	};
	/// @brief Calculate the physical luminace of a color 
	static inline double Luminance(const Color& c)
	{
		if(c.Transparent())
			return -1;
		double R=linearizeBrightness(c.R);
		double G=linearizeBrightness(c.G);
		double B=linearizeBrightness(c.B);
		return 0.2126*R+0.7152*G+0.0722*B;
	};
public:
    /**
	 * @brief Based on WCAG 2.1
	 * @return Contrast ratio (>=1), or -1.0 if any of the colors is transparent.
	*/
    static inline double ContrastRatio(const Color& c1,const Color& c2)
	{
        double l1=Luminance(c1);
        double l2=Luminance(c2);
        if(l1<0||l2<0)
			return -1.0; // 透明
        return (std::max(l1,l2)+0.05)/(std::min(l1,l2)+0.05);
    }
};