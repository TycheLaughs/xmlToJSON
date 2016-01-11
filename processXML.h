#ifndef PROCESSXML_H
#define PROCESSXML_H

#include <cstdlib>
#include <vector>
#include <string>
/** 
* File:   processXML.h
* Author: Uzi (Susan Souza)  sksouza.art@gmail.com
* Assignment 04: Building a Tree Struture from an XML File
* Created  on October 19, 2014 2:21 PM by Uzi for use in Assignment 04. 
* Modified on October 20, 2014 by Uzi 
* Mofified on October 23, 2014 by Uzi to continue working on tree implementation and a revision of the parser function(s).
* Modified on October 24, 2014 by Uzi to continue working on tree implementation and a revision of the parser function(s).
* Modified on October 25, 2014 by Uzi to continue working on tree implementation and output function(s).
* Modified on October 26, 2014 by Uzi to scrap the previous week's work and to restart by modifying code from Uz's Assignment 03
* Modified on October 27, 2014 by Uzi to tune the parser
* Modified on October 28, 2014 by Uzi to chase that last bug in the parser, fix the attribute processing function and 
* to make a quick revision to the tree-building bits to make them far simpler.
* Modified across all files on November 08, 2014 at 7:32 AM to handle content of Elements.
*/

#include <iostream>
#include <fstream>
#include <iterator>
#include "Element.h"
#include "Attribute.h"

using namespace std;




/*this enum from prof's notes: https://teaching.cs.uml.edu/~heines/91.204/91.204-2014-15f/204-lecs/lecture07.jsp */
enum ParserState {UNKNOWN, STARTING_DOCUMENT, DIRECTIVE, ELEMENT_OPEN_TAG, /*ELEMENT_CONTENT, */SELF_CLOSING_TAG, ELEMENT_CLOSING_TAG, ELEMENT_OPEN_AND_CLOSING_TAG_ON_ONE_LINE, 
STARTING_COMMENT, IN_COMMENT, END_COMMENT, ONE_LINE_COMMENT, END_OF_FILE, ERROR};



/* parse reads and tokenizes one line and tries to detect what type of data is in each token
*@param in the file being read
*@param state ParserState as the function is called
*@param line usually the string in which the tag name is put; sometimes an error message instead
*@param allAttr this is where a string containing any attributes is put
*@param content this is where any element content found is stored
*@ return state should be updated with current state
*/
ParserState parse(ifstream& in, ParserState& state, string& line, string& allAttr, string& content);

/** processAttribute separates a string into a name-value pair so that a new Attribute may be created
*@param attr string that will be processed into a name-value pair
*@param name the tokenized name portion of the name-value pair processed
*@param value the tokenized value portion of the name-value pair
*@return state the parser state after the attribute has been processed.  It should be set back to ELEMENT_OPEN_TAG if successful and ERROR if not
*/
ParserState processAttribute(string& attr, string& name, string& value);

/** populateTree reads a XML file line by line and creates a tree structure not unlike the XML file's own out of Element Objects, 
*presuming the file is well-formed
*@param root A reference to the Element that will be the root element for the entire tree
*@param infile the file to be processed
*@return nothing; the 'tree' should be fully populated at reaching the end of the file
*/
void populateTreeV2(ifstream& infile, Element& root);
//void populateTree(Element& root, ifstream& in);

/**
*toJSON is a recursive function that builds JSON from a tree structure created from Element objects
*@param ele is the root Element object each time the function is called
*@param level is the starting tier of the Element with which the function begins work
*@param outString is the string to which all content to output is added
*@param root is a pointer to the root Element of the tree for marking the end of the tree
*@return nothing
*/
void toJSON(Element& ele, int& level, string& outString, Element* root);


#endif //processXML.h