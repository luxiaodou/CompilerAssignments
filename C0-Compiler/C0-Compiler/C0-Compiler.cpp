// C0-Compiler.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "global.h"
#include "Lexer.h"
#include "Parser.h"

int errorNum;
vector<Quadruple> quadTable;
map<string, string> strTable;
int stbIndex;
Table constTable("consttab");
Table symbolTable("symboltab");	//声明符号表

int _tmain(int argc, _TCHAR* argv[])
{
	errorNum = 0;
	stbIndex = 0;
	//Lexer::getpath();
	//Lexer::path = "C:/Users/luxiaodou/Desktop/13071079_test.txt";
	Lexer::path = "13071079_test.txt";
	//path = "C:/Users/luxiaodou/Desktop/a.txt";
	Lexer::init();
	Parser::program();
	cout << "Syntax Analyze complete!" << endl;
	cout << "err:" << errorNum << endl;
	Lexer::close();
	
	return 0;
}

