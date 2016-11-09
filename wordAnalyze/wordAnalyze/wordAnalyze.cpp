// wordAnalyze.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "symDef.h"
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

void getinput(){		//获取输入并存入input中备用
	string temp = "";
	while (cin >> temp){
		input  = input.append(temp.append("\n")) ;
	}
}

void getpath()
{
	cin >> path;
}

void clearToken() {
	token = "";
}

//void getch() {	//从input中获得下一个字符
//	if (index < int(input.length()))
//	{
//		ch = input.at(index++);
//	}
//	else
//	{
//		ch = 0;
//		index ++;
//	}	
//}

void getch()
{
	inputfile.get(ch);
	if (inputfile.eof())
	{
		ch =0;
		sym = 0;
	}
}

void catToken() {
	token += ch;
}

void skip(){
	while (ch == ' ' || ch == '\n' || ch == '\t')
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
	inputfile.unget();
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
	else return 0;
}

bool isStringCon()
{
	if ((ch >=32 || ch <= 126) && ch != 34)
		return true;
	else return false;
}

int getsym() {
	sym = 0;
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
	else if (isDigit())	
	{
		while (isDigit() && ch != 0)
		{
			if (ch == '0')
			{
				catToken();
				getch();
				if (isDigit())
				{
					cout << "zero cannot be the first digit of number! " << endl;
					while (isDigit())
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
				catToken();
				getch();
			}			
		}
		if (isDigit() || isLetter())
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
		if (token.length() > 30)		//数字不超过30位
		{
			cout << "integer is out of limit! " << endl;
			exit(1);
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
	//cin >> path;
	//path = "C:/Users/luxiaodou/Desktop/13071079_test.txt";
	path = "C:/Users/luxiaodou/Desktop/a.txt";
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

