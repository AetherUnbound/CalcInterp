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

Token yylex(); //might be global variables here

int main(int argc, char *argv[]) {
	
	inputFile = "read A jeff := 2 + 4 $$";
	currPos = inputFile.begin();

	while (!endOfFile) {
		yylex();
	}


	return 0;
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
				currPos--; //set the iterater back
				thisToken.TokenNum = DIVOP;
				thisToken.TokenString = *currPos; //should be the '/' symbol
				tokenFound = true;
			}

			//In these next statements, I use GOTO statements because I have to 
			//go *back* to previous states, which would be difficult (impossible)
			//with just loops
			else { //if currPos == '*'
				currPos++;
			FirstStar:
				while (*currPos != '*') {
					currPos++;
				}
				currPos++;
				goto SecondStar;
			SecondStar: 
				if (*currPos == '/') {
					currPos++;
					goto EndComment;
				}
				else if (*currPos == '*') {
					currPos++;
					goto SecondStar;
				}
				else {
					currPos++;
					goto FirstStar;
				}
			EndComment:
				;//nothing here, just the end of the else statement
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
							goto EndIDLoop;
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
			if (tolower(*currPos) == 'w') {
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
								goto EndIDLoop;
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
			//This might be really dumb but I think it will work

			//now I have to just get an ID string
			thisToken.TokenNum = ID;
			do {
				toAppend = *currPos;
				thisToken.TokenString.append(toAppend);
				currPos++;
			} while (isalpha(*currPos) || isdigit(*currPos) || *currPos == '_');
			tokenFound = true;
		EndIDLoop: 
			;
		}
	} while (!tokenFound);
	cout << "Tokens: " << thisToken.TokenNum << " " << thisToken.TokenString << endl;
	return thisToken;
}
