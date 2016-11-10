// wordAnalyze.cpp : 定义控制台应用程序的入口点。
//	词法分析主程序

#include "stdafx.h"
#include "symDef.h"
#include "typDef.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

char ch;
string token;
int sym;
int num;
string input;
string path;
int index = 0;
int count = 1;
ofstream outfile("output.txt");
fstream inputfile;
void output();

//用来获取输入文件的路径
void getpath()
{
	cin >> path;
}

//清空token
void clearToken() {
	token = "";
}

//获得下一个输入字符，当读到文件末尾的时候返回空字符
void getch()
{
	inputfile.get(ch);
	if (inputfile.eof())
	{
		ch =0;
		sym = 0;
	}
}

//拼接token
void catToken() {
	token += ch;
}

//跳过无意义的空白字符，本文法中针对空格，换行符以及水平制表符
void skip(){
	while (ch == ' ' || ch == '\n' || ch == '\t')
	{
		getch();
	} 
}

//判断当前ch是否是字母类型，注意根据文法定义下划线'_'也算作字母
bool isLetter(){
	if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z' || ch == '_')
	{
		return true;
	}
	return false;
}

//判断ch是否是数字
bool isDigit() {
	if (ch >= '0' && ch <= '9'){
		return true;		
	}
	return false;
}

//读字符的指针退格
void retract() {
	inputfile.unget();
}

//分辨当前标识符是否是保留字，是的话返回对应的sym，不是返回0说明是标识符
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
	else return 0;
}

//检查当前ch是不是组成字符串的合法元素，文法定义为“十进制编码为32,33,35-126的ASCII字符”
bool isStringCon()
{
	if ((ch >=32 || ch <= 126) && ch != 34)
		return true;
	else return false;
}

