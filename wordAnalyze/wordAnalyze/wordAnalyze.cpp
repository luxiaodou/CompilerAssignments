// wordAnalyze.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "symDef.h"
#include <iostream>
#include <string>
using namespace std;

char ch;
string token;
int sym;
int num;
string input;
int index = -1;

void getinput(){		//获取输入并存入input中备用
	string temp = "";
	while (cin >> temp){
		input  = input.append(temp.append("\n")) ;
	}
}

void clearToken() {
	token = "";
}

void getch() {	//从input中获得下一个字符
	if (index < input.length() - 1)
	{
		ch = input.at(++index);
	}
	else {
		printf("Word Analyze complete\n");
		exit(0);
	}
}

void catToken() {
	token += ch;
}

void skip(){
	if (ch == ' ' || ch == '\n' || ch == '\t')
	{
		getch();
	} 
}

bool isLetter(){
	if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z' || ch == '_')
	{
		return true;
	}
	return false;
}

bool isDigit() {
	if (ch >= '0' && ch <= '9'){
		return true;		
	}
	return false;
}

void retract() {
	if (index >0)
		index --;
}

int isreserve() {
	if (token == "main")
		return MAINSYM;
	else if (token == "int")
		return INTSYM;
	else if (token == "char")
		return CHARSYM;
	else if (token == "const")
		return CONSTSYM;
	else if (token == "void")
		return VOIDSYM;
	else if (token == "if")
		return IFSYM;
	else if (token == "while")
		return WHILESYM;
	else if (token == "switch")
		return SWITCHSYM;
	else if (token == "case")
		return CASESYM;
	else if (token == "default")
		return DEFAULTSYM;
	else if (token == "scanf")
		return SCANFSYM;
	else if (token == "printf")
		return PRINTFSYM;
	else if (token == "return")
		return RETURNSYM;
	else return -1;
}


int getsym() {
	clearToken();
	getch();
	skip();
	if (isLetter())
	{
		while(isLetter() || isDigit()){
			catToken();
			getch();
		}
		retract();
		int result;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{	
	//getinput();
	string a = "test";
	char b = '!';
	a += b;
	bool cae = a == "test!";
	cout << cae<<endl;
	return 0;
}

