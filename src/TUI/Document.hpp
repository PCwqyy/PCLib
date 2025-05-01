#include<string>
#include<cctype>
#include<vector>
#include<print>

using std::string;
using std::vector;
using std::map;

#include"Element.hpp"

class Document: public Element
{
	short x=0,y=0,width;
	map<string,StyleSheet> styles;
public:
	Document():Element(){}
	Document(Element a):Element(a){}
};