/* 
 * File:   Element.h
 * Author: Uzi  sksouza.art@gmail.com
 * Assignment 04: Building a Tree Struture from an XML File
 * Created on September 19, 2014, 5:51 PM
 * Modified on September 19, 2014, 10:10 PM
 * Copied on October 1, 2014 by Uzi for use in next assignment.
 * Copied on October 15, 2014 by Uzi for use in next assignment.
 * Modified on October 21, 2014 to add child Element and Attricute vectors as well as to modify functions.
 * Modified across all files on November 08, 2014 at 7:32 AM to handle content of Elements.

 */

#ifndef ELEMENT_H
#define	ELEMENT_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Attribute.h"

using namespace std;

class Element {
public:
/** Element default contructor
* sets tag to empty string and content to {EMPTY}
*@return returns an Element object
*/
	Element();
   
/** Element contructor
* sets tag and tier and line number to given values
*@param name holds the value to be stored in the tag member of Element
*@param line holds the value to be stored in the lineNum member of Element
*@param level holds the value to be stored in the tier member of Element
*@param eleContent holds the content string for the Element
*@return returns an Element object.
*/
   Element(string name, int line, int tier, string eleContent);
   
/** Element copy contructor
*@param orig the Attribute object whose members' values are to be copied
*@return returns an Element object.
*/
	Element(const Element& orig);
   
/** Element destructor
*/
	virtual ~Element();

 /** 
 * Accessor function for the tag member variable.
 * @return string tag
 */
    string getName();

/** Returns the content of the Element object.
*@return returns the content string of the Element object upon which this function is called.
*/
	string getContent();

/** Returns the tier of the Element object.
*@return returns the value in the tier member variable of the Element object upon which this function is called.
*/
	int getTier();

/** Returns the line number of the Element object.
*@return returns the value in the lineNum member variable of the Element object upon which this function is called.
*/
	int getLine();

 /** Sets the string tag value to the given string
 *@param name: string to which the tag value will be changed
 */
    void setName(string name);


/**Sets value of lineNum to value passed to function.
 *@param num the new line number
 *@return lineNum member variable contains value from num
 */
	void setLine(int num);
	
/**Sets value of content string to value passed to function.
*@param someContent: the new content string
*@return content member variable contains value from someContent
*/
	void setContent(string someContent);

/**adds a pointer to a new child Element to the vector of child Elements of this Element
*@param name name of child Element
*@param line line number at which child Element is found
*@param level tier at which the current/parent Element is found
*@param eleContent content string for Element
*@return nothng
*/
	void addChild(string name, int line, int level, string eleContent);


/**adds a pointer to a new Attribute object to the vector of Attributes of this Element
 *@param name attribute name
 *@param value attribute value string
 *@return nothing
 */
	void addAttr(string name, string value);

/**Sets parent pointer to a new value
 *@param parentalUnit address for the parent pointer to store
 *@return nothing
 */
	void setParent(Element* parentalUnit);

/**Gets parent pointer of this Element
 *@return pointer to parent Element
 */
	Element* getParent();

/**Sets tier to a new value
 *@param level the new value for the Element tier
 *@return nothing
 */
	void setLevel(int level);
	
/** Gets a pointer to the private member vector children
*@return Returns a pointer to the vector of Element pointers 'children'
*/
	vector<Element*>* getChildren();

/** Gets a pointer to the private member vector attributes
*@return Returns a pointer to the vector of Attribute pointers 'attributes'
*/
	vector<Attribute*>* getAttr();

/** Overloaded operator to output the values of each member variable of an Element object
*@param out a reference to the out stream used
*@param ele a reference to the Element object whose data will be output
*@return out, the output stream to use
*/
    friend ostream& operator <<(ostream& out, Element& ele);

private:
        /**
         * Tag/name of this Element
        */
        string tag;
        /**
         * Content found between the ioening and closing tags of this Element
         */
	string content;
        /**
         * Line number at which this Element was found
         */
	int lineNum;
        /**
         * Tier or level at which this Element is found within the tree structure
         */ 
	int tier;
        /**
         * Pointer to the parent Element of this Element. NULL if root.
         */
	Element* parent;
        /**
         * Vector of pointers to child Element objects of this Element
         */
	vector<Element*> children;
        /**
         * Vector of pointers to Attribute objects belonging to this Element
         */
	vector<Attribute*> attributes;
	//int numOccur; 
        //vector<int> lineNum;
};

#endif	/* ELEMENT_H */

