#include "stdafx.h"
#include "global.h"



int ttype = 0, tident = 0;

void enter() {

}

//＜程序＞::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program() {
	getsym();

	//处理常量声明
	if (sym == CONSTSYM) {
		conststate();
	}

	//处理变量声明
	if (sym == INTSYM || sym == CHARSYM) {	//需要预读判断是变量定义还是有返回值函数定义
		backup();
		getsym();
		if (sym != IDSYM) {
			error(MISSING_IDENT);
		}
		getsym();

		if (sym == COMMA || sym == LBRACK) {		//变量
			retrieve();
			varstate();		//所有变量声明理应在这里处理完毕
		}
		else if (sym == LPARENT) {
			retrieve();
			funcdef();		//注意这里只处理了一个有返回值的函数
		}
	}
	else {
		error(0);
	}

	//处理函数声明
	while (sym == INTSYM || sym == CHARSYM || sym == VOIDSYM) {
		if (sym == VOIDSYM) {
			backup();
			getsym();
			if (sym == MAINSYM) {
				break;
			}
			if (sym != IDSYM) {
				error(MISSING_IDENT);
			}
			getsym();
			if (sym == LPARENT) {
				retrieve();
				voidfdef();
			}
			else
			{
				error(MISSING_LPARENT);
			}
		}
		else {
			backup();
			getsym();
			if (sym != IDSYM) {
				error(MISSING_IDENT);
			}
			getsym();
			if (sym == LPARENT) {
				retrieve();
				funcdef();
			}
			else
			{
				error(MISSING_LPARENT);
			}
		}
	}

	//处理main函数
	if (sym == VOIDSYM) {
		mainfdef();
	}
	else
	{
		error(0);
	}

	cout << "This is a program!" << endl;
}

//＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void conststate() {
	do
	{
		if (sym != CONSTSYM) {
			error(0);
		}
		getsym();
		constdef();
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
	} while (sym == CONSTSYM);
	cout << "this is a const statement!" << endl;
}

//＜常量定义＞ ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//					| char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }
void constdef() {
	if (sym == INTSYM) {
		getsym();
		if (sym != IDSYM) {
			error(MISSING_IDENT);
		}
		getsym();
		if (sym != ASSIGN) {
			error(MISSING_ASSIGN);
		}
		getsym();
		if (sym == PLUSSYM || sym == MINUSSYM) {	//＜加法运算符＞ ::= +｜-
			getsym();
		}
		if (sym == NUMSYM) {		//＜整数＞::= ［＋｜－］＜无符号整数＞｜０

		}
		else
		{
			error(WRONG_TYPE);
		}
		getsym();
		while (sym == COMMA) {
			getsym();
			if (sym != IDSYM) {
				error(MISSING_IDENT);
			}
			getsym();
			if (sym != ASSIGN) {
				error(MISSING_ASSIGN);
			}
			getsym();
			if (sym == PLUSSYM || sym == MINUSSYM) {
				getsym();
			}
			if (sym != NUMSYM) {
				error(WRONG_TYPE);
			}
			getsym();
		}
	}
	else if (sym == CHARSYM) {
		getsym();
		if (sym != IDSYM) {
			error(MISSING_IDENT);
		}
		getsym();
		if (sym != ASSIGN) {
			error(MISSING_ASSIGN);
		}
		getsym();
		if (sym != CHARTY) {
			error(WRONG_TYPE);
		}
		getsym();
		while (sym == COMMA) {
			getsym();
			if (sym != IDSYM) {
				error(MISSING_IDENT);
			}
			getsym();
			if (sym != ASSIGN) {
				error(MISSING_ASSIGN);
			}
			getsym();
			if (sym != CHARTY) {
				error(WRONG_TYPE);
			}
			getsym();
		}
	}
	cout << "this is a const defination!" << endl;
}

//＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void varstate() {
	vardef();
	if (sym != SEMICOLON) {
		error(MISSING_SEMICOLON);
	}
	getsym();
	while (sym == INTSYM || sym == CHARSYM) {
		backup();
		getsym();
		if (sym != IDSYM) {
			error(MISSING_IDENT);
		}
		getsym();
		if (sym == COMMA || sym == LBRACK) {
			retrieve();
			vardef();
		}
		else if (sym == LPARENT || sym == VOIDSYM) {	//不再是变量声明
			retrieve();
			break;
		}
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
	}
	cout << "This is a var state!" << endl;
}

//＜变量定义＞::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}
void vardef() {
	if (sym == INTSYM || sym == CHARSYM) {
		getsym();
		if (sym == IDSYM) {
			getsym();
			if (sym == LBRACK) {
				getsym();
				if (sym != NUMSYM) {
					error(WRONG_TYPE);
				}
				else
				{
					//处理无符号整数
				}
				getsym();
				if (sym != RBRACK)
				{
					error(MISSING_RBRACK);
				}
				getsym();
			}
			while (sym == COMMA) {
				getsym();
				if (sym == IDSYM) {
					getsym();
					if (sym == LBRACK) {
						getsym();
						if (sym != NUMSYM) {
							error(WRONG_TYPE);
						}
						else {
							//deal with no-sign number
						}
						getsym();
						if (sym != RBRACK) {
							error(MISSING_RBRACK);
						}
						getsym();
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
	cout << "This is a var defination" << endl;
}

//＜参数表＞ ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
void paralist() {
	if (sym == INTSYM || sym == CHARSYM) {
		getsym();
		if (sym == IDSYM) {
			getsym();
			while (sym == COMMA) {
				getsym();
				if (sym != INTSYM && sym != CHARSYM) {
					error(WRONG_TYPE);
				}
				getsym();
				if (sym == IDSYM) {
					//not implemented
				}
				else
				{
					error(MISSING_IDENT);
				}
				getsym();
			}
		}
	}
	cout << "This is a paralist!" << endl;
}

//＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void compound() {
	if (sym == CONSTSYM) {
		conststate();
	}
	if (sym == INTSYM || sym == CHARSYM) {
		varstate();
	}
	while (sym == IFSYM || sym == WHILESYM || sym == LBRACE || sym == INTSYM || sym == CHARSYM || sym == VOIDSYM
		|| sym == IDSYM || sym == PRINTFSYM || sym == SCANFSYM || sym == RETURNSYM || sym == SEMICOLON || sym == SWITCHSYM) {
		statement();
	}
	cout << "This is a compound statement!" << endl;
}

//＜有返回值函数定义＞:: = ＜声明头部＞‘(’＜参数表＞‘)’ ‘ { ’＜复合语句＞‘ }’
void funcdef() {
	if (sym == INTSYM || sym == CHARSYM) {
		getsym();
		if (sym != IDSYM) {
			error(MISSING_IDENT);
		}
		getsym();
		if (sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		getsym();
		paralist();
		if (sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		getsym();
		if (sym != LBRACE) {
			error(MISSING_LBRACE);
		}
		getsym();
		compound();
		if (sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		getsym();
	}
	else
	{
		error(WRONG_TYPE);
	}
	cout << "This is a fuction defination!" << endl;
}

//＜无返回值函数定义＞:: = void＜标识符＞‘(’＜参数表＞‘)’‘ { ’＜复合语句＞‘ }’
void voidfdef() {
	if (sym != VOIDSYM) {
		error(MISSING_VOID);
	}
	getsym();
	if (sym == IDSYM) {
		getsym();
		if (sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		getsym();
		paralist();
		if (sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		getsym();
		if (sym != LBRACE) {
			error(MISSING_LBRACE);
		}
		getsym();
		compound();
		if (sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		getsym();
	}
	else
	{
		error(MISSING_IDENT);
	}
	cout << "This is a void function define!" << endl;
}

//＜主函数＞ ::= void main‘(’‘)’‘{’＜复合语句＞‘}’
void mainfdef() {
	if (sym != VOIDSYM) {
		error(MISSING_VOID);
	}
	getsym();
	if (sym != MAINSYM) {
		error(0);
	}
	getsym();
	if (sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	getsym();
	if (sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	getsym();
	if (sym != LBRACE) {
		error(MISSING_LBRACE);
	}
	compound();
	if (sym != RBRACE) {
		error(MISSING_RBRACE);
	}
	cout << "This is a main function" << endl;
}


//＜语句＞:: = ＜条件语句＞｜＜循环语句＞ | ‘{ ’＜语句列＞‘ }’｜＜有返回值函数调用语句＞;
//				| ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜情况语句＞｜＜返回语句＞;
void statement() {
	switch (sym)
	{
	case IFSYM: //ifstate
		ifstate();
		break;
	case WHILESYM:	//while state
		whilestate();
		break;
	case LBRACE:	//statement list
		getsym();
		while (sym == IFSYM || sym == WHILESYM || sym == LBRACE || sym == INTSYM || sym == CHARSYM || sym == VOIDSYM
			|| sym == IDSYM || sym == PRINTFSYM || sym == SCANFSYM || sym == RETURNSYM || sym == SEMICOLON || sym == SWITCHSYM) {
			statement();
		}
		if (sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		getsym();
		break;
	case INTSYM:	//func with value
	case CHARSYM:
		funcdef();
		break;
	case VOIDSYM:	//void func
		voidfdef();
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
		break;
	case IDSYM:	//assignment
		assignstate();
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
		break;
	case PRINTFSYM:
		printfstate();
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
		break;
	case SCANFSYM:
		scanfstate();
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
		break;
	case SEMICOLON:
		getsym();
		break;
	case SWITCHSYM:
		switchstate();
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
		break;
	case RETURNSYM:
		returnstate();
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
		getsym();
		break;
	default:
		break;
	}
	cout << "This is a statement statement!" << endl;
}

//＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
void expression() {
	if (sym == PLUSSYM || sym == MINUSSYM) {
		getsym();
	}
	term();
	while (sym == PLUSSYM || sym == MINUSSYM) {
		getsym();
		term();
	}
	cout << "This is a expression!" << endl;
}

//＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void term() {
	factor();
	while (sym == MULTISYM || sym == DEVIDESYM) {
		getsym();
		factor();
	}
	cout << "This is a term!" << endl;
}

//＜因子＞ ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’|‘(’＜表达式＞‘)’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞ 
void factor() {
	switch (sym)
	{
	case IDSYM:
		getsym();
		if (sym == LBRACK) {
			getsym();
			expression();
			if (sym != RBRACK) {
				error(MISSING_RBRACK);
			}
			getsym();
		}
		else if (sym == LPARENT) {	//＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
			getsym();	//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
			if (sym == IDSYM || sym == PLUSSYM || sym == MINUSSYM) {
				expression();
				getsym();
				while (sym == COMMA) {
					expression();
					getsym();
				}
			}			
		}
		break;
	case LPARENT:
		getsym();
		expression();
		if (sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		break;
	case PLUSSYM:
	case MINUSSYM:
		getsym();
		if (sym == NUMSYM) {
			//not implement yet!
		}
		else
		{
			error(WRONG_TYPE);
		}
		getsym();
		break;
		break;
	case NUMSYM:
		//not implement!
		getsym();
		break;
	case QUOTE:
		getsym();
		if (sym != CHARTY) {
			error(WRONG_TYPE);
		}
		getsym();
		if (sym != QUOTE) {
			error(CHAR_MISS_QUOTE);
		}
		getsym();
		break;
	default:
		error(0);
		break;
	}
	cout << "This is a factor!" << endl;
}

//＜条件语句＞:: = if ‘(’＜条件＞‘)’＜语句＞
void ifstate() {
	if (sym != IFSYM) {
		error(0);
	}
	getsym();
	if (sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	getsym();
	condition();
	if (sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	getsym();
	statement();
	cout << "This is an if statement!" << endl;
}

//＜条件＞::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
void condition()
{
	expression();
	if (sym == LESS || sym == LESSEQU || sym == GREAT || sym == GREATEQU || sym == NEQUAL || sym == EQUAL) {
		getsym();
		expression();
	}
	else
	{
		error(0);
	}
	cout << "This is a condition statement!" << endl;
}

//＜赋值语句＞:: = ＜标识符＞＝＜表达式＞ | ＜标识符＞‘[’＜表达式＞‘]’ = ＜表达式＞
void assignstate() {
	if (sym != IDSYM) {
		error(MISSING_IDENT);
	}
	getsym();
	if (sym == LBRACK) {
		getsym();
		expression();
		getsym();
		if (sym != RBRACK) {
			error(MISSING_RBRACK);
		}
		getsym();
	}
	if (sym != ASSIGN) {
		error(0);
	}
	getsym();
	expression();
	cout << "This is an assign statement!" << endl;
}

//＜循环语句＞:: = while ‘(’＜条件＞‘)’＜语句＞
void whilestate() {
	if (sym != WHILESYM) {
		error(0);
	}
	getsym();
	if (sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	getsym();
	condition();
	getsym();
	if (sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	statement();
	cout << "This is a while statement!" << endl;
}

//＜情况语句＞:: = switch ‘(’＜表达式＞‘)’ ‘ { ’＜情况表＞＜缺省＞ ‘ }’
void switchstate() {
	if (sym != SWITCHSYM) {
		error(0);
	}
	getsym();
	if (sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	getsym();
	expression();
	getsym();
	if (sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	getsym();
	if (sym == LBRACE) {		//＜情况表＞::=  ＜情况子语句＞{＜情况子语句＞}
		getsym();
		while (sym == CASESYM) {
			casestate();
			getsym();
		}
		defaultstate();
	}
	getsym();
	if (sym != RBRACE) {
		error(MISSING_RBRACE);
	}
	cout << "This is a switch statement!" << endl;
}

//＜情况子语句＞:: = case＜常量＞：＜语句＞
void casestate() {
	if (sym == CASESYM) {
		getsym();
		if (sym != NUMSYM && sym != QUOTE && sym != PLUSSYM && sym != MINUSSYM) {		//＜常量＞ ::= ＜整数＞|＜字符＞
			error(WRONG_TYPE);
		}
		getsym();
		if (sym != COLON) {
			error(MISSING_COLON);
		}
		statement();
	}
	else
	{
		error(0);	//没有case
	}
	cout << "This is a case statement!" << endl;
}

//＜缺省＞::=  default : ＜语句＞|＜空＞
void defaultstate() {
	if (sym == DEFAULTSYM) {
		getsym();

	}
	cout << "This is a default statement!" << endl;
}

//＜读语句＞::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
void scanfstate() {
	if (sym == SCANFSYM) {
		getsym();
		if (sym == LPARENT) {
			getsym();
			if (sym != IDSYM) {
				error(MISSING_IDENT);
			}
			getsym();
			while (sym == COMMA) {
				getsym();
				if (sym != IDSYM) {
					error(MISSING_IDENT);
				}
				getsym();
			}
			if (sym != RPARENT) {
				error(MISSING_RPARENT);
			}
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
	cout << "This is a scanf statement!" << endl;
}

//＜写语句＞::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’| printf ‘(’＜字符串＞ ‘)’
//				| printf ‘(’＜表达式＞‘)’
void printfstate() {
	if (sym != PRINTFSYM) {
		error(0);
	}
	getsym();
	if (sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	getsym();
	if (sym == STRING) {
		getsym();
		if (sym == COMMA) {
			getsym();
			expression();
		}
	}
	else if (sym == PLUSSYM || sym == MINUSSYM || sym == IDSYM) {
		expression();
	}
	if (sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	getsym();
	cout << "This is a printf statement!" << endl;
}

//＜返回语句＞::=  return[‘(’＜表达式＞‘)’]     
void returnstate() {
	if (sym != RETURNSYM) {
		error(0);
	}
	getsym();
	if (sym == LPARENT) {
		getsym();
		expression();
		if (sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		getsym();
	}
	cout << "This is a return statement!" << endl;
}