//词法分析主要部分，判别当前单词的类型
int getsym() {
	sym = 0;	//默认sym为0，在output()中不作处理
	clearToken();
	getch();
	skip();
	if (isLetter())
	{
		while(isLetter() || isDigit()){
			catToken();
			getch();
		}
		retract();	//注意这里预读了下一个ch，为了防止下次执行getsym时读过，需要退格
		if (token.length() > IDENT_MAXLENGTH)	//判断标识符长度是否超过限制， 超过则报错
		{
			cout << "your ident is too long, please make it short!" << endl;
			sym = 0;
			return 0;
		}
		int result;
		result = isreserve();
		if(result == 0)
			sym = IDSYM;
		else sym = result;
	}
	else if (isDigit())	//处理数字
	{
		if (ch == '0')	//零开头的时候需要注意，因为文法不支持前导零
		{
			catToken();
			getch();
			if (isDigit())
			{
				cout << "zero cannot be the first digit of number! " << endl;
				while (isDigit() || isLetter())
				{
					getch();
				}
				sym = 0;
				return 0;
			}
			else
			{
				num = 0;
				sym = NUMSYM;
			}
		} 
		else
		{
			while (isDigit() && ch != 0)
			{
				catToken();
				getch();
			}
		}
		
		if (isDigit() || isLetter())		//检查后面是否接了字母表示标识符
		{
			while (isLetter() || isDigit())
			{
				getch();
			}
			cout << "IDENT cannot start with numbers!" << endl;
			sym = 0;
			return 0;
		}		
		retract();
		if (token.length() > NUM_MAXLENGTH)		//数字不超过30位
		{
			cout << "integer is out of limit! " << endl;
			return 0;
		}
		num = stoi(token);
		sym = NUMSYM;
	}
	else if (ch == '\'')	//处理字符
	{
		sym = QUOTE;		
		getch();
		if (isLetter() || ch == '+' || ch == '-' || ch == '*' || ch == '/' || isDigit())
		{
			output();
			sym = CHARTY;
			catToken();
			getch();
			if (ch != '\'')
			{
				retract();
				cout << "char type error! missing ' !" << endl;
			}
			else
			{
				output();
				sym = QUOTE;
			}
		}
		else
		{
			retract();
			cout << "char type error! The content is not acceptable! " << endl;
		}
	}
	else if(ch == '\"')
	{
		sym = DOUQUOTE;
		output();
		getch();
		if (ch == '\"')
		{
			sym = DOUQUOTE;
			return 0;
		}
		while(ch != '\"' && isStringCon() && ch != 0)
		{
			catToken();
			getch();
		}
		if (ch == '\"')
		{
			if (token.length() > STRING_MAXLENGTH)
			{
				cout << "Your String is too long, please make it short!" << endl;
				sym = 0;
				return 0;
			}
			sym = STRING;
			output();
			sym = DOUQUOTE;
		}
		else
		{
			cout<<"string content error! Please check your input! " << endl;
			sym = 0;
			/*	if (ch == 0)
			{
			return 0;
			}			*/
			while(ch != '\"' && ch !=0)
				getch();	//跳过无用字符
		}
	}
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
		sym =  COMMA;
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
		outfile<< count++ <<" IDSYM "+ token + ";"<< endl;	break;
	case NUMSYM:
		outfile << count++ << " NUMSYM  " << num << ";"<< endl;	 break;
	case CHARTY:
		outfile << count++ << " CHARTY " << token << ";" << endl; break;
	case STRING:
		outfile << count++ << " STRING " << token << ";" << endl; break;
	case PLUSSYM:
		outfile << count++ << " PLUSSYM ;" << endl; break;
	case MINUSSYM:
		outfile << count++ << " MINUSSYM ;" << endl; break;
	case MULTISYM:
		outfile << count++ << " MULTISYM ;" << endl; break;
	case DEVIDESYM:
		outfile << count++ << " DEVIDESYM ;" << endl; break;
	case QUOTE:
		outfile << count++ << " QUOTE  ;" << endl; break;
	case LBRACK:
		outfile << count++ << " LBRACK  ;" << endl; break;
	case RBRACK:
		outfile << count++ << " RBRACK  ;" << endl; break;
	case LPARENT:
		outfile << count++ << " LPARENT  ;" << endl; break;
	case RPARENT:
		outfile << count++ << " RPARENT  ;" << endl; break;
	case LBRACE:
		outfile << count++ << " LBRACE  ;" << endl; break;
	case RBRACE:
		outfile << count++ << " RBRACE  ;"<< endl; break;
	case COLON:
		outfile << count++ << " COLON  ;" << endl; break;
	case COMMA:
		outfile << count++ << " COMMA  ;" << endl; break;
	case SEMICOLON:
		outfile << count++ << " SEMICOLON  ;" << endl; break;
	case EQUAL:
		outfile << count++ << " EQUAL  ;" << endl; break;
	case ASSIGN:
		outfile << count++ << " ASSIGN  ;" << endl; break;
	case LESS:
		outfile << count++ << " LESS  ;" << endl; break;
	case LESSEQU:
		outfile << count++ << " LESSEQU  ;" << endl; break;
	case GREAT:
		outfile << count++ << " GREAT  ;" << endl; break;
	case GREATEQU:
		outfile << count++ << " GREATEQU  ;" << endl; break;
	case NEQUAL:
		outfile << count++ << " NEQUAL  ;" << endl; break;
	case DOUQUOTE:
		outfile << count++ << " DOUQUOTE  ;" << endl; break;
	case MAINSYM:
		outfile << count++ << " MAINSYM  ;" << endl; break;
	case INTSYM:
		outfile << count++ << " INTSYM  ;" << endl; break;
	case CHARSYM:
		outfile << count++ << " CHARSYM  ;" << endl; break;
	case CONSTSYM:
		outfile << count++ << " CONSTSYM  ;" << endl; break;
	case VOIDSYM:
		outfile << count++ << " VOIDSYM  ;" << endl; break;
	case IFSYM:
		outfile << count++ << " IFSYM  ;" << endl; break;
	case WHILESYM:
		outfile << count++ << " WHILESYM  ;" << endl; break;
	case SWITCHSYM:
		outfile << count++ << " SWITCHSYM  ;" << endl; break;
	case CASESYM:
		outfile << count++ << " CASESYM  ;" << endl; break;
	case DEFAULTSYM:
		outfile << count++ << " DEFAULTSYM  ;" << endl; break;
	case SCANFSYM:
		outfile << count++ << " SCANFSYM  ;" << endl; break;
	case PRINTFSYM:
		outfile << count++ << " PRINTFSYM  ;" << endl; break;
	case RETURNSYM:
		outfile << count++ << " RETURNSYM  ;" << endl; break;	
	case 0:
		break;
	default: outfile << "sym type not defined!" << endl;		 
	}
}

int _tmain(int argc, _TCHAR* argv[])
{		
	getpath();
	//path = "C:/Users/luxiaodou/Desktop/13071079_test.txt";
	//path = "C:/Users/luxiaodou/Desktop/a.txt";
	inputfile.open(path,ios::in);
	while (!inputfile.eof())
	{
		getsym();
		/*if (ch == 0)
		{
		continue;
		}*/
		output();
	}
	inputfile.close();
	cout << "Word Analyze complete!" << endl;
	outfile.close();
	return 0;
}