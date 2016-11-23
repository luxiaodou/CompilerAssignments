#include "stdafx.h"
#include "global.h"
#include "Parser.h"
#include "Lexer.h"

int tab = 0;

void tabf() {
	int i = 0;
	while (i < tab) {
		cout << "|---";
		i++;
	}
}

int ttype = 0, tident = 0;

void Parser::enter() {

}

//＜程序＞::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void Parser::program() {
	/*tab += 1;*/
	tabf();
	cout << "program" << endl;

	Lexer::getsym();
	//处理常量声明
	if (Lexer::sym == CONSTSYM) {
		conststate();
	}

	//处理变量声明
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {	//需要预读判断是变量定义还是有返回值函数定义
		Lexer::backup();
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		Lexer::getsym();

		if (Lexer::sym == COMMA || Lexer::sym == LBRACK) {		//变量
			Lexer::retrieve();
			varstate();		//所有变量声明理应在这里处理完毕
		}
		else if (Lexer::sym == LPARENT) {
			Lexer::retrieve();
			funcdef();		//注意这里只处理了一个有返回值的函数
		}
	}
	else {
		error(0);
	}

	//处理函数声明
	while (Lexer::sym == INTSYM || Lexer::sym == CHARSYM || Lexer::sym == VOIDSYM) {
		if (Lexer::sym == VOIDSYM) {
			Lexer::backup();
			Lexer::getsym();
			if (Lexer::sym == MAINSYM) {
				Lexer::retrieve();
				break;
			}
			if (Lexer::sym != IDSYM) {
				error(MISSING_IDENT);
			}
			Lexer::getsym();
			if (Lexer::sym == LPARENT) {
				Lexer::retrieve();
				voidfdef();
			}
			else
			{
				error(MISSING_LPARENT);
			}
		}
		else {
			Lexer::backup();
			Lexer::getsym();
			if (Lexer::sym != IDSYM) {
				error(MISSING_IDENT);
			}
			Lexer::getsym();
			if (Lexer::sym == LPARENT) {
				Lexer::retrieve();
				funcdef();
			}
			else
			{
				error(MISSING_LPARENT);
			}
		}
	}

	//处理main函数
	if (Lexer::sym == VOIDSYM) {
		mainfdef();
	}
	else
	{
		error(0);
	}

	//cout << "This is a program!" << endl;
	//tab--;
}

//＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void Parser::conststate() {
	tab++;
	tabf();
	cout << "const statement!" << endl;
	do
	{
		if (Lexer::sym != CONSTSYM) {
			error(0);
		}
		Lexer::getsym();
		constdef();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		Lexer::getsym();
	} while (Lexer::sym == CONSTSYM);
	tab--;
	//cout << "this is a const statement!" << endl;
}

//＜常量定义＞ ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//					| char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }
void Parser::constdef() {
	tab++;
	tabf();
	cout << "const defination" << endl;
	if (Lexer::sym == INTSYM) {
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		Lexer::getsym();
		if (Lexer::sym != ASSIGN) {
			error(MISSING_ASSIGN);
		}
		Lexer::getsym();
		if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {	//＜加法运算符＞ ::= +｜-
			Lexer::getsym();
		}
		if (Lexer::sym == NUMSYM) {		//＜整数＞::= ［＋｜－］＜无符号整数＞｜０

		}
		else
		{
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		while (Lexer::sym == COMMA) {
			Lexer::getsym();
			if (Lexer::sym != IDSYM) {
				error(MISSING_IDENT);
			}
			Lexer::getsym();
			if (Lexer::sym != ASSIGN) {
				error(MISSING_ASSIGN);
			}
			Lexer::getsym();
			if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
				Lexer::getsym();
			}
			if (Lexer::sym != NUMSYM) {
				error(WRONG_TYPE);
			}
			Lexer::getsym();
		}
	}
	else if (Lexer::sym == CHARSYM) {
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		Lexer::getsym();
		if (Lexer::sym != ASSIGN) {
			error(MISSING_ASSIGN);
		}
		Lexer::getsym();
		if (Lexer::sym != CHARTY) {
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		while (Lexer::sym == COMMA) {
			Lexer::getsym();
			if (Lexer::sym != IDSYM) {
				error(MISSING_IDENT);
			}
			Lexer::getsym();
			if (Lexer::sym != ASSIGN) {
				error(MISSING_ASSIGN);
			}
			Lexer::getsym();
			if (Lexer::sym != CHARTY) {
				error(WRONG_TYPE);
			}
			Lexer::getsym();
		}
	}
	tab--;
	//cout << "this is a const defination!" << endl;
}

//＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void Parser::varstate() {
	tab++;
	tabf();
	cout << "var statement" << endl;
	vardef();
	if (Lexer::sym != SEMICOLON) {
		error(MISSING_SEMICOLON);
	}
	Lexer::getsym();
	while (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		Lexer::backup();
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		Lexer::getsym();
		if (Lexer::sym == COMMA || Lexer::sym == LBRACK || Lexer::sym == SEMICOLON) {	//not proved
			Lexer::retrieve();
			vardef();
		}
		else if (Lexer::sym == LPARENT || Lexer::sym == VOIDSYM) {	//不再是变量声明
			Lexer::retrieve();
			break;
		}
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		Lexer::getsym();
	}
	tab--;
	//cout << "This is a var state!" << endl;
}

