/** 
* File:   Attribute.cpp
* Author: Uzi  sksouza.art@gmail.com
* Assignment 04: Building a Tree Struture from an XML File
* Created on October 20, 2014 for use in Assignment 04.
*/

#include "Attribute.h"


/** Attribute default contructor
* sets name and value strings to empty string
*@return returns an Attribute object
*/
Attribute::Attribute(){
	name ="";
	value="";

}
/** Attribute contructor
* sets name and value strings to empty string
*@param n holds the value to be stored in the name member of Attribute
*@param v holds the value to be stored in the value member of Attribute
*/
Attribute::Attribute(string n, string v){
	name = n;
	value = v;
}
/**Attribute copy constructor
*@param orig is the Attribute whose member values are to be copied
*@return returns an Attribute object
*/
Attribute::Attribute(const Attribute& orig){
	name = orig.name;
	value = orig.value;

}

/** Attribute Destructor
*Destroys Attribute object
*/
 Attribute::~Attribute(){
}
/**Sets the value of the name member of Attribute object
*@param attr value to which the string should be set
*@return nothing
*/
void Attribute::setName(string attr){
	name = attr;
}
/**Sets the value of the value member of Attribute object
*@param val value to which the string should be set
*@return nothing
*/
void Attribute::setValue(string val){
	value = val;
}

/** Gets the value of the value member of the Attribute 
*@return the value stores in the value member of the Attribute object
*/
string Attribute::getValue(){
	return value;
}

/**Gets value of name member of Attribute
*@return the value stored in the name member of Attribute object
*/
string Attribute::getAttrName(){
	return name;

}

/**Overloaded operator <<, used for outputting member values of the Attribute objects
*@param out ostream object: stream used for output
*@param attr Attribute object whose data is being output
*@return
*/
ostream& operator <<(ostream& out, Attribute& attr){
	out<<"name: "<< attr.name <<" and value: "<< attr.value;
	return out;
}