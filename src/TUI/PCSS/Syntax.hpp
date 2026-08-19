#pragma once
#define PCL_TUI_PCSS_SYNTAX

#include<string>
using std::string;

#include"../../Exception.hpp"
#include"Type.hpp"

namespace PCSS
{

class SyntaxParser
{
private:
	class Atom{
		Type::Types type;
		string identifier;
		Atom(Type::Types tp):type(tp){
			if(tp==Type::Types::Identifier)
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"PCSS Atom parser received 'Identifier' type without content");
		}
		Atom(Type::Types tp,string id){
			if(tp==Type::Types::Identifier)
				throw pc::Exception(pcXPT_INVALID_ARGUMENT,
					"PCSS Atom parser received a identifier({}) but the type is not identifer",id);
		}
	};
};

} // namespace PCSS
