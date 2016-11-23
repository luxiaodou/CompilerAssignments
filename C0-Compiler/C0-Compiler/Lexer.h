#pragma once
#ifndef _Lexer_H_
#define _Lexer_H_
#include <fstream>
#include <string>
#include "defination.h"
using namespace std;
class Lexer {
public:
	static char ch;
	static string token, inputline, path;
	static int sym, num, line, count, ll, lc;
	static ifstream infile;
	static ofstream outfile;
	static void output();
	static int getsym();
	static void backup();
	static void retrieve();
	static void init();
	static void close();
private:
	static streampos bookmark;
	static int bline, bcount, bll, blc, bsym;
	static string binputline;
	static void getpath();
	static void clearToken();
	static void getnextline();
	static void getch();
	static void catToken();
	static void skip();
	static bool isLetter();
	static bool isDigit();
	static void retract();
	static int	isreserve();
	static bool isStringCon();
};
#endif