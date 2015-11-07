#ifndef H_calcLex
#define H_calclex

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>
#include <map>

using namespace std;

enum CALCTOKENS {
	EOFSY = 0, //End of file
	ID, //ID
	NUMCONST, //number constant
	READYSY, //read keyword
	WRITESY, //write keyword
	ASSIGNSY, //assignment symbol ':=' 
	LPAREN, //left parenthesis '('
	RPAREN, //right parenthesis ')'
	ADDOP, //addition sign '+'
	MINOP, //minus sign '-'
	MULTOP, //multiplication sign '*'
	DIVOP, //division sign '/'
	/*DOT = 12, //dot symbol '.'
	//don't know that I need these 
	RCOMM = 13, //right comment symbol '/*'
	LCOMM = 14, //left comment symbol '*/
	
};

map<CALCTOKENS, string> TOKENMAP;

void setupMap () {
	TOKENMAP[EOFSY] = "EOFSY";
	TOKENMAP[ID] = "ID";
	TOKENMAP[NUMCONST] = "NUMCONST";
	TOKENMAP[READYSY] = "READYSY";
	TOKENMAP[WRITESY] = "WRITESY";
	TOKENMAP[ASSIGNSY] = "ASSIGNSY";
	TOKENMAP[LPAREN] = "LPAREN";
	TOKENMAP[RPAREN] = "RPAREN";
	TOKENMAP[ADDOP] = "ADDOP";
	TOKENMAP[MINOP] = "MINOP";
	TOKENMAP[MULTOP] = "MULTOP";
	TOKENMAP[DIVOP] = "DIVOP";
};



struct Token {
	CALCTOKENS TokenNum;
	string TokenString;
};


#endif