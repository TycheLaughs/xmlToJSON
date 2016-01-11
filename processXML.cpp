#include "processXML.h"
/** 
 * File:   processXML.cpp
 * Author: Uzi (Susan Souza)  sksouza.art@gmail.com
 * Assignment 04: Building a Tree Struture from an XML File
 * Created  on October 15, 2014 2:21 PM by Uzi for use in Assignment 04. 
 * Modified on October 19, 2014 by Uzi 
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

/** populateTree reads a XML file line by line and creates a tree structure not unlike the XML file's own out of Element Objects, 
 *presuming the file is well-formed
 *@param root A reference to the Element that will be the root element for the entire tree
 *@param in the file to be processed
 *@return nothing; the 'tree' should be fully populated at reaching the end of the file
 */
void populateTree(Element& root, ifstream& in) {
    int lineNumber = 0; //we haven't read any lines yet.
    int tier = 0; //likewise, we are starting this entire process at the root
    int rootMakeOnce = 1;
    string lineHolder; //This string holds one line at a time, gotten from the file to be processed.
    string token; //this string holds each token we get from the initial parser
    string attributeString;
    Element* parentElement; //keeps track of the parent Element-- mostly for reference and for filling in the parent member of each new child Element.
    ParserState state;
    string attrName = "";
    string attrVal = "";
    string content = "";
    state = UNKNOWN;
    do {
        //THINGS TO KEEP:   ELEMENT_OPEN_TAG,ELEMENT_OPEN_AND_CLOSING_TAG_ON_ONE_LINE, SELF_CLOSING_TAG
        lineNumber++;
        token = "";
        state = parse(in, state, token, attributeString, content); //process all of the line, until iterator reaches the end
        if (state == ELEMENT_OPEN_TAG || state == SELF_CLOSING_TAG || state == ELEMENT_OPEN_AND_CLOSING_TAG_ON_ONE_LINE) {
            //if we've found the root element opening tag (by seeing that the root has no children)
            if (tier == 0 && rootMakeOnce == 1) {
                //populate the root Element with the tag name and so on
                root.setName(token);
                cout << "Processing " << token << " on line " << lineNumber << "\n";
                root.setLine(lineNumber);
                root.setLevel(tier);
                parentElement = &root; //parent pointer starts at the root and moves down each tier 
                rootMakeOnce--; //need to make this specific if happen only once, ever
            } else { //otherwise make a new element and add it to the vector of elements 
                tier++; //this is a child, so increment tier
                parentElement->addChild(token, lineNumber, tier, content); //add this Element as a child
                cout << "Processing " << token << " on line " << lineNumber << "\n"; //test print
                parentElement = (parentElement->getChildren()->back()); //move the 'parent' pointer to the child Element so it could possibly have children


            }
        } else if (attributeString != "") {
            do {
                //call processAttribute until attributeString == ""
                attrName = "";
                attrVal = "";
                state = processAttribute(attributeString, attrName, attrVal); //if state is ATTRIBUTE, process as an attribute name-value pair
                if (state != ERROR) {
                    //make a new attribute, add it to the current Element we're working on
                    parentElement->addAttr(attrName, attrVal);
                    cout << "new attribute with name " << attrName << " and " << attrVal << endl;
                }
            } while (attributeString != "");
        } else if (state == ELEMENT_CLOSING_TAG || state == SELF_CLOSING_TAG || state == ELEMENT_OPEN_AND_CLOSING_TAG_ON_ONE_LINE) {
            tier--; //we just closed and element tag, which means no more children for the element closed
            parentElement = parentElement->getParent(); //we decremented the tier, which means we've moved up one, which means the parent pointer must also be moved



        }

        //if we have not gotten the state CLOSING_TAG from parse at newline(begin == npos), the next line should be processed into 
        //a child Element; tier should be incremented
        if (state == ERROR) {//on error, clean up memory after printing error messages with current line number
            cout << lineNumber << endl;
        }





    } while (state != ERROR && state != END_OF_FILE);


    return;

}

/* parse reads and tokenizes one line and tries to detect what type of data is in each token
 *@param in the file being read
 *@param state ParserState as the function is called
 *@param line usually the string in which the tag name is put; sometimes an error message instead
 *@param allAttr this is where a string containing any attributes is put
 *@param content this is where any element content found is stored
 *@ return state should be updated with current state
 */
