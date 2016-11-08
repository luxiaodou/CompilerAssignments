// wordAnalyze.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "symDef.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

char ch;
string token;
int sym;
int num;
string input;
int index = -1;
int count = 1;

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
	if (index < int(input.length())-1)
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

bool isStringCon()
{
	if ((ch >=32 || ch <= 126) && ch != 34)
		return true;
	else return false;
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
		result = isreserve();
		if(result == 0)
			sym = IDSYM;
		else sym = result;
	}
	else if (isDigit() && ch != '0')	//第一个数字不为零
	{
		while (isDigit())
		{
			catToken();
			getch();
		}
		retract();
		if (token.length() > 30)
		{
			cout << "integer is out of limit! " << endl;
			exit(1);
		}
		num = stoi(token);
		sym = NUMSYM;
	}
	else if (ch == '\'')
		sym = QUOTE;
	else if(ch == '\"')
		sym = DOUQUOTE;
	else if (ch == '+')
		sym = PLUSSYM;
	else if (ch == '-')
		sym = MINUSSYM;
	else if (ch == '*')
		sym = MULTISYM;
	else if (ch == '/')
		sym = DEVIDESYM;
	else if (ch == '[')
		sym = LBRACK;
	else if (ch == ']')
		sym = RBRACK;
	else if(ch == '(')
		sym = LPARENT;
	else if (ch == ')')
		sym = RPARENT;
	else if (ch == '{')
		sym = LBRACE;
	else if (ch == '}')
		sym = RBRACE	;
	else if (ch == ':')
		sym = COLON;
	else if(ch == ';')
		sym = SEMICOLON;
	else if (ch == '=')
	{
		skip();
		getch();
		if(ch == '=')
			sym = EQUAL;
		else
		{
			retract();
			sym = ASSIGN;
		}
	}
	else if (ch == ',')
		return COMMA;
	else if (ch == '<')
	{
		skip();
		getch();
		if (ch == '=')
		{
			sym = LESSEQU;
		}
		else
		{
			retract();
			sym = LESS;
		}
	}
	else if (ch == '>')
	{
		skip();
		getch();
		if (ch == '=')
			sym = GREATEQU;
		else
		{
			retract();
			sym = GREAT;
		}
	}
	else if (ch == '!') {
		skip();
		getch();
		if(ch == '=')
			sym = NEQUAL;
		else {
			retract();
			cout<< "error using '!' " << endl;
		}
	}
	return 0;
}

void output(){
	switch(sym) {
	case IDSYM:	
		cout<< count++ <<" IDSYM "+ token + ";"<< endl;
		break;
	case NUMSYM:
		cout << count++ << " NUMSYM  ;" << endl;
		break;
	case PLUSSYM:
		cout << count++ << " PLUSSYM ;" << endl;
	case MINUSSYM:
		cout << count++ << " MINUSSYM ;" << endl;
	case MULTISYM:
		cout << count++ << " MULTISYM ;" << endl;
	case DEVIDESYM:
		cout << count++ << " DEVIDESYM ;" << endl;
	case QUOTE:
		cout << count++ << " QUOTE  ;" << endl;
	case LBRACK:
		cout << count++ << " LBRACK  ;" << endl;
	case RBRACK:
		cout << count++ << " RBRACK  ;" << endl;
	case LPARENT:
		cout << count++ << " LPARENT  ;" << endl;
	case RPARENT:
		cout << count++ << " RPARENT  ;" << endl;
	case LBRACE:
		cout << count++ << " LBRACE  ;" << endl;
	case COLON:
		cout << count++ << " COLON  ;" << endl;
	case COMMA:
		cout << count++ << " COMMA  ;" << endl;
	case SEMICOLON:
		cout << count++ << " SEMICOLON  ;" << endl;
	case EQUAL:
		cout << count++ << " EQUAL  ;" << endl;
	case ASSIGN:
		cout << count++ << " ASSIGN  ;" << endl;
	case LESS:
		cout << count++ << " LESS  ;" << endl;
	case LESSEQU:
		cout << count++ << " LESSEQU  ;" << endl;
	case GREAT:
		cout << count++ << " GREAT  ;" << endl;
	case GREATEQU:
		cout << count++ << " GREATEQU  ;" << endl;
	case NEQUAL:
		cout << count++ << " NEQUAL  ;" << endl;
	case DOUQUOTE:
		cout << count++ << " DOUQUOTE  ;" << endl;
	case MAINSYM:
		cout << count++ << " MAINSYM  ;" << endl;
	case INTSYM:
		cout << count++ << " INTSYM  ;" << endl;
	case CHARSYM:
		cout << count++ << " CHARSYM  ;" << endl;
	case CONSTSYM:
		cout << count++ << " CONSTSYM  ;" << endl;
	case VOIDSYM:
		cout << count++ << " VOIDSYM  ;" << endl;
	case IFSYM:
		cout << count++ << " IFSYM  ;" << endl;
	case WHILESYM:
		cout << count++ << " WHILESYM  ;" << endl;
	case SWITCHSYM:
		cout << count++ << " SWITCHSYM  ;" << endl;
	case CASESYM:
		cout << count++ << " CASESYM  ;" << endl;
	case DEFAULTSYM:
		cout << count++ << " DEFAULTSYM  ;" << endl;
	case SCANFSYM:
		cout << count++ << " SCANFSYM  ;" << endl;
	case PRINTFSYM:
		cout << count++ << " PRINTFSYM  ;" << endl;
	case RETURNSYM:
		cout << count++ << " RETURNSYM  ;" << endl;	
	}
}

int _tmain(int argc, _TCHAR* argv[])
{	
	//getinput();
	input = "main ";
	while (index < int(input.length())){
		getsym();
		cout << sym << endl;
		//output();
	}
}

