#ifndef H_calcLex
#define H_calclex

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>

using namespace std;

enum CALCTOKENS {
	EOFSY = 0, //End of file
	ID = 1, //ID
	NUMCONST = 2, //number constant
	READYSY = 3, //read keyword
	WRITESY = 4, //write keyword
	ASSIGNSY = 5, //assignment symbol ':=' 
	LPAREN = 6, //left parenthesis '('
	RPAREN = 7, //right parenthesis ')'
	ADDOP = 8, //addition sign '+'
	MINOP = 9, //minus sign '-'
	MULTOP = 10, //multiplication sign '*'
	DIVOP = 11, //division sign '/'
	/*DOT = 12, //dot symbol '.'
	//don't know that I need these 
	RCOMM = 13, //right comment symbol '/*'
	LCOMM = 14, //left comment symbol '*/
	
};

struct Token {
	CALCTOKENS TokenNum;
	string TokenString;
};


#endif