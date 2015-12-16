//CalcLex Program
//Scanner 
//
//Matthew Bowden

#include "calcinterp.h"

using namespace std;

//GLOBALS
string inputFile;
string::iterator currPos;
bool endOfFile = false;
int tokenCount = 0;
Token tok;
map<string, double> symTable;

//Function Prototypes
void yyprogram();
void sList();
void stmt();
double expr();
double term();
double tTail(double inval);
double factor();
double fTail(double inval);

Token yylex(); //might be global variables here
void yyconvertFiletoString(string);

int main(int argc, char *argv[]) {
	setupMap(); //sets up the map to print ENUM names
	

	//Get the filename from the command line 
	if (argc > 2 || argc == 1) {
		cerr << "Usage: " << argv[0] << " FILENAME" << endl;
		return 1;
	} 
	//else if (argc == 1) {
		//==DEBUG TESTING==
		//inputFile = "read A jeff_224 := 2.5 + 40zz Write jeff read readFile /* comment * **/ \n /* \n rEAd B /**/ B := \t (2 / 5) qq22qq:=1+1.5975/2 ";
		//inputFile += "sum := A + B write sum";
	//}
	else {
	string fileName = argv[1];
	yyconvertFiletoString(fileName);
	cout << "====INPUT FILE====" << endl;
	cout << inputFile << endl; 
	cout << "====INTERPRETER OUTPUT====" << endl;
	}
	currPos = inputFile.begin();

	while (!endOfFile) {
		try {
			tok = yylex();
			yyprogram();
		} 
		catch (exception& e) 
		{
			cout << "***" << e.what() << endl;
			cout << "==Halting Program==" << endl;
			return 0;
		}
	}
	//cout << endl << "Number of tokens: " << tokenCount << endl;
	return 0;
}

void match(CALCTOKENS expectedToken) {
	if (tok.TokenNum == expectedToken) {
		if (tok.TokenNum != EOFSY) {
			tok = yylex();
		}
	}
	else {
		cout << "Token expected: " << TOKENMAP[expectedToken] << endl << "Token encountered: " << TOKENMAP[tok.TokenNum] << endl;
	}
}

void yyprogram() {
	sList();
	match(EOFSY);

	//cout << "CalcInterp Value: " << finalVal << endl;
	//return finalVal;
}

void sList() {
	if(tok.TokenNum != EOFSY) {
		stmt();
		sList();		
	}
}

void stmt() {
	// id := <expr>
	if (tok.TokenNum == ID) {
		//This operation can always be performed
		//as an assignment doesn't care about previous values
		string currID = tok.TokenString;
		match(ID);
		match(ASSIGNSY);
		//ID = result of expression
		symTable[currID] = expr();
		cout << "Assign:\t" << currID << " = " << symTable[currID] << endl;
	}
	// write <expr>
	else if (tok.TokenNum == WRITESY) {
		match(WRITESY);
		double toWrite = expr();
		cout << "Write:\t" << toWrite << endl;
	}
	// read id
	else if (tok.TokenNum == READYSY) {
		match(READYSY);
		string toRead;
		cout << "Read:\tEnter value for " << tok.TokenString << "> ";
		cin >> toRead;
		if(!isDouble(toRead)) {
			throw exception("Did not enter a valid number");
		}
		//Add symbol to the table
		symTable[tok.TokenString] = strtod(toRead.c_str(), NULL);
		match(ID);
		cin.clear();
	}
	else {		
		match(EOFSY);
		//throw exception("Invalid token read when encountering a statment");
	}
}

double expr() {
	double val1 = term();
	double val2 = tTail(val1);
	return val2;
}

double term() {
	double val1 = factor(); 
	double val2 = fTail(val1);
	return val2;
}

double tTail(double inval) {
	if (tok.TokenNum == ADDOP) {
		match(ADDOP);
		double val1 = term();
		double val2 = tTail(inval + val1);
		return val2;
	}
	else if (tok.TokenNum == MINOP) {
		match(MINOP);
		double val1 = term();
		double val2 = tTail(inval - val1);
		return val2;
	}
	return inval; //no tail
}

double factor() {
	if(tok.TokenNum == LPAREN) {
		match(LPAREN);
		double val = expr();
		match(RPAREN);
		return val;
	}
	else if (tok.TokenNum == NUMCONST) {
		//suitable conversion from string to double
		double val = strtod(tok.TokenString.c_str(), NULL);
		match(NUMCONST);
		return val;
	}
	else if (tok.TokenNum == ID) {
		//Return value of ID
		double val = symTable[tok.TokenString];
		match(ID);
		return val;

	}
	else {
		throw exception("Invalid token read when encountering a factor");
	}
}

double fTail(double inval) {
	if(tok.TokenNum == MULTOP) {
		match(MULTOP);
		double val1 = factor();
		double val2 = fTail(inval * val1);
		return val2;
	}
	else if(tok.TokenNum == DIVOP) {
		match(DIVOP);
		double val1 = factor();
		double val2 = fTail(inval / val1);
		return val2;
	}
	return inval;
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
bool setToken(Token& currToken, CALCTOKENS OP, string tString = "") {
	currToken.TokenNum = OP;
	if(tString.length() == 0) {
		currToken.TokenString = *currPos;
	}
	else {
		currToken.TokenString = tString;
	}
	if(OP != EOFSY) {
		currPos++;
	}
	return true;
}

//==YYLEX===
Token yylex() {
	bool tokenFound = false;	
	Token thisToken;
	thisToken.TokenString = "";
	do {
		//check against tokens		
		//If fell off the end of the inputfile
		if (currPos == inputFile.end()) {
			tokenFound = setToken(thisToken, EOFSY, "$$");
			endOfFile = true;
		}
		else if ((*currPos) == '\n' || (*currPos) == '\t' || (*currPos) == ' ') {
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
			char toAppend;
			do {
				toAppend = *currPos;
				thisToken.TokenString += toupper(toAppend);
				currPos++;
			} while (isalpha(*currPos) || isdigit(*currPos) || *currPos == '_');
			//tokenFound = true;	
			if (iequals(thisToken.TokenString, "read")) {
				tokenFound = setToken(thisToken, READYSY, "read");
			}
			else if (iequals(thisToken.TokenString, "write")) {
				tokenFound = setToken(thisToken, WRITESY, "write");
			}
			else {
				thisToken.TokenNum = ID;
				tokenFound = true;
			}			
		}
		//EOFSY
		else if (*currPos == '$') {
 			currPos++; //increment to check next value
 			if (*currPos == '$') {
 				tokenFound = setToken(thisToken, EOFSY, "$$");
 				endOfFile = true;
 			}
 			else //in case there is bad input after '$'
 				throw exception("Received an unexpected character after the initial character for EOFSY");
 		}
		//If a character found is not recognized
		else {
			throw exception("Received character was not recognized by the scanner");
		}
	} while (!tokenFound);
	//cout << "Token: " << setfill('0') << setw(2) << thisToken.TokenNum << " " << TOKENMAP[thisToken.TokenNum] << " (" << thisToken.TokenString << ")" << endl;
	tokenCount++;
	return thisToken;
}
