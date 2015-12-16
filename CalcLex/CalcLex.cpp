//CalcLex Program
//Scanner 
//
//Matthew Bowden

#include "calcLex.h"

using namespace std;

//GLOBALS
string inputFile;
string::iterator currPos;
bool endOfFile = false;
int tokenCount = 0;

Token yylex(); //might be global variables here
void yyconvertFiletoString(string);

int main(int argc, char *argv[]) {
	setupMap(); //sets up the map to print ENUM names
	

	//Get the filename from the command line 
	if (argc > 2 ) { //|| argc == 1) {
		cerr << "Usage: " << argv[0] << " FILENAME" << endl;
		return 1;
	} 
	else if (argc == 1) {
		//==DEBUG TESTING==
		inputFile = "read A jeff_224 := 2.5 + 40zz Write jeff read readFile /* comment * **/ \n /* \n rEAd B /**/ B := \t (2 / 5) qq22qq:=1+1.5975/2 ";
		inputFile += "sum := A + B write sum";
	}
	else {
	string fileName = argv[1];
	yyconvertFiletoString(fileName);
	cout << "====INPUT FILE====" << endl;
	cout << inputFile << endl; 
	cout << "====SCANNER OUTPUT====" << endl;
	}
	currPos = inputFile.begin();

	while (!endOfFile) {
		try {
			yylex();
		} 
		catch (exception& e) 
		{
			cout << "***" << e.what() << endl;
			cout << "==Halting Program==" << endl;
			return 0;
		}
	}
	cout << endl << "Number of tokens: " << tokenCount << endl;
	return 0;
}

//Function to convert the file specified in the command line
//into the string to be iterated
void yyconvertFiletoString(string fileName) {
	ifstream fileToOpen(fileName);
	if (fileToOpen.is_open())
	{
		inputFile = ""; //initialise input file
		string line;
		while (fileToOpen.good())
		{
			getline(fileToOpen, line);
			inputFile += line;
			//because getline discards delimiters, need to add it back in here
			inputFile += '\n'; 
		}
		fileToOpen.close();
	}
	else cout << "File Open error" << endl;
}

//Set simple tokens via function
bool setToken(Token& thisToken, CALCTOKENS OP, string tString = "") {
	thisToken.TokenNum = OP;
	if(tString.length() == 0) {
		thisToken.TokenString = *currPos;
	}
	else {
		thisToken.TokenString = tString;
	}
	currPos++;
	return true;
}


//Function to iterate through the file and find the next token
Token yylex() {
	bool tokenFound = false;	
	Token thisToken;
	thisToken.TokenString = "";
	do {
		//check against tokens
		if ((*currPos) == '\n' || (*currPos) == '\t' || (*currPos) == ' ') {
			currPos++; //skip white space
		}
		//LPAREN 
		else if ((*currPos) == '(') { 
			tokenFound = setToken(thisToken, LPAREN);
		}
		//RPAREN 
		else if (*currPos == ')') { 
			tokenFound = setToken(thisToken, RPAREN);
		}
		//ADDOP 
		else if (*currPos == '+') { 
			tokenFound = setToken(thisToken, ADDOP);
		}
		//MINOP 
		else if (*currPos == '-') { 
			tokenFound = setToken(thisToken, MINOP);
		}
		//MULTOP 
		else if (*currPos == '*') { 
			tokenFound = setToken(thisToken, MULTOP);
		}
		//ASSIGNSY
		else if (*currPos == ':') {
			currPos++; //increment to check next value
			if (*currPos == '=') {
				tokenFound = setToken(thisToken, ASSIGNSY, ":=");
			}
			else //in case there is bad input after ':'
				throw exception("Received an unexpected character after the initial character for ASSIGNSY");
		}
		//DIVOP & Comment checking
		else if (*currPos == '/') {
			currPos++; //be wary of this increment!
			if (*currPos != '*') { //if it's a valid DIVOP				
				tokenFound = setToken(thisToken, DIVOP, "/");
				currPos--;
			}

			//Previously had GOTO statements, reimplemented with loops
			else { //if currPos == '*'
				currPos++; 
				bool endFound = false;
				while (!endFound && *currPos != '\n') { //might need to revisit this to ensure functionality of newline end
					if (*currPos != '*') {
						currPos++;
					}
					else if (*currPos == '*') {
						currPos++;
						if (*currPos == '/') {
							endFound = true;
							currPos++;
						}
					}
				}
			}
		}
		//NUMCONST
		else if (isdigit(*currPos)) {
			string toAppend;
			thisToken.TokenNum = NUMCONST;
			bool decFound = false;
			do {
				//Checks input to see if a decimal has already been found in a single NUMCONST
				if(*currPos == '.' && decFound) {
					throw exception("Received a NUMCONST with multiple decimal points");
				}
				else if (*currPos == '.') {
					decFound = true;
					currPos++;
					if(!isdigit(*currPos)) {
						throw exception("Received an unexpected symbol after the decimal for NUMCONST");
					}
					else {
						currPos--;
					}
				}
				toAppend = *currPos;
				thisToken.TokenString.append(toAppend); //add current character to TokenString Buffer
				currPos++;
			} while((isdigit(*currPos) || *currPos == '.')); //checks if next value is a digit or decimal
			//if next falue is a decimal
			if(isalpha(*currPos)) {
				cout << "***Did not encounter a delimiter after reading NUMCONST \n***Parsing next sequence as NUMCONST then ID" << endl;								
			}			
			tokenFound = true;
		}
		//ID or READSY or WRITESY
		else if (isalpha(*currPos)) {
			string toAppend;
			do {
				toAppend = *currPos;
				thisToken.TokenString.append(toAppend);
				currPos++;
			} while (isalpha(*currPos) || isdigit(*currPos) || *currPos == '_');
			tokenFound = true;	
			if (iequals(thisToken.TokenString, "read")) {
				thisToken.TokenNum = READYSY;
				thisToken.TokenString = "read";
			}
			else if (iequals(thisToken.TokenString, "write")) {
				thisToken.TokenNum = WRITESY;
				thisToken.TokenString = "write";
			}
			else {
				thisToken.TokenNum = ID;
			}			
		}
		//If fell off the end of the inputfile
		else if (currPos == inputFile.end()) {
			tokenFound = setToken(thisToken, EOFSY, "$$");
			endOfFile = true;
		}
		//If a character found is not recognized
		else {
			throw exception("Received character was not recognized by the scanner");
		}
	} while (!tokenFound);
	cout << "Token: " << setfill('0') << setw(2) << thisToken.TokenNum << " " << TOKENMAP[thisToken.TokenNum] << " (" << thisToken.TokenString << ")" << endl;
	tokenCount++;
	return thisToken;
}
