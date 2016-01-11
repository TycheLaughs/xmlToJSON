/** 
* File:   main.cpp
* Author: Uzi (Susan Souza)  sksouza.art@gmail.com
* Assignment 04: Building a Tree Struture from an XML File
* Created on September 18, 2014, 11:19 AM
* Modified on September 19, 2014, 1:12 PM
* Modified on September 19, 2014, 5:32 PM
* Modified on September 19, 2014, 10:10 PM
* Duplicated on October 1, 2014 by Uzi for use in next assignment.
* Modified on October 3, 2014 by Uzi
* Modified on October 4, 2014 by Uzi
* Duplicated on October 15, 2014 2:21 PM by Uzi for use in Assignment 04. 
* Modified on October 19, 2014 by Uzi Large blocks of unused code removed for ease of reading for own anticipated revisions.
* Modified on October 21, 2014, 5:32 AM by Uzi to add Attribute class to project and modify members and methods to the Element class 
* as well as to block out pseudocode for how to go about the revisions for the new functionality(making specific encountered tokens Elements, 
* Attributes and storing them in a tree structure).
* Mofified on October 23, 2014 by Uzi to continue working on tree implementation and a revision of the parser function(s).
* Modified on October 24, 2014 by Uzi to continue working on tree implementation and a revision of the parser function(s).
* Modified on October 25, 2014 by Uzi to continue working on tree implementation and output function(s).
* Duplicated on November 01, 2014 by Uzi for use in JSON output project.
* Modified on November 02, 2014 by Uzi to begin work on JSON conversion/output.
* Modified on November 05, 2014 by Uzi to continue work on JSON output.
* Modified on November 07, 2014 by Uzi to continue work on toJSON function.
* Modified across all files on November 08, 2014 at 7:32 AM to handle content of Elements.
* Moved toJSON function to processXML. November 08, 2014, 8:00 AM.
* Modified on November 08, 2014 to change JSON output to file, rather than console.
*/


#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "Element.h"
#include "Attribute.h"
#include "processXML.h"

using namespace std;



/**
* This application reads in a XML file and creates a tree out of Element objects to mimic 
*the XML file's own strucutre, converting it to JSON output.
* 
*/
int main(int argc, char** argv) {

	Element root;
	string outString;
	int level;
	ifstream infile;//stream for file we're reading
	ofstream outfile;
	string of;
	string filename = "albums.xml";//name of file
	string outfilename = "dist/out.json";
	level = 0;
	root.setParent(NULL);//this is the root Element, which does not have a parent
	infile.open(filename.c_str());//open file... 
	outfile.open(outfilename.c_str(), std::fstream::out);

	populateTreeV2(infile, root);

	//We've reached the end of the file or an error, so we'd better close the file now 
	//that we're not using it.
	infile.close();

	//cout<< root;
	toJSON(root, level, outString, &root);

	outfile<< endl << outString;
	outfile.close();
	cout<< "JSON output written to "<< outfilename<<"."<<endl;

	return 0;
}


