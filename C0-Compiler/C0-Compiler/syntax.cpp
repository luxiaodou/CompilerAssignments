#include "stdafx.h"
#include "global.h"

void enter();		//将符号存入符号表
void program();	//处理程序的递归子程序
void conststate();		//处理常量说明的递归子程序
void constdef();	//处理常量定义的递归子程序
void varstate();		//处理变量声明的递归子程序
void vardef();		//处理变量定义的递归子程序
void headdef();	//处理头声明的递归子程序
void paralist();		//处理函数声明时参数表的递归子程序
void funcdef();	//处理函数声明的递归子程序
void statement();		//处理语句的递归子程序
void expression();	//处理表达式的递归子程序
void term();	//处理项的递归子程序
void factor();	//处理因子的递归子程序
void assignstate();	//处理赋值语句的递归子程序
void ifstate();	//处理if语句的递归子程序
void whilestate();	//处理while循环语句的递归子程序
void switchstate();	//处理switch的递归子程序
void printfstate();		//处理printf的递归子程序
void scanfstate();		//处理scanf的递归子程序
void returnstate();	//处理return的递归子程序

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
			varstate();		//所有变量声明理应在这里处理完毕
		}
		else if (sym == LPARENT) {
			funcdef();		//注意这里只处理了一个有返回值的函数
		}
	}
	else {
		error(0);
	}

	//处理函数声明
	while (sym == INTSYM || sym == CHARSYM || sym == VOIDSYM) {
		backup();
		getsym();
		if (sym == MAINSYM) {
			retrieve();
			break;
		}
		else if (sym != IDSYM) {
			error(MISSING_IDENT);
		}
		getsym();
		if (sym == LPARENT) {
			funcdef();
		}
		else {

		}

	}

	//处理main函数




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
			getsym();
		}
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
		if (sym == PLUSSYM) {	//＜加法运算符＞ ::= +｜-
			
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
	retrieve();
	vardef();
	if (sym != SEMICOLON) {
		error(MISSING_SEMICOLON);
	}
	getsym();
	backup();
	while (sym == INTSYM || sym == CHARSYM) {
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
			return;	
		}
		if (sym != SEMICOLON) {
			error(MISSING_SEMICOLON);
		}
	}
	cout << "This is a var state!" << endl;
}


//＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}
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
							getsym();
						}
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
}