ParserState parse(ifstream& in, ParserState& state, string& line, string& allAttr, string& content) {
    string lineHolder; //This string holds one line at a time, gotten from the file to be processed.

    //This special duckie is for use with the string::find function. 
    //Reference: http://www.cplusplus.com/reference/string/basic_string/find/
    size_t encountered;
    size_t el; //used to iterate through a line to pretty much tokenize a single tag name from the line for storage
    //Reference for checking input stream: http://www.cplusplus.com/reference/ios/ios/good/
    if ((getline(in, lineHolder)).good()) {
        //cout<< "Parsing: "<< lineHolder <<endl;
        //Reference: http://www.cplusplus.com/reference/string/basic_string/find/
        encountered = lineHolder.find('<'); //look for left angle bracket
        if (encountered == string::npos) {
            if (lineHolder.find('>') != string::npos && lineHolder[lineHolder.find('>') - 1] == '-') {
                if (state == IN_COMMENT || state == STARTING_COMMENT)
                    return END_COMMENT;
                else return ERROR;
            } else {
                if (state == IN_COMMENT || state == STARTING_COMMENT) {
                    el = 0;
                    /* Unnecessary processing to follow, commented out.
                    while( el < lineHolder.size()&& lineHolder[el] != '>'&& lineHolder[el] != '\n'){//Keep those valid characters in a string, and continue to 
                    //read them in until hitting a delimiter.
                    line = line + lineHolder[el];
                    el++;
                    }*/
                    return IN_COMMENT;
                }
            }
        }
        if (encountered != string::npos) {//if the find function did in fact return a position, use that to check the next character  
            if (lineHolder[encountered + 1] == '/') {//is this a closing tag on its own line?
                if (lineHolder.find('>') == string::npos) {
                    line = "Expected a > to close tag on this line.";
                    return ERROR;
                }
                el = encountered + 2;
                while (el < lineHolder.size() && lineHolder[el] != ' ' && lineHolder[el] != '>' && lineHolder[el] != '\n') {//Keep those valid characters in a string, and continue to 
                    //read them in until hitting a delimiter.
                    line = line + lineHolder[el];
                    el++;
                }
                return ELEMENT_CLOSING_TAG;
            } else if (lineHolder[encountered + 1 ] == '?') {//does this look like the start of a directive?
                if (lineHolder.find('>') == string::npos) {
                    line = "Expected a > to close tag on this line.";
                    return ERROR;
                }
                if (lineHolder.find('>') != string::npos && lineHolder[lineHolder.find('>') - 1] == '?') {//does the end also look like a directive?
                    el = encountered + 2;
                    /*//unnecessary proceccing commented out to follow
                    while( el < lineHolder.size()&& lineHolder[el] != '?' && lineHolder[el] != '>'&& lineHolder[el] != '\n'){//Keep those valid characters in a string, and continue to 
                    //read them in until hitting a delimiter.
                    line = line + lineHolder[el];
                    el++;
                    }*/

                    return DIRECTIVE;
                } else {
                    line = "Encountered unexpected values when scanning apparent directive.";
                    return ERROR;
                }
            } else if (lineHolder[encountered + 1] == '!') {//does this look like the start of a comment?
                if (lineHolder[encountered + 2] == '-' && lineHolder[encountered + 3] == '-') {
                    el = encountered + 4;
                    while (el < lineHolder.size() && lineHolder[el] != '-' && lineHolder[el] != '>' && lineHolder[el] != '\n') {//Keep those valid characters in a string, and continue to 
                        //read them in until hitting a delimiter.
                        line = line + lineHolder[el];
                        el++;
                    }
                    if (lineHolder.find('>') != string::npos && lineHolder[lineHolder.find('>') - 1] == '-' && lineHolder[lineHolder.find('>') - 2] == '-') {

                        return ONE_LINE_COMMENT;
                    } else return STARTING_COMMENT;
                } else {
                    line = "Encountered unexpected values when scanning apparent comment.";
                    return ERROR;
                }
            } else if (lineHolder[(lineHolder.find('>') - 1)] == '/') {//does this look like a self-closing tag?
                if (lineHolder.find('>') == string::npos) {
                    line = "Expected a > to close tag on this line.";
                    return ERROR;
                }
                el = encountered + 1;
                while (el < lineHolder.size() && lineHolder[el] != ' ' && lineHolder[el] != '>' && lineHolder[el] != '\n') {//Keep those valid characters in a string, and continue to 
                    //read them in until hitting a delimiter.
                    line = line + lineHolder[el];
                    el++;
                }
                //el--;
                if (lineHolder[el] == ' ') {
                    //cout<< "Found an attribute, I think.\n\n";
                    el++;
                    for (int i = 0; lineHolder[el + i] != '>'; i++) {
                        allAttr = allAttr + lineHolder[el + i];
                    }

                }
                //shove any and all attributes into another string reference here, for processing by attribite processing function
                return SELF_CLOSING_TAG;
            } else {/*It was an opening tag, then.*/
                if (lineHolder.find('>') == string::npos) {
                    line = "Expected a > to close tag on this line.";
                    return ERROR;
                }
                el = encountered + 1;
                while (el < lineHolder.size() && lineHolder[el] != ' ' && lineHolder[el] != '>' && lineHolder[el] != '\n') {//Keep those valid characters in a string, and continue to 
                    //read them in until hitting a delimiter.
                    line = line + lineHolder[el];
                    el++;
                }
                //--el;
                if (lineHolder[el] == ' ') {
                    //cout<< "Found an attribute, I think.\n\n";
                    el++;
                    for (int i = 0; lineHolder[el + i] != '>'; i++) {
                        allAttr = allAttr + lineHolder[el + i];
                    }
                }
                encountered = lineHolder.find('>');
                if (lineHolder.find('<', encountered) != string::npos) {//is there a closing tag on this line indicated by a left angle bracket?
                    encountered = lineHolder.find('<', encountered);
                    if (lineHolder[encountered + 1] == '/') {
                        if (lineHolder.find('>') == string::npos) {
                            line = "Expected a > to close tag on this line.";
                            return ERROR;
                        }//is this new tag in fact a closing tag?
                        else if (lineHolder.find('>', encountered) != string::npos) {
                            //shove any and all attributes into another string reference here, for processing by attribite processing function... already did this on line 226
                            //shove content string into... content.  so. find end of opening tag:
                            encountered = 0; //first move position size type to the beginning of the line
                            encountered = lineHolder.find('>', encountered); //now move it to the end of the opening tag.  We know it exists; we found it before, so no checking if we found it again. We found it, ok?
                            encountered++; //move off of the '>'
                            while (lineHolder.at(encountered) != '<') {//while we've not found the closing tag
                                content += lineHolder.at(encountered); //add characters to content string
                                encountered++;
                            }
                            return ELEMENT_OPEN_AND_CLOSING_TAG_ON_ONE_LINE;
                        }
                    } else {
                        line = "Encountered unexpected values when scanning what may have otherwise been an element opening and closing tag pair on one line.";
                        return ERROR;
                    }
                }
                //shove any and all attributes into another string reference here, for processing by attribite processing function
                //el--;
                if (lineHolder[el] == ' ') {
                    //cout<< "Found an attribute, I think.\n\n";
                    el++;
                    for (int i = 0; lineHolder[el + i] != '>'; i++) {
                        allAttr = allAttr + lineHolder[el + i];
                    }
                }
                return ELEMENT_OPEN_TAG;
            }
        }
    }
    return END_OF_FILE;
}

