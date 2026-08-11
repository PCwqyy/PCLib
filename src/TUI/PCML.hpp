/**
 * @file PCML.hpp
 * @author GithubCopilot
 * This file replaces the previous regex-based PCML/XML parser with a simple
 * hand-written parser that scans the source string character by character.
 * 
 * The parser supports:
 *   - XML declaration: <?xml ... ?>
 *   - Opening tags:   <tag attr="value" ...>
 *   - Self-closing:  <tag attr="value" .../>
 *   - Nested elements (recursive Markup)
 *   - Attributes of form: name="value"
 *   - Skipping comments: <!-- ... -->
 *
 * Notes / limitations (kept simple on purpose):
 *   - Attribute values are assumed to be double-quoted (no escape handling).
 *   - Tag/attribute name character set is simplified (letters, digits, ':' '_' '-').
 *   - Text nodes are not produced/kept (same behavior as original code).
 *   - Does a minimal "matching closing tag" search and relies on recursion to
 *	 properly pair nested elements of the same tag name.
 */

#pragma once
#define PCL_TUI_PCML

#include<cstdio>
#include<format>
#include<optional>
#include<string>
#include<utility>

using std::format;
using std::string;

#include"../Exception.hpp"
#include"Util.hpp"
#include"Element.hpp"
#include"Document.hpp"

