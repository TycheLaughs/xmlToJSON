/** 
* File:   Attribute.h
* Author: Uzi  sksouza.art@gmail.com
* Assignment 04: Building a Tree Struture from an XML File
* Created on October 20, 2014 for use in Assignment 04.
*/
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class Attribute{
	
private:
        /**
         * Attribute name
         */
	string name;
        /**
         * Attribute value
         */
	string value;

public:	
	/** Attribute default contructor
	* sets name and value strings to empty string
	*@return returns an Attribute object
	*/
	Attribute();
	/** Attribute contructor
	* sets name and value strings to empty string
	*@param n holds the value to be stored in the name member of Attribute
	*@param v holds the value to be stored in the value member of Attribute
	*/
	Attribute(string n, string v);

	/**Attribute copy constructor
	*@param orig is the Attribute whose member values are to be copied
	*@return returns an Attribute object
	*/
	Attribute(const Attribute& orig);

	/** Attribute Destructor
	*Destroys Attribute object
	*/
	virtual ~Attribute();
	
	/**Sets the value of the name member of Attribute object
	*@param attr value to which the string should be set
	*@return nothing
	*/
	void setName(string attr);

	/**Sets the value of the value member of Attribute object
	*@param val value to which the string should be set
	*@return nothing
	*/
	void setValue(string val);

	/** Gets the value of the value member of the Attribute 
	*@return the value stores in the value member of the Attribute object
	*/
	string getValue();

	/**Gets value of name member of Attribute
	*@return the value stored in the name member of Attribute object
	*/
	string getAttrName();

	/**Overloaded operator <<, used for outputting member values of the Attribute objects
	*@param out ostream object: stream used for output
	*@param attr Attribute object whose data is being output
	*@return
	*/
	friend ostream& operator <<(ostream& out, Attribute& attr);
};


#endif //Attribute.h