/** populateTree reads a XML file line by line and creates a tree structure not unlike the XML file's own out of Element Objects, 
 *presuming the file is well-formed
 *@param root A reference to the Element that will be the root element for the entire tree
 *@param infile the file to be processed
 *@return nothing; the 'tree' should be fully populated at reaching the end of the file
 */

void populateTreeV2(ifstream& infile, Element& root) {
    vector<Element*> eleStack;
    ParserState state;
    //Element somethingHere;//this is an element that holds on to the data coming in from the parse function for 
    // an instance to which the stored Element pointers on the vector will point
    string token; //usually the tag name,  the content of a comment or of a directive
    string attributeString = "";
    bool error = false; //if we've hit no errors, this will remain false.  Exists for quick exit of the big loop
    int lineCounter = 0; //this tracks which line is going through the parse function
    int tier = 0;
    string attrName = "";
    string attrVal = "";
    string content = "";
    state = UNKNOWN;
    do {
        if (!eleStack.empty()) {//if there are elements on the stack...
            tier = eleStack.size() + 1;
        } else {
            tier = 0;
        }
        lineCounter++; //we just moved onto a new line
        token = "";
        content = "";
        state = parse(infile, state, token, attributeString, content);
        switch (state) {//switch responding to the parser state received from parse function
            case ELEMENT_OPEN_TAG:
                //attribute checks here?
                //cout <<"Line "<<lineCounter <<":\n"<<"Found ELEMENT_OPEN_TAG:  "<<token<<"\n";
                if (eleStack.empty()) {//working with the original root here
                    root.setName(token);
                    root.setLine(lineCounter);
                    root.setLevel(tier);
                    eleStack.push_back(&root);

                    while (attributeString != "" && attributeString == "\"/") {//process attribute here
                        processAttribute(attributeString, attrName, attrVal);
                        eleStack.back()->addAttr(attrName, attrVal);
                        attrName = "";
                        attrVal = "";
                    }
                    attributeString = "";

                } else {
                    //the Element at the back of the stack is this child Element's parent-- 
                    //Hadn't thought of using a stack this way until Prof. Heines described it in class 
                    //this morning. Was building tree a different way before. This seems simpler. 10/28/2014
                    //tier++;
                    eleStack.back()->addChild(token, lineCounter, tier, content);
                    eleStack.push_back(eleStack.back()->getChildren()->back()); //push back also a pointer 
                    //to the child that we pushed onto children vector in case it has children
                    while (attributeString != "") {//process attribute here
                        processAttribute(attributeString, attrName, attrVal);
                        eleStack.back()->addAttr(attrName, attrVal);
                        attrName = "";
                        attrVal = "";
                    }
                    attributeString = "";

                }
                //cout<< "Added Element to stack.\nThere are "<<eleStack.size()<<" Elements on stack: \n";
                /*if(!eleStack.empty()){
                for(unsigned int i = 0; i< eleStack.size(); i++){
                cout<< *eleStack[i] <<"  ";
                }
                cout<<"\n\n";
                }*/
                break;
            case ELEMENT_CLOSING_TAG:
                //cout <<"Line "<<lineCounter <<":\n"<<"Found ELEMENT_CLOSING_TAG:  "<<token<<"\n";
                //if(token == eleStack[eleStack.size()-1]->getName()){
                //delete(eleStack[eleStack.size()-1]);
                eleStack.pop_back();
                //cout<< "Removed an Element from the stack.\n"<< "There are "<<eleStack.size()<<" Elements on stack: \n";
                /*if(eleStack.size()!=0){
                for(unsigned int i = 0; i< eleStack.size(); i++){
                cout<< *eleStack[i] <<"  ";
                }

                }cout<<"\n\n";*/
                //}
                //else{
                //	cout<< "*    ERROR: Opening/Closing tag mismatch.\n";
                //	error= true;
                //}
                break;
            case STARTING_DOCUMENT:
                //cout <<"Line "<<lineCounter <<":\nSTARTING_DOCUMENT\n \n";
                break;
            case DIRECTIVE:
                //cout <<"Line "<<lineCounter <<":\n"<<"Found DIRECTIVE: " << token <<"\nStack is unchanged.\n\n";
                break;
            case SELF_CLOSING_TAG:

                eleStack.back()->addChild(token, lineCounter, tier, content);
                //eleStack.push_back(eleStack.back()->getChildren()->back());
                while (attributeString != "" && attributeString == "\"/") {//process attribute here
                    processAttribute(attributeString, attrName, attrVal);
                    eleStack.back()->addAttr(attrName, attrVal);
                    attrName = "";
                    attrVal = "";
                }
                attributeString = "";

                //cout <<"Line "<<lineCounter <<":\n" <<"Found SELF_CLOSING_TAG: " <<token <<"\nStack is unchanged.\n\n";

                break;
            case STARTING_COMMENT:
                //cout <<"Line "<<lineCounter <<":\n" <<"Found STARTING_COMMENT\nStack is unchanged.\n\n";
                break;
            case IN_COMMENT:
                //cout <<"Line "<<lineCounter <<":\n" <<"IN_COMMENT: "<< token <<"\nStack is unchanged.\n\n";
                break;
            case END_COMMENT:
                //	cout <<"Line "<<lineCounter <<":\n" <<"Found END_COMMENT\nStack is unchanged.\n\n";
                break;
            case ONE_LINE_COMMENT:
                //cout <<"Line "<<lineCounter <<":\n" <<"Found ONE_LINE_COMMENT: " << token <<"\nStack is unchanged.\n\n";
                break;
            case ELEMENT_OPEN_AND_CLOSING_TAG_ON_ONE_LINE:

                eleStack.back()->addChild(token, lineCounter, tier, content);
                //eleStack.push_back(eleStack.back()->getChildren()->back());
                //eleStack.pop_back();
                while (attributeString != "" && attributeString == "\"/") {//process attribute here
                    processAttribute(attributeString, attrName, attrVal);
                    eleStack.back()->addAttr(attrName, attrVal);
                    attrName = "";
                    attrVal = "";
                }
                attributeString = "";


                //cout<<"Line "<<lineCounter <<":\n" <<"Found ELEMENT_OPEN_AND_CLOSING_TAG_ON_ONE_LINE: " << token <<"\nStack is unchanged.\n\n";
                break;
            case UNKNOWN:
                //cout <<"Line "<<lineCounter <<":\n" <<"Encountered UNKNOWN\n\n";
                break;
            case END_OF_FILE:
                //cout <<"Line "<<lineCounter <<":\n" <<"Found END_OF_FILE\n\n";
                break;
            case ERROR:
                cout << "Line " << lineCounter << ":\n*    Encountered ERROR: " << token << "\n";
                error = true;
                break;
        };

    } while (!infile.eof()&& !error);

    //if(!eleStack.empty()){
    //cout<< "*    ERROR.  XML file not well-formed.\n     Emptying stack.\n\n";
    /*for(unsigned int n = eleStack.size()-1; eleStack.size()> 0; n--){
    delete(eleStack[n]);
    eleStack.pop_back();
    }*/
    //eleStack.clear();
    //}
    //else cout<<"*    XML file appears to be well-formed.\n\n";

}

