// C0-Compiler.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "global.h"
#include "Lexer.h"
#include "Parser.h"

int errornum;
vector<Quadruple> quadtable;
map<string, string> strtable;
int stbindex;
Table constTable("consttab");
Table symbolTable("symtab");

int _tmain(int argc, _TCHAR* argv[])
{
	errornum = 0;
	stbindex = 0;
	//Lexer::getpath();
	//Lexer::path = "C:/Users/luxiaodou/Desktop/13071079_test.txt";
	Lexer::path = "13071079_test.txt";
	//path = "C:/Users/luxiaodou/Desktop/a.txt";
	Lexer::init();
	Parser::program();
	cout << "Syntax Analyze complete!" << endl;
	cout << "err:" << errornum << endl;
	Lexer::close();
	return 0;
}