namespace PCML
{

void skipWhitespace(const string &s,int &pos)
{
	while(pos<s.size()&&isspace(s[pos])) ++pos;
}

/**
 * @brief Parse a name token: letters/digits/'_'/'-'
 * @return Empty string if no valid name at pos
 */
string parseName(const string &s,int &pos)
{
	int start=pos;
	while(pos<s.size())
	{
		char c=s[pos];
		if(isalnum(c)||c=='_'||c=='-')
			++pos;
		else break;
	}
	return (pos>start)?s.substr(start,pos-start) : string();
}
/**
 * @brief Parse attribute value starting at a quote character (pos points to the opening `"`)
 * @return the value (without the quotes) and advances pos to the char after the closing `"`
 * if closing quote not found, consumes to end and returns what was found.
 */
string parseQuotedValue(const string &s,int &pos)
{
	if(pos>=s.size()||s[pos]!='"')
		return "";
	++pos; // skip opening quote
	int start=pos;
	while(pos<s.size())
	{
		if(s[pos]=='"')
		{
			string val=s.substr(start,pos-start);
			++pos; // skip closing quote
			return val;
		}
		++pos;
	}
	// unterminated quote -> return what we have
	return s.substr(start);
}

/**
 * @brief The function splits the first token as tag name and parses any attributes
 * of the form `name="value"` into Element::Attribute.
 * @param content The inside of a tag (between '<' and '>' or '/>'):
 *  e.g. 'person id="1" class="a b"'
 * @return an Element whose Tag and Attribute are set; inlineStyle/style are left as-is.
 */
Element parseTag(const string &content)
{
	int pos=0;
	// trim leading spaces
	skipWhitespace(content,pos);

	// parse tag name
	string tag=parseName(content,pos);
	Element ans(tag);

	// parse attributes
	while(true) {
		skipWhitespace(content,pos);
		if(pos>=content.size()) break;

		// try parse attribute name
		string attrName=parseName(content,pos);
		if(attrName.empty()) {
			// nothing parsable -> stop
			break;
		}

		skipWhitespace(content,pos);
		// expect '='
		if(pos>=content.size()||content[pos]!='=') {
			// malformed attribute, store empty value and continue
			ans.Attribute.Set(attrName,"");
			continue;
		}
		++pos; // skip '='
		skipWhitespace(content,pos);
		// expect double-quoted value
		if(pos<content.size()&&content[pos]=='"') {
			string val=parseQuotedValue(content,pos);
			ans.Attribute.Set(attrName,val);
		} else {
			// no quotes: read until whitespace (lenient)
			int start=pos;
			while(pos<content.size()&&!isspace((unsigned char)content[pos])) ++pos;
			ans.Attribute.Set(attrName,content.substr(start,pos-start));
		}
	}
	return ans;
}

/// @brief Remove the first n characters from string s
void DeletePrefix(string &s,int n)
{
	if(n>=s.size()) s.clear();
	else s=s.substr(n);
}

/**
 * @brief Try to parse one element from the beginning of pcml.
 * The function consumes the parsed prefix from pcml (pcml is modified).
 * @return std::nullopt if there's no element to parse at the current head.
*/
std::optional<Element> Make(string &pcml)
{
	int pos=0;

	// loop to skip whitespace and comments at the head
	while(true)
	{
		skipWhitespace(pcml,pos);
		// if comment starts here, remove it and restart
		if(pos + 4 <= pcml.size()&&pcml.substr(pos,4)=="<!--") {
			// find closing '-->'
			int endComment=pcml.find("-->",pos + 4);
			if(endComment==string::npos) {
				// unterminated comment: drop everything
				pcml.clear();
				return std::nullopt;
			}
			// consume through the end of comment
			DeletePrefix(pcml,endComment + 3);
			pos=0; // reset
			continue;
		}
		// not a comment start, break to next stage
		break;
	}

	// skip whitespace again
	skipWhitespace(pcml,pos);
	if(pos>=pcml.size()) {
		// nothing left
		pcml.clear();
		return std::nullopt;
	}

	// if next chars are a closing tag for some ancestor (e.g. </tag>), we should return nullopt
	if(pcml[pos]=='<'&&pos + 1<pcml.size()&&pcml[pos + 1]=='/') {
		return std::nullopt;
	}

	// must start with '<' for an element
	if(pcml[pos]!='<') {
		// not an element (text or whitespace). Make returns no element.
		return std::nullopt;
	}

	// find the end of this tag. must respect quoted attributes (don't stop at '>' inside quotes)
	int i=pos + 1;
	bool inQuote=false;
	for (; i<pcml.size(); ++i) {
		char c=pcml[i];
		if(c=='"') inQuote=!inQuote;
		else if(c=='>'&&!inQuote) break;
	}
	if(i>=pcml.size()) {
		// unterminated tag -> consume all and fail
		pcml.clear();
		return std::nullopt;
	}

	// now i is index of '>'
	// check if tag is a processing instruction <?...?> or similar-we treat it as not an element here
	if(pos + 1<pcml.size()&&pcml[pos + 1]=='?') {
		// find closing '?>'
		int piEnd=pcml.find("?>",pos + 2);
		if(piEnd==string::npos) {
			pcml.clear();
			return std::nullopt;
		}
		DeletePrefix(pcml,piEnd + 2);
		return std::nullopt;
	}

	// get inner content between '<' and '>'
	string inner=pcml.substr(pos + 1,i-(pos + 1)); // may end with '/' for self-closing
	// detect self-closing: last non-space character before '>' is '/'
	// find last non-space char in inner
	int lastNonSpace=inner.find_last_not_of(" \t\r\n");
	bool selfClosing=false;
	if(lastNonSpace!=string::npos&&inner[lastNonSpace]=='/') {
		selfClosing=true;
		// strip trailing '/' for attribute parsing
		inner=inner.substr(0,lastNonSpace);
	}

	// build element from inner content
	Element ans=parseTag(inner);

	// consume the opening tag from pcml
	DeletePrefix(pcml,i + 1);

	if(selfClosing) {
		// done-element has no children
		return ans;
	}

	// Non-self-closing -> parse children until matching closing tag is found.
	// We will look for a closing tag that matches ans.Tag. Parsing is recursive:
	while(true) {
		// skip whitespace and comments before child or closing tag
		int localPos=0;
		skipWhitespace(pcml,localPos);
		// remove leading comments as in top loop
		bool removedComment=false;
		if(localPos + 4 <= pcml.size()&&pcml.substr(localPos,4)=="<!--") {
			int endComment=pcml.find("-->",localPos + 4);
			if(endComment==string::npos) {
				// unterminated comment -> drop everything and stop
				pcml.clear();
				break;
			}
			DeletePrefix(pcml,endComment + 3);
			// continue parsing children after removing comment
			continue;
		}

		// after skipping whitespaces, if next token is a closing tag for ans, consume it and finish
		skipWhitespace(pcml,localPos);
		if(localPos<pcml.size()&&pcml[localPos]=='<'&&localPos + 1<pcml.size()&&pcml[localPos + 1]=='/') {
			// parse the name after '</'
			int namePos=localPos + 2;
			string closeName=parseName(pcml,namePos);
			if(!closeName.empty()&&closeName==ans.GetTag()) {
				// find '>' and consume closing tag fully
				// we must find the closing '>' (not inside quotes; but closing tags shouldn't have quotes)
				int gt=pcml.find('>',namePos);
				if(gt==string::npos) {
					pcml.clear();
					break;
				}
				DeletePrefix(pcml,gt + 1);
				break; // finished this element
			}
			// closing tag doesn't match -> it may be closing some deeper node; let recursion handle it.
		}

		// If next non-whitespace char is '<', try to parse a child element
		skipWhitespace(pcml,localPos);
		if(localPos<pcml.size()&&pcml[localPos]=='<') {
			auto child=Make(pcml);
			if(!child.has_value()) {
				// no element parsed -> probably text or mismatched closing tag; stop collecting children
				break;
			}
			// append parsed child (keeps ownership semantics compatible with original)
			ans.AppendChild(child.value());
			continue;
		}

		// If next char is not '<', this is text content / data node; original code did not handle text nodes,
		// so we stop and return current element as-is (text remains in pcml).
		break;
	}

	return ans;
}

#define pcML_ERR_XML "Can't find XML declaration of the Document"
/*
  Parse entire document at path:
	- read file into string
	- locate XML declaration <?xml ... ?> and extract attributes inside; keep the remaining content
	- build a top-level document element from xml-decl attributes (same behaviour as original)
	- repeatedly call Make to append top-level child elements until EOF
*/
Document Parse(const string &path)
{
	FILE *flPCML=fopen(path.c_str(),"r");
	if(!flPCML) throw pc::Exception(pcXPT_FILE,"Failed to open PCML file: %s",path);
	string pcml;
	int ch;
	while((ch=fgetc(flPCML))!=EOF) pcml.push_back((char)ch);
	fclose(flPCML);

	// locate XML declaration '<?xml' ... '?>'
	int pos=0;
	skipWhitespace(pcml,pos);
	int declStart=pcml.find("<?xml",pos);
	if(declStart==string::npos) throw pc::Exception(pcXPT_PARSING,pcML_ERR_XML);
	int declEnd=pcml.find("?>",declStart + 5);
	if(declEnd==string::npos) throw pc::Exception(pcXPT_PARSING,pcML_ERR_XML);

	// extract declaration inner content (between '<?xml' and '?>')
	string declInner=pcml.substr(declStart + 5,declEnd-(declStart + 5));
	// remove declaration from pcml (everything after declEnd+2)
	pcml=pcml.substr(declEnd + 2);

	// construct the Document object using the xml declaration attributes
	// original code used: Document ans=parseTag("xml "+string(res[1]));
	// emulate same behaviour: make a virtual "xml" tag with the declaration attributes
	Element xmlDecl=parseTag(string("xml ") + declInner);
	Document ans(xmlDecl);

	// parse remaining content and append elements
	while(!pcml.empty()) {
		// try parse next top-level element
		auto node=Make(pcml);
		if(node.has_value()) {
			ans.AppendChild(node.value());
			// continue until Make returns nullopt; skip any leading whitespace/comments is inside Make
		} else {
			// no element parsed at head -> drop leading whitespace and try again
			int p=0;
			skipWhitespace(pcml,p);
			if(p>=pcml.size()) break;
			// If non-whitespace remains that isn't an element (text), remove it conservatively to avoid infinite loop.
			// Find next '<' and cut out text before it.
			int nextLT=pcml.find('<',p);
			if(nextLT==string::npos) break;
			pcml=pcml.substr(nextLT);
		}
	}

	return ans;
}

} // namespace PCML