/** processAttribute separates a string into a name-value pair so that a new Attribute may be created
 *@param attr string that will be processed into a name-value pair
 *@param name the tokenized name portion of the name-value pair processed
 *@param value the tokenized value portion of the name-value pair
 *@return state the parser state after the attribute has been processed.  It should be set back to ELEMENT_OPEN_TAG if successful and ERROR if not
 */
ParserState processAttribute(string& attr, string& name, string& value) {
    size_t it, it2;
    it = attr.find('='); //find returns size_t, while begin returns string::iterator
    if (it == string::npos) {
        cout << "*    Error: Encountered what appears to be incorrectly specified attribute.*\n";
        return ERROR;
    }
    it = 0;
    while (attr.at(it) == '"' || attr.at(it) == ' ') {
        ++it;
    }
    for (; attr.at(it) != '='; ++it) {
        name += attr.at(it);
    }
    if (attr.at(it + 1) != '"') {
        cout << "*    Error: Could not find an open quote to delineate a value for an attribute.*\n";
        return ERROR;
    }
    it += 2; //bypass the open doublequote and equals sign that starts the value token 
    //and then check if there is in fact a second doublequote in this string
    it2 = attr.find('"', it);
    if (it2 == string::npos) {
        cout << "*    Error: Could not find a closing quote to delineate the end of a value for an attribute.\n";
        return ERROR;
    }
    for (; it != it2; ++it) {
        value += attr.at(it);
    }

    attr = attr.substr(it, (attr.size()-(it2)) + 1);
    if (attr == "\"" || attr == "/") {
        attr = "";
    }
    return UNKNOWN; //if all was successful, change the state back so the parse function will keep looking for other attributes
}

