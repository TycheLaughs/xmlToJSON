/** 
* File:   Element.cpp
* Author: Uzi  sksouza.art@gmail.com
* Assignment 04: Building a Tree Struture from an XML File
* Created on September 19, 2014, 5:51 PM
* Modified on September 19, 2014, 10:10 PM
* Copied on October 1, 2014 by Uzi for use in next assignment.
* Modified on October 4, 2014 by Uzi to make significant changes in Element, 
* namely removing the vector of line numbers and the occurence counter and associated 
* functions and adding a string variable to store non-tag name content.
* Modified on October 20, 2014 for use in Assignment 04.
* Modified on October 21, 2014 to add child Element and Attricute vectors as well as to modify functions.
* Modified on October 24, 2014 to tune overloaded operator <<
* Modified on October 28, 2014 to make last adjustments to overloaded operator <<
* Modified across all files on November 08, 2014 at 7:32 AM to handle content of Elements.
*/

#include "Element.h"

/** Element default contructor
* sets tag to empty string and content to {EMPTY}
*@return returns an Element object
*/
Element::Element() {
	tag = "";
	content = "";
	// numOccur = 0;
}

/** Element copy contructor
*@param orig the Attribute object whose members' values are to be copied
*@return returns an Element object.
*/
Element::Element(const Element& orig) {
	unsigned int i;
	tag = orig.tag;
	//content = orig.content;
	// numOccur = orig.numOccur;
	//Remember in copying a vector member of a class object that you need all of its elements, too.
	for(i = 0; i < orig.children.size(); i++){
		children.push_back(orig.children[i]);
	}
	for(i = 0; i < orig.attributes.size(); i++){
		attributes.push_back(orig.attributes[i]);
	}
	parent = orig.parent;
}

/** Element contructor
* sets tag and tier and line number to given values
*@param name holds the value to be stored in the tag member of Element
*@param line holds the value to be stored in the lineNum member of Element
*@param level holds the value to be stored in the tier member of Element
*@param eleContent holds the content string for the Element
*@return returns an Element object.
*/
Element::Element(string name, int line, int level, string eleContent){
	tag = name;
	lineNum = line;
	tier = level;
	content = eleContent;

}

/** Element destructor
*/
Element::~Element() {
	for(int i = children.size(); i > 0; i--){
		children.clear();
	/*delete children[i];
	children.pop_back();*/
	}
	for(int i = attributes.size(); i > 0; i--){
		attributes.clear();
	/*delete attributes[i];
	attributes.pop_back();*/
	}
}

/**Sets value of content string to value passed to function.
*@param someContent: the new content string
*@return content member variable contains value from someContent
*/
void Element::setContent(string someContent){
	content = someContent;
}
/**Sets parent pointer to a new value
*@param parentalUnit address for the parent pointer to store
*@return nothing
*/
void Element::setParent(Element* parentalUnit){
	parent = parentalUnit;
}

/** Returns the content of the Element object.
*@return returns the content string of the Element object upon which this function is called.
*/
string Element::getContent(){
	return content;
}
/** Returns the tier of the Element object.
*@return returns the value in the tier member variable of the Element object upon which this function is called.
*/

int Element::getTier(){
return tier;
}


/** Returns the line number of the Element object.
*@return returns the value in the lineNum member variable of the Element object upon which this function is called.
*/
int Element::getLine(){

	return lineNum;
}

/** 
* Accessor function for the tag member variable.
* @return string tag
*/
string Element::getName(){
	return tag;
}

/** Sets the string tag value to the given string
*@param name: string to which the tag value will be changed
*/
void Element::setName(string name){
	tag = name;
}


/**Sets value of lineNum to value passed to function.
*@param num the new line number
*@return lineNum member variable contains value from num
*/
void Element::setLine(int num){
	lineNum = num;
}


/**adds a pointer to a new child Element to the vector of child Elements of this Element
*@param name name of child Element
*@param line line number at which child Element is found
*@param level tier at which the current/parent Element is found
*@param eleContent content string for Element
*@return nothng
*/
void Element::addChild(string name, int line, int level, string eleContent){
	children.push_back(new Element(name, line, level+1, eleContent));
	children.back()->setParent(this);
}


/**Gets parent pointer of this Element
*@return pointer to parent Element
*/
Element* Element::getParent(){
return parent;
}
void Element::addAttr(string name, string value){
	attributes.push_back(new Attribute(name, value));

}

/**Sets tier to a new value
*@param level the new value for the Element tier
*@return nothing
*/
void Element::setLevel(int level){

	tier = level;
}


/** Gets a pointer to the private member vector children
*@return Returns a pointer to the vector of Element pointers 'children'
*/
vector<Element*>* Element::getChildren(){
	return &children;

}

/** Gets a pointer to the private member vector attributes
*@return Returns a pointer to the vector of Attribute pointers 'attributes'
*/
vector<Attribute*>* Element::getAttr(){
	return &attributes;
}

/** Overloaded operator to output the values of each member variable of an Element object
*@param out a reference to the out stream used
*@param ele a reference to the Element object whose data will be output
*@return out, the output stream to use
*/
ostream& operator <<(ostream& out, Element& ele){
	for(int i = 0; i < ele.tier; i++){
		out<< ". ";//if we changed tiers, which direction was it? 
		//if up, { and if down, }...maybe?  for commas, do ",\n" at the beginning of all lines but first in a series... 
	}//print root.. do we do it inside a curly brace or outside? confuuuuused.
	out<<ele.tag<<" found on line "<< ele.lineNum <<" with "<< ele.attributes.size()<<" attributes and "<< ele.children.size()<<" children.\n";
	for(unsigned int j = 0; j < ele.attributes.size(); j++){
			for(int i = 0; i < ele.tier; i++)
				out<< ". ";//then attributes
		out<<"Attribute with "<<*(ele.attributes[j])<<" found on line "<<ele.lineNum <<"\n";
	}
	for(unsigned int k = 0; k < ele.children.size(); k++){//then children.  This should be recursive...
		out<<*(ele.children[k]);
	}
	return out;
}