//＜变量定义＞::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}
void Parser::vardef() {
	tab++;
	tabf();
	cout << "var defination" << endl;
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		Lexer::getsym();
		if (Lexer::sym == IDSYM) {
			Lexer::getsym();
			if (Lexer::sym == LBRACK) {
				Lexer::getsym();
				if (Lexer::sym != NUMSYM) {
					error(WRONG_TYPE);
				}
				else
				{
					//处理无符号整数
				}
				Lexer::getsym();
				if (Lexer::sym != RBRACK)
				{
					error(MISSING_RBRACK);
				}
				Lexer::getsym();
			}
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				if (Lexer::sym == IDSYM) {
					Lexer::getsym();
					if (Lexer::sym == LBRACK) {
						Lexer::getsym();
						if (Lexer::sym != NUMSYM) {
							error(WRONG_TYPE);
						}
						else {
							//deal with no-sign number
						}
						Lexer::getsym();
						if (Lexer::sym != RBRACK) {
							error(MISSING_RBRACK);
						}
						Lexer::getsym();
					}
				}
				else {
					error(MISSING_IDENT);
				}
			}
		}
		else
			error(MISSING_IDENT);
	}
	else {
		error(0);
	}
	tab--;
	//cout << "This is a var defination" << endl;
}

//＜参数表＞ ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
void Parser::paralist() {
	tab++;
	tabf();
	cout << "paralist" << endl;
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		Lexer::getsym();
		if (Lexer::sym == IDSYM) {
			Lexer::getsym();
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				if (Lexer::sym != INTSYM && Lexer::sym != CHARSYM) {
					error(WRONG_TYPE);
				}
				Lexer::getsym();
				if (Lexer::sym == IDSYM) {
					//not implemented
				}
				else
				{
					error(MISSING_IDENT);
				}
				Lexer::getsym();
			}
		}
	}
	tab--;
	//cout << "This is a paralist!" << endl;
}

//＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void Parser::compound() {
	tab++;
	tabf();
	cout << "compound" << endl;
	if (Lexer::sym == CONSTSYM) {
		conststate();
	}
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		varstate();
	}
	while (Lexer::sym == IFSYM || Lexer::sym == WHILESYM || Lexer::sym == LBRACE || Lexer::sym == INTSYM || Lexer::sym == CHARSYM || Lexer::sym == VOIDSYM
		|| Lexer::sym == IDSYM || Lexer::sym == PRINTFSYM || Lexer::sym == SCANFSYM || Lexer::sym == RETURNSYM || Lexer::sym == SEMICOLON || Lexer::sym == SWITCHSYM) {
		statement();
	}
	tab--;
	//cout << "This is a compound statement!" << endl;
}