/**
 *toJSON is a recursive function that builds JSON from a tree structure created from Element objects
 *@param ele is the root Element object each time the function is called
 *@param level is the starting tier of the Element with which the function begins work
 *@param outString is the string to which all content to output is added
 *@param root is a pointer to the root Element of the tree for marking the end of the tree
 *@return nothing
 */
void toJSON(Element& ele, int& level, string& outString, Element* root) {

    if (ele.getTier() == 0) {
        //if(ele.getParent() == NULL){
        //outString = "";//if we're just starting, the string we have is empty
        //outString += "{\n";
        //cout<< "Root? "<<ele.getName()<< endl;
        //}


        /*else{*/

        outString += "{\n"; //either this is the root object or we went up a tier (down the tree), so open an object
        /*}*/
    }
    //else{	
    //	//if(level > ele.getTier() ){//|| level == 0){
    //	for(unsigned int i = 0; i < (ele.getTier());i++){
    //		outString+= " ";//get the indent
    //	}
    //}

    // }
    for (int i = 0; i <= (ele.getTier() + 1); i++) {
        outString += " "; //get the indent
    }
    outString += "\"";
    outString += ele.getName(); //print current element name
    outString += "\":";
    //for(unsigned int k = 0; k < ele.getTier()+1 ; k++){
    //	outString += " ";//again, indent control
    //}
    //outString += "{\n";
    if (ele.getContent() != "") {//if there is content, open up a bracket for object content
        outString += "{\n "; //bracket from line 523\n";
        for (int k = 0; k < ele.getTier(); k++) {
            outString += " "; //again, indent control
        }
        outString += "\"content\":";
        outString += "\"";
        outString += ele.getContent(); //print the element content
        outString += "\"";
    }
    //outString += "\n";
    if (ele.getAttr()->size() > 0) {//if there are attributes, print those
        //if the preceding content wasn't empty, print a comma
        if (ele.getContent() != "") {
            outString += ", "; //comma from line 534\n";
        } else {

            outString += "\n";
            for (int k = 0; k < ele.getTier() + 3; k++) {
                outString += " "; //again, indent control
            }
            outString += "{\n"; //otherwise print an opening bracket

        }
        for (unsigned int j = 0; j < ele.getAttr()->size(); j++) {
            if (j == 0) {
                for (int k = 0; k < ele.getTier(); k++) {
                    outString += " "; //again, indent control
                }
                //outString += "  \n";//open the attributes object
            }
            for (int k = 0; k < ele.getTier() + 3; k++) {
                outString += " "; //again, indent control
            }
            outString += "\"";
            outString += (ele.getAttr())->at(j)->getAttrName(); //print attribute name
            outString += "\":\"";
            outString += (ele.getAttr())->at(j)->getValue(); //print attribute value


            outString += "\""; //ok.. next attribute?  
            if (j < ele.getAttr()->size() - 1) {
                outString += ", "; //comma from line 558";//put commas where appropriate
            }
            if (j == ele.getAttr()->size()) {
                for (int k = 0; k < ele.getTier(); k++) {
                    outString += " "; //again, indent control
                }

            }

        }//outString += "  }";//close the attributes object
    }//end print attributes here
    //so now we need to get the children
    if (ele.getParent() != NULL) {
        if (!ele.getParent()->getChildren()->empty() && ele.getAttr()->empty() && ele.getContent() == "") {//if there are kids and we had no attributes
            //open a bracket
            outString += "\n";
            for (int k = 0; k < ele.getTier() + 3; k++) {
                outString += " "; //again, indent control
            }
            outString += "{ "; //bracket from line 582";
        }
    }
    else {//if this is the absolute root, we need a bracket
        //outString += "\n";
        //for(unsigned int k = 0; k < ele.getTier()+3; k++){
        //	outString += " ";//again, indent control
        //}
        outString += "\n{ "; //bracket from line 589";
    }
    if (!ele.getChildren()->empty() && (!ele.getAttr()->empty() || ele.getContent() != "")) {
        //or if we had preceding content or attributes or siblings
        outString += ","; // comma from line 595";//put commas where appropriate

    }
    for (unsigned int x = 0; x < ele.getChildren()->size(); x++) {
        level = ele.getTier();
        outString += "\n";
        toJSON(*(ele.getChildren()->at(x)), level, outString, root); //recursion here
        outString += "\n";
        for (int i = 0; i < (ele.getTier() + 2) - 1; i++) {
            outString += " "; //get the indent
        }
        outString += "} "; //bracket from line 606";
        if (x < ele.getChildren()->size() - 1) {
            outString += ", "; //comma from line 608";//put commas where appropriate
        }
    }
    if (&ele == root->getChildren()->at(root->getChildren()->size() - 1)) {
        outString += "\n ";
        for (int k = 0; k < ele.getTier() - 1; k++) {
            outString += " "; //again, indent control
        }

        outString += "}\n";
        for (int k = 0; k < ele.getTier() - 1; k++) {
            outString += " "; //again, indent control
        }

        outString += "}";
    }
    return;
}