// C0-Compiler.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "global.h"
#include "Lexer.h"
#include "Parser.h"
#include "Generator.h"
#include "Table.h"

int errorNum;
int stbIndex;
vector<Quadruple> quadTable;
map<string, string> strTable;
Table symbolTable("symboltab");	//声明符号表

int _tmain(int argc, _TCHAR* argv[])
{
	errorNum = 0;
	stbIndex = 0;
	strTable["$enter"] = "\\n";
	//Lexer::getpath();
	//Lexer::path = "C:/Users/luxiaodou/Desktop/13071079_test.txt";
	//path = "C:/Users/luxiaodou/Desktop/a.txt";
	Lexer::path = "13071079_test.txt";
	ofstream midfile;
	midfile.open("13071079_test_mid.txt");
	Lexer::init();
	Parser::program();
	cout << "Syntax Analyze complete!" << endl;
	cout << "err:" << errorNum << endl;
	for (int i = 0; i < quadTable.size(); i++) {
		Quadruple q = quadTable[i];
		midfile << q.op << " " << q.para1 << " "<< q.para2 << " "<< q.para3 << endl;
	}
	Generator gen("13071079_test_asm.txt");
	gen.work();
	Lexer::close();
	gen.close();
	midfile.close();
	//string s20 = "1234567890123";
	//cout << _atoi64(s20.c_str());
	return 0;
}