//＜有返回值函数定义＞:: = ＜声明头部＞‘(’＜参数表＞‘)’ ‘ { ’＜复合语句＞‘ }’
void Parser::funcdef() {
	tab++;
	tabf();
	cout << "funcdef" << endl;
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		Lexer::getsym();
		if (Lexer::sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		Lexer::getsym();
		paralist();
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
		if (Lexer::sym != LBRACE) {
			error(MISSING_LBRACE);
		}
		Lexer::getsym();
		compound();
		if (Lexer::sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		Lexer::getsym();
	}
	else
	{
		error(WRONG_TYPE);
	}
	tab--;
	//cout << "This is a fuction defination!" << endl;
}

//＜无返回值函数定义＞:: = void＜标识符＞‘(’＜参数表＞‘)’‘ { ’＜复合语句＞‘ }’
void Parser::voidfdef() {
	tab++;
	tabf();
	cout << "voidfdef" << endl;
	if (Lexer::sym != VOIDSYM) {
		error(MISSING_VOID);
	}
	Lexer::getsym();
	if (Lexer::sym == IDSYM) {
		Lexer::getsym();
		if (Lexer::sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		Lexer::getsym();
		paralist();
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
		if (Lexer::sym != LBRACE) {
			error(MISSING_LBRACE);
		}
		Lexer::getsym();
		compound();
		if (Lexer::sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		Lexer::getsym();
	}
	else
	{
		error(MISSING_IDENT);
	}
	tab--;
	//cout << "This is a void function define!" << endl;
}

//＜主函数＞ ::= void main‘(’‘)’‘{’＜复合语句＞‘}’
void Parser::mainfdef() {
	tab++;
	tabf();
	cout << "main" << endl;
	if (Lexer::sym != VOIDSYM) {
		error(MISSING_VOID);
	}
	Lexer::getsym();
	if (Lexer::sym != MAINSYM) {
		error(0);
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	if (Lexer::sym != LBRACE) {
		error(MISSING_LBRACE);
	}
	Lexer::getsym();
	compound();
	if (Lexer::sym != RBRACE) {
		error(MISSING_RBRACE);
	}
	//cout << "This is a main function" << endl;
	tab--;
}


//＜语句＞:: = ＜条件语句＞｜＜循环语句＞ | ‘{ ’＜语句列＞‘ }’｜＜有返回值函数调用语句＞;
//				| ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜情况语句＞｜＜返回语句＞;
void Parser::statement() {
	tab++;
	tabf();
	cout << "statement" << endl;
	switch (Lexer::sym)
	{
	case IFSYM: //ifstate
		ifstate();
		break;
	case WHILESYM:	//while state
		whilestate();
		break;
	case LBRACE:	//statement list
		Lexer::getsym();
		while (Lexer::sym == IFSYM || Lexer::sym == WHILESYM || Lexer::sym == LBRACE || Lexer::sym == INTSYM || Lexer::sym == CHARSYM || Lexer::sym == VOIDSYM
			|| Lexer::sym == IDSYM || Lexer::sym == PRINTFSYM || Lexer::sym == SCANFSYM || Lexer::sym == RETURNSYM || Lexer::sym == SEMICOLON || Lexer::sym == SWITCHSYM) {
			statement();
		}
		if (Lexer::sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		Lexer::getsym();
		break;
	case IDSYM:	//assignment or callfunc
		Lexer::backup();
		Lexer::getsym();
		if (Lexer::sym == LPARENT) {
			Lexer::retrieve();
			calfunc();
		}
		else
		{
			Lexer::retrieve();
			assignstate();			
		}
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		Lexer::getsym();
		break;
	case PRINTFSYM:
		printfstate();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		Lexer::getsym();
		break;
	case SCANFSYM:
		scanfstate();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		Lexer::getsym();
		break;
	case SEMICOLON:
		Lexer::getsym();
		break;
	case SWITCHSYM:
		switchstate();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		Lexer::getsym();
		break;
	case RETURNSYM:
		returnstate();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		Lexer::getsym();
		break;
	default:
		break;
	}
	//cout << "This is a statement statement!" << endl;
	tab--;
}

//＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
void Parser::expression() {
	tab++;
	tabf();
	cout << "expression" << endl;
	if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
		Lexer::getsym();
	}
	term();
	while (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
		Lexer::getsym();
		term();
	}
	//cout << "This is a expression!" << endl;
	tab--;
}

//＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Parser::term() {
	tab++;
	tabf();
	cout << "term" << endl;
	factor();
	while (Lexer::sym == MULTISYM || Lexer::sym == DEVIDESYM) {
		Lexer::getsym();
		factor();
	}
	//cout << "This is a term!" << endl;
	tab--;
}

//＜因子＞ ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’|‘(’＜表达式＞‘)’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞ 
void Parser::factor() {
	tab++;
	tabf();
	cout << "factor" << endl;
	switch (Lexer::sym)
	{
	case IDSYM:
		Lexer::backup();
		Lexer::getsym();
		if (Lexer::sym == LBRACK) {
			Lexer::getsym();
			expression();
			if (Lexer::sym != RBRACK) {
				error(MISSING_RBRACK);
			}
			Lexer::getsym();
		}
		else if (Lexer::sym == LPARENT) {	//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
			//Lexer::getsym();	//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
			//if (Lexer::sym == IDSYM || Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
			//	expression();
			//	while (Lexer::sym == COMMA) {
			//		Lexer::getsym();
			//		expression();
			//	}
			//	if (Lexer::sym != RPARENT)
			//	{
			//		error(MISSING_RPARENT);
			//	}
			//	Lexer::getsym();
			//}
			Lexer::retrieve();
			calfunc();
		}
		break;
	case LPARENT:
		Lexer::getsym();
		expression();
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
		break;
	case PLUSSYM:
	case MINUSSYM:
		Lexer::getsym();
		if (Lexer::sym == NUMSYM) {
			//not implement yet!
		}
		else
		{
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		break;
		break;
	case NUMSYM:
		//not implement!
		Lexer::getsym();
		break;
	case QUOTE:
		Lexer::getsym();
		if (Lexer::sym != CHARTY) {
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		if (Lexer::sym != QUOTE) {
			error(CHAR_MISS_QUOTE);
		}
		Lexer::getsym();
		break;
	default:
		error(0);
		break;
	}
	//cout << "This is a factor!" << endl;
	tab--;
}

//＜条件语句＞:: = if ‘(’＜条件＞‘)’＜语句＞
void Parser::ifstate() {
	tab++;
	tabf();
	cout << "if statement" << endl;
	if (Lexer::sym != IFSYM) {
		error(0);
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	condition();
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	statement();
	//cout << "This is an if statement!" << endl;
	tab--;
}

//＜条件＞::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
void Parser::condition()
{
	tab++;
	tabf();
	cout << "condition" << endl;
	expression();
	if (Lexer::sym == LESS || Lexer::sym == LESSEQU || Lexer::sym == GREAT || Lexer::sym == GREATEQU || Lexer::sym == NEQUAL || Lexer::sym == EQUAL) {
		Lexer::getsym();
		expression();
	}
	else
	{
		error(0);
	}
	//cout << "This is a condition statement!" << endl;
	tab--;
}

//＜赋值语句＞:: = ＜标识符＞＝＜表达式＞ | ＜标识符＞‘[’＜表达式＞‘]’ = ＜表达式＞
void Parser::assignstate() {
	tab++;
	tabf();
	cout << "assign statement" << endl;
	if (Lexer::sym != IDSYM) {
		error(MISSING_IDENT);
	}
	Lexer::getsym();
	if (Lexer::sym == LBRACK) {
		Lexer::getsym();
		expression();
		if (Lexer::sym != RBRACK) {
			error(MISSING_RBRACK);
		}
		Lexer::getsym();
	}
	if (Lexer::sym != ASSIGN) {
		error(0);
	}
	Lexer::getsym();
	expression();
	//cout << "This is an assign statement!" << endl;
	tab--;
}

//＜循环语句＞:: = while ‘(’＜条件＞‘)’＜语句＞
void Parser::whilestate() {
	tab++;
	tabf();
	cout << "while statement" << endl;
	if (Lexer::sym != WHILESYM) {
		error(0);
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	condition();
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	statement();
	//cout << "This is a while statement!" << endl;
	tab--;
}

//＜情况语句＞:: = switch ‘(’＜表达式＞‘)’ ‘ { ’＜情况表＞＜缺省＞ ‘ }’
void Parser::switchstate() {
	tab++;
	tabf();
	cout << "switch statement" << endl;
	if (Lexer::sym != SWITCHSYM) {
		error(0);
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	expression();
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	if (Lexer::sym == LBRACE) {		//＜情况表＞::=  ＜情况子语句＞{＜情况子语句＞}
		Lexer::getsym();
		while (Lexer::sym == CASESYM) {
			casestate();
		}
		defaultstate();
	}
	if (Lexer::sym != RBRACE) {
		error(MISSING_RBRACE);
	}
	//cout << "This is a switch statement!" << endl;
	tab--;
}

//＜情况子语句＞:: = case＜常量＞：＜语句＞
void Parser::casestate() {
	tab++;
	tabf();
	cout << "case statement" << endl;
	if (Lexer::sym == CASESYM) {
		Lexer::getsym();
		if (Lexer::sym != NUMSYM && Lexer::sym != QUOTE && Lexer::sym != PLUSSYM && Lexer::sym != MINUSSYM) {		//＜常量＞ ::= ＜整数＞|＜字符＞
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		if (Lexer::sym != COLON) {
			error(MISSING_COLON);
		}
		Lexer::getsym();
		statement();
	}
	else
	{
		error(0);	//没有case
	}
	//cout << "This is a case statement!" << endl;
	tab--;
}

//＜缺省＞::=  default : ＜语句＞|＜空＞
void Parser::defaultstate() {
	tab++;
	tabf();
	cout << "default statement" << endl;
	if (Lexer::sym == DEFAULTSYM) {
		Lexer::getsym();
		if (Lexer::sym != COLON) {
			error(MISSING_COLON);
		}
		Lexer::getsym();
		statement();
	}

	//cout << "This is a default statement!" << endl;
	tab--;
}

//＜读语句＞::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
void Parser::scanfstate() {
	tab++;
	tabf();
	cout << "scanf statement" << endl;
	if (Lexer::sym == SCANFSYM) {
		Lexer::getsym();
		if (Lexer::sym == LPARENT) {
			Lexer::getsym();
			if (Lexer::sym != IDSYM) {
				error(MISSING_IDENT);
			}
			Lexer::getsym();
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				if (Lexer::sym != IDSYM) {
					error(MISSING_IDENT);
				}
				Lexer::getsym();
			}
			if (Lexer::sym != RPARENT) {
				error(MISSING_RPARENT);
			}
			Lexer::getsym();
		}
		else
		{
			error(MISSING_LPARENT);
		}
	}
	else
	{
		error(0);
	}
	//cout << "This is a scanf statement!" << endl;
	tab--;
}

//＜写语句＞::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’| printf ‘(’＜字符串＞ ‘)’
//				| printf ‘(’＜表达式＞‘)’
void Parser::printfstate() {
	tab++;
	tabf();
	cout << "printf statement" << endl;
	if (Lexer::sym != PRINTFSYM) {
		error(0);
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	if (Lexer::sym == STRING) {
		Lexer::getsym();
		if (Lexer::sym == COMMA) {
			Lexer::getsym();
			expression();
		}
	}
	else if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM || Lexer::sym == IDSYM || Lexer::sym == LPARENT) {
		expression();
	}
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	//cout << "This is a printf statement!" << endl;
	tab--;
}

//＜返回语句＞::=  return[‘(’＜表达式＞‘)’]     
void Parser::returnstate() {
	tab++;
	tabf();
	cout << "return statement" << endl;
	if (Lexer::sym != RETURNSYM) {
		error(0);
	}
	Lexer::getsym();
	if (Lexer::sym == LPARENT) {
		Lexer::getsym();
		expression();
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
	}
	//cout << "This is a return statement!" << endl;
	tab--;
}

//＜有返回值函数调用语句＞ :: = ＜标识符＞‘(’＜值参数表＞‘)’
//＜无返回值函数调用语句＞ :: = ＜标识符＞‘(’＜值参数表＞‘)’
//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Parser::calfunc() {
	tab++;
	tabf();
	cout << "call function" << endl;
	if (Lexer::sym ==IDSYM) {
		Lexer::getsym();
		if (Lexer::sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		Lexer::getsym();
		if (Lexer::sym == IDSYM || Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM ||Lexer::sym == LPARENT || Lexer::sym == QUOTE) {
			expression();
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				expression();
			}
		}
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
	}
	else
	{
		error(0);
	}
	tab--;
}