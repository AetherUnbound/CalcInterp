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
void convertFiletoString(string);

int main(int argc, char *argv[]) {
	setupMap(); //sets up the map to pring ENUM names
	

	//Get the filename from the command line 
	if (argc > 2) {
		cerr << "Usage: " << argv[0] << " FILENAME" << endl;
		return 1;
	}
	else if (argc == 1) {
		//==DEBUG TESTING==
		inputFile = "read A jeff_224 := 2 + 4 Write jeff /* comment * ** */ \n /* \n rEAd B /**/ B := \t 2 / 5 qq22qq:=1+1.5975/2 ";
		inputFile += "sum := A + B write sum $$";
	}
	else {
		string fileName = argv[1];
		convertFiletoString(fileName);
		cout << inputFile << endl;
	}
	currPos = inputFile.begin();

	while (!endOfFile) {
		try {
			yylex();
		} 
		catch (exception& e) 
		{
			cout << e.what() << endl;
			cout << "==Halting Program==" << endl;
			return 0;
		}
	}
	cout << endl << "Number of tokens: " << tokenCount << endl;
	return 0;
}

//Function to convert the file specified in the command line
//into the string to be iterated
void convertFiletoString(string fileName) {
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
			thisToken.TokenNum = LPAREN;
			thisToken.TokenString = *currPos;
			currPos++; //move on to next char
			tokenFound = true; //break loop
		}
		//RPAREN 
		else if (*currPos == ')') { 
			thisToken.TokenNum = RPAREN;
			thisToken.TokenString = *currPos;
			currPos++;
			tokenFound = true;
		}
		//ADDOP 
		else if (*currPos == '+') { 
			thisToken.TokenNum = ADDOP;
			thisToken.TokenString = *currPos;
			currPos++;
			tokenFound = true;
		}
		//MINOP 
		else if (*currPos == '-') { 
			thisToken.TokenNum = MINOP;
			thisToken.TokenString = *currPos;
			currPos++;
			tokenFound = true;
		}
		//MULTOP 
		else if (*currPos == '*') { 
			thisToken.TokenNum = MULTOP;
			thisToken.TokenString = *currPos;
			currPos++;
			tokenFound = true;
		}
		//ASSIGNSY
		else if (*currPos == ':') {
			currPos++; //increment to check next value
			if (*currPos == '=') {
				thisToken.TokenNum = ASSIGNSY;
				thisToken.TokenString = ":=";
				tokenFound = true;
				currPos++;
			}
			else //in case there is bad input after ':'
				throw exception("Received an unexpected character after the initial character for ASSIGNSY");
		}
		//EOFSY
		else if (*currPos == '$') {
			currPos++; //increment to check next value
			if (*currPos == '$') {
				thisToken.TokenNum = EOFSY;
				thisToken.TokenString = "$$";
				tokenFound = true;
				endOfFile = true;
			}
			else //in case there is bad input after '$'
				throw exception("Received an unexpected character after the initial character for EOFSY");
		}
		//DIVOP & Comment checking
		else if (*currPos == '/') {
			currPos++; //be wary of this increment!
			if (*currPos != '*') { //if it's a valid DIVOP				
				thisToken.TokenNum = DIVOP;
				thisToken.TokenString = '/'; 
				tokenFound = true;
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
			do {
				toAppend = *currPos;
				thisToken.TokenString.append(toAppend); //add current character to TokenString Buffer
				currPos++;
			} while(isdigit(*currPos) || *currPos == '.'); //checks if next value is a digit or decimal
			tokenFound = true;
		}
		//ID or READSY or WRITESY
		else if (isalpha(*currPos)) {
			string toAppend;
			if (tolower(*currPos) == 'r') {
				currPos++;
				if (tolower(*currPos) == 'e') {
					currPos++;
					if (tolower(*currPos) == 'a') {
						currPos++;
						if (tolower(*currPos) == 'd') {
							currPos++;
							thisToken.TokenNum = READYSY;
							thisToken.TokenString = "read";
							tokenFound = true;							
						}
						else {
							currPos--; currPos--; currPos--; //revert the last 3 increments
						}
					}
					else {
						currPos--; currPos--; //revert the last 2 increments
					}
				}
				else {
					currPos--;
				}
			}
			else if (tolower(*currPos) == 'w') {
				currPos++;
				if (tolower(*currPos) == 'r') {
					currPos++;
					if (tolower(*currPos) == 'i') {
						currPos++;
						if (tolower(*currPos) == 't') {
							currPos++;
							if (tolower(*currPos) == 'e') {
								currPos++;
								thisToken.TokenNum = WRITESY;
								thisToken.TokenString = "write";
								tokenFound = true;								
							}
							else {
								for (int i = 0; i < 4; i++)
									currPos--;
							}
						}
						else {
							for (int i = 0; i < 3; i++)
								currPos--;
						}
					}
					else {
						for (int i = 0; i < 2; i++)
							currPos--;
					}
				}
				else {
					currPos--;
				}
			}
			if (!tokenFound) {
				//now I have to just get an ID string
				thisToken.TokenNum = ID;
				do {
					toAppend = *currPos;
					thisToken.TokenString.append(toAppend);
					currPos++;
				} while (isalpha(*currPos) || isdigit(*currPos) || *currPos == '_');
				tokenFound = true;
			}		
		}
	} while (!tokenFound);
	cout << "Token: " << setfill('0') << setw(2) << thisToken.TokenNum << " " << TOKENMAP[thisToken.TokenNum] << " (" << thisToken.TokenString << ")" << endl;
	tokenCount++;
	return thisToken;
}
