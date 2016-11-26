#include "stdafx.h"
#include "global.h"
#include "Parser.h"
#include "Lexer.h"
#include "defination.h"
#include "Quadruple.h"

int tab = 0;
bool debug = false;	//是否开启调试模式,检查Parser中的错误
//int ttype = 0, tident = 0;
string name;
int addr, kind, type, value, number;
int exp_index;
//没有实际意义的函数,只是为了console输出更美观,缩进功能
void tabf() {
	int i = 0;
	while (i < tab) {
		cout << "|---";
		i++;
	}
}

//将类型转换为字符串,方便生成四元式
string type2str(int type) {
	if (type == INTSYM) {
		return "INT";
	}
	else if (type == CHARSYM) {
		return "CHAR";
	}
	else if (type == VOIDSYM) {
		return "VOID";
	}
	else {
		return "";
	}
}

//将整数转换为字符串,方便生成四元式
string num2str(int n) {
	return to_string(long long(n));
}

string op2str(int op) {
	if (op== PLUSSYM) {
		return "+";
	}
	else if (op == MINUSSYM) {
		return "-";
	}
	else if (op == MULTISYM) {
		return "*";
	}
	else if (op == DEVIDESYM) {
		return "/";
	}
	else {
		return "";
	}
}

//TODO:是否error单做一类
//用来输出错误信息的函数,在考虑是否需要单做一类	
void Parser::error(int errorid)
{
	errorNum++;
	switch (errorid) {
	case CASE_TYPE_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "case的声明出错,标识符类型不正确!" << endl;
		break;
	case UNDEF_ID:
		cout << "Parser Error: line" << Lexer::line << ":" << "使用了未定义的标识符" << Lexer::token << endl;
		break;
	case MULTI_DEF:
		cout << "Parser Error: line" << Lexer::line << ":" << "重定义标识符" << Lexer::token << endl;
		break;
	case ID_TOO_LONG:
		cout << "Parser Error: line" << Lexer::line << ":" << "标识符过长!" << endl;
		break;
	case PROGRAM_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "程序结构错误!" << endl;
		break;
	case STR_CONTENT_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "字符串出现非法内容!" << endl;
		break;
	case STR_TOO_LONG:
		cout << "Parser Error: line" << Lexer::line << ":" << "字符串超长,最长请不要超过" << STRING_MAXLENGTH << "个字符!" << endl;
		break;
	case ZERO_HEAD_NUM:
		cout << "Parser Error: line" << Lexer::line << ":" << "非零数字不能有前导零!" << endl;
		break;
	case NUM_HEAD_IDENT:
		cout << "Parser Error: line" << Lexer::line << ":" << "标识符不能以数字开头!" << endl;
		break;
	case NUM_TOO_LARGE:
		cout << "Parser Error: line" << Lexer::line << ":" << "数字过大! 请不要超过32位立即数的最大范围!" << endl;
		break;
	case CHAR_MISS_QUOTE:
		cout << "Parser Error: line" << Lexer::line << ":" << "字符类型丢失单引号!" << endl;
		break;
	case CHAR_CONTENT_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "字符内容不合法!" << endl;
		break;
	case CHAR_OVERFLOW:
		cout << "Parser Error: line" << Lexer::line << ":" << "字符ascii码溢出!" << endl;
		break;
	case NEQUAL_MISS:
		cout << "Parser Error: line" << Lexer::line << ":" << "错误使用不等号! 应使用'!='" << endl;
		break;
	case MISSING_SEMI:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失分号! 请检查分号是否匹配!" << endl;
		break;
	case MISSING_RPARENT:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失右括号!" << endl;
		break;
	case MISSING_RBRACK:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失右中括号!" << endl;
		break;
	case MISSING_RBRACE:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失右大括号!" << endl;
		break;
	case MISSING_IDENT:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失标识符!" << endl;
		break;
	case MISSING_LPARENT:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失左括号!" << endl;
		break;
	case MISSING_LBRACK:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失左中括号!" << endl;
		break;
	case MISSING_LBRACE:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失左大括号!" << endl;
		break;
	case MISSING_PLUS:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失加号或减号!" << endl;
		break;
	case MISSING_MULTI:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失乘号或除号!" << endl;
		break;
	case ASSIGN_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "赋值时发生错误!" << endl;
		break;
	case RETURN_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "返回值错误!" << endl;
		break;
	case MISSING_MAIN:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失main函数!" << endl;
		break;
	case MISSING_RETURN:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失return语句!" << endl;
		break;
	case EXPRESSION_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "表达式错误!" << endl;
		break;
	case OUT_OF_TABLE:
		cout << "Parser Error: line" << Lexer::line << ":" << "符号表满了!" << endl;
		break;
	case OUT_OF_ARRAY:
		cout << "Parser Error: line" << Lexer::line << ":" << "数组溢出!" << endl;
		break;
	case WRONG_TYPE:
		cout << "Parser Error: line" << Lexer::line << ":" << "声明类型错误!" << endl;
		break;
	case MISSING_ASSIGN:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失等号!" << endl;
		break;
	case MISSING_COLON:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失冒号!" << endl;
		break;
	case MISSING_VOID:
		cout << "Parser Error: line" << Lexer::line << ":" << "丢失void关键字!" << endl;
		break;
	case MISSING_DOUQUOTE:
		cout << "Parser Error: line" << Lexer::line << ":" << "引号缺失!" << endl;
		break;
	case CONST_TYPE_ERROR:
		cout << "Parser Error: line" << Lexer::line << ":" << "常量类型错误,常量只允许是int或者char类型" << endl;
	default:
		cout << "Parser Error: line" << Lexer::line << ":" << "发生了未知错误!" << endl;	//理论上永远不可能执行到这里
		break;
	}
}

//错误处理的跳读函数,输入需要跳读的符号集,跳过所有符号集中的字符
void Parser::skip(set<int> symset)
{
	while (symset.count(Lexer::sym)) {
		Lexer::getsym();
	}
}

//＜程序＞::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void Parser::program() {
	if (debug) {
		tabf();
		cout << "program" << endl;
	}

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
			else {
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
			else {
				error(MISSING_LPARENT);
			}
		}
	}

	//处理main函数
	if (Lexer::sym == VOIDSYM) {
		mainfdef();
	}
	else {
		error(0);
	}
}

//＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}	//qfinish
void Parser::conststate() {
	if (debug) {
		tab++;
		tabf();
		cout << "const statement!" << endl;
	}

	do {
		if (Lexer::sym != CONSTSYM) {
			error(0);	//正常程序不可能到达此分支
		}
		Lexer::getsym();
		constdef();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMI);
		}
		Lexer::getsym();
	} while (Lexer::sym == CONSTSYM);
	tab--;
}

//＜常量定义＞ ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//					| char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }
void Parser::constdef() {
	//todo:相同值的常量可以合并成一个,待优化
	int sign = 1;	//sign == 1 为正数, -1为负数
	if (debug) {
		tab++;
		tabf();
		cout << "const defination" << endl;
	}

	if (Lexer::sym == INTSYM) {
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		else {
			name = Lexer::token;
			Lexer::getsym();
		}
		if (Lexer::sym != ASSIGN) {
			error(MISSING_ASSIGN);
		}
		Lexer::getsym();
		if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {	//＜加法运算符＞ ::= +｜-
			if (Lexer::sym == MINUSSYM) {
				sign = -1;
			}
			Lexer::getsym();
		}
		if (Lexer::sym == NUMTY) {		//＜整数＞::= ［＋｜－］＜无符号整数＞｜０
			value = Lexer::value * sign;
			if (symbolTable.con_insert(name, NUMTY, value)) {	//返回值等于一说明已经声明了相同标识符的常/变量
				error(MULTI_DEF);
			}
			else {
				quadTable.push_back(Quadruple("CON", name, "INT", num2str(value)));
			}
		}
		else {
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		while (Lexer::sym == COMMA) {
			sign = 1;
			Lexer::getsym();
			if (Lexer::sym != IDSYM) {
				error(MISSING_IDENT);
			}
			else {
				name = Lexer::token;
				Lexer::getsym();
			}
			if (Lexer::sym != ASSIGN) {
				error(MISSING_ASSIGN);
			}
			Lexer::getsym();
			if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
				if (Lexer::sym == MINUSSYM) {
					sign = -1;
				}
				Lexer::getsym();
			}
			if (Lexer::sym != NUMTY) {
				error(WRONG_TYPE);
			}
			else {
				value = Lexer::value * sign;
				if (symbolTable.con_insert(name, NUMTY, value)) {
					error(MULTI_DEF);
				}
				else {
					quadTable.push_back(Quadruple("CON", name, "INT", num2str(value)));
				}
			}
			Lexer::getsym();
		}
	}
	else if (Lexer::sym == CHARSYM) {
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			name = Lexer::token;
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
		else {
			value = Lexer::value;
			if (symbolTable.con_insert(name, CHARTY, value)) {
				error(MULTI_DEF);
			}
			else {
				quadTable.push_back(Quadruple("CON", name, "CHAR", num2str(value)));
			}
		}
		Lexer::getsym();
		while (Lexer::sym == COMMA) {
			Lexer::getsym();
			if (Lexer::sym != IDSYM) {
				name = Lexer::token;
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
			else {
				value = Lexer::value;
				if (symbolTable.con_insert(name, CHARTY, value)) {
					error(MULTI_DEF);
				}
				else {
					quadTable.push_back(Quadruple("CON", name, "CHAR", to_string(long long(value))));
				}
			}
			Lexer::getsym();
		}
	}
	else {
		error(CONST_TYPE_ERROR);	//常量只允许是int或者char类型
	}
	tab--;
}

//＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}	//qfinish
void Parser::varstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "var statement" << endl;
	}

	vardef();
	if (Lexer::sym != SEMICOLON) {
		error(MISSING_SEMI);
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
			error(MISSING_SEMI);
		}
		Lexer::getsym();
	}
	tab--;
}

//＜变量定义＞::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ )}	//qfinish
void Parser::vardef() {
	if (debug) {
		tab++;
		tabf();
		cout << "var defination" << endl;
	}

	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		type = Lexer::sym;
		Lexer::getsym();
		if (Lexer::sym == IDSYM) {
			name = Lexer::token;
			Lexer::getsym();
			if (Lexer::sym == LBRACK) {	//数组声明
				Lexer::getsym();
				if (Lexer::sym != NUMTY) {
					error(WRONG_TYPE);
				}
				else {
					number = Lexer::value;	//记录数组长度
				}
				Lexer::getsym();
				if (Lexer::sym != RBRACK) {
					error(MISSING_RBRACK);
				}
				if (symbolTable.arr_insert(name, type, number)) {
					error(MULTI_DEF);
				}
				else {
					quadTable.push_back(Quadruple("ARR", name, type2str(type), num2str(number)));
				}
				Lexer::getsym();
			}
			else {	//单一变量声明
				if (symbolTable.var_insert(name, type)) {
					error(MULTI_DEF);
				}
				else {
					quadTable.push_back(Quadruple("VAR", name, type2str(type), ""));
				}
			}
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				if (Lexer::sym == IDSYM) {
					name = Lexer::token;
					Lexer::getsym();
					if (Lexer::sym == LBRACK) {	//数组
						Lexer::getsym();
						if (Lexer::sym != NUMTY) {
							error(WRONG_TYPE);
						}
						else {
							number = Lexer::value;	//记录数组大小
						}
						Lexer::getsym();
						if (Lexer::sym != RBRACK) {
							error(MISSING_RBRACK);
						}
						if (symbolTable.arr_insert(name, type, number)) {
							error(MULTI_DEF);
						}
						else {
							quadTable.push_back(Quadruple("ARR", name, type2str(type), num2str(number)));
						}
						Lexer::getsym();
					}
					else {	//单一变量声明
						if (symbolTable.var_insert(name, type)) {
							error(MULTI_DEF);
						}
						else {
							quadTable.push_back(Quadruple("VAR", name, type2str(type), ""));
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
	tab--;
}

//＜参数表＞ ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞	//qfinish
void Parser::paralist() {
	if (debug) {

		tab++;
		tabf();
		cout << "paralist" << endl;
	}
	number = 0;	//开始记录参数的个数
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		type = Lexer::sym;
		Lexer::getsym();
		if (Lexer::sym == IDSYM) {
			name = Lexer::token;
			if (symbolTable.para_insert(name, type)) {
				error(MULTI_DEF);
			}
			else {
				number++;
				quadTable.push_back(Quadruple("PARA", name, type2str(type), ""));
			}
			Lexer::getsym();
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				if (Lexer::sym != INTSYM && Lexer::sym != CHARSYM) {
					error(WRONG_TYPE);
				}
				else {
					type = Lexer::sym;
				}
				Lexer::getsym();
				if (Lexer::sym == IDSYM) {
					name = Lexer::token;
					if (symbolTable.para_insert(name, type)) {
						error(MULTI_DEF);
					}
					else {
						number++;
						quadTable.push_back(Quadruple("PARA", name, type2str(type), ""));
					}
				}
				else {
					error(MISSING_IDENT);
				}
				Lexer::getsym();
			}
		}
	}
	tab--;
}

//＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］＜语句列＞	//qfinish
void Parser::compound() {
	if (debug) {
		tab++;
		tabf();
		cout << "compound" << endl;

	}
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
}

//＜有返回值函数定义＞:: = ＜声明头部＞‘(’＜参数表＞‘)’ ‘ { ’＜复合语句＞‘ }’
void Parser::funcdef() {
	if (debug) {
		tab++;
		tabf();
		cout << "funcdef" << endl;
	}
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		int ftype = Lexer::sym;
		string fname;
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		else {
			fname = Lexer::token;
			if (symbolTable.func_insert(fname, ftype, 0)) {	//注意这时候还没有设置para的个数
				error(MULTI_DEF);
			}
			else {
				//todo:函数声明的四元式
				Lexer::getsym();
			}
		}
		if (Lexer::sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		Lexer::getsym();
		paralist();
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		symbolTable.setnum(fname, number);	//此时给才正式给函数添加符号
		Lexer::getsym();
		if (Lexer::sym != LBRACE) {
			error(MISSING_LBRACE);
		}
		symbolTable.addlevel();		//大括号里面的参数需要加一层
		Lexer::getsym();
		compound();
		if (Lexer::sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		symbolTable.decllevel();
		symbolTable.poplevel();//将函数体中的符号退栈
		Lexer::getsym();
	}
	else {
		error(WRONG_TYPE);
	}
	tab--;
}

//＜无返回值函数定义＞:: = void＜标识符＞‘(’＜参数表＞‘)’‘ { ’＜复合语句＞‘ }’
void Parser::voidfdef() {
	if (debug) {
		tab++;
		tabf();
		cout << "voidfdef" << endl;
	}
	if (Lexer::sym != VOIDSYM) {
		error(MISSING_VOID);
	}
	else {
		type = Lexer::sym;
		Lexer::getsym();
	}
	if (Lexer::sym == IDSYM) {
		string fname = Lexer::token;
		if (symbolTable.func_insert(fname, type, 0)) {
			error(MULTI_DEF);
		}
		else {
			//tudo:生成四元式操作
			Lexer::getsym();
		}
		if (Lexer::sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		Lexer::getsym();
		paralist();
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		symbolTable.setnum(fname, number);
		Lexer::getsym();
		if (Lexer::sym != LBRACE) {
			error(MISSING_LBRACE);
		}
		symbolTable.addlevel();
		Lexer::getsym();
		compound();
		if (Lexer::sym != RBRACE) {
			error(MISSING_RBRACE);
		}
		symbolTable.decllevel();
		symbolTable.poplevel();
		Lexer::getsym();
	}
	else {
		error(MISSING_IDENT);
	}
	tab--;
}

//＜主函数＞ ::= void main‘(’‘)’‘{’＜复合语句＞‘}’
void Parser::mainfdef() {
	if (debug) {
		tab++;
		tabf();
		cout << "main" << endl;
	}
	if (Lexer::sym != VOIDSYM) {
		error(MISSING_VOID);
	}
	Lexer::getsym();
	if (Lexer::sym != MAINSYM) {
		error(0);
	}
	symbolTable.func_insert("MAIN", VOIDSYM, 0);	//主函数只有这一种类型
	//todo:完成主函数相关的四元式说明
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
	symbolTable.addlevel();
	Lexer::getsym();
	compound();
	if (Lexer::sym != RBRACE) {
		error(MISSING_RBRACE);
	}
	symbolTable.decllevel();
	symbolTable.poplevel();
	tab--;
}

//＜语句＞:: = ＜条件语句＞｜＜循环语句＞ | ‘{ ’＜语句列＞‘ }’｜＜有返回值函数调用语句＞;
//				| ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜情况语句＞｜＜返回语句＞;
void Parser::statement() {
	if (debug) {
		tab++;
		tabf();
		cout << "statement" << endl;
	}
	switch (Lexer::sym) {
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
		else {
			Lexer::retrieve();
			assignstate();
		}
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMI);
		}
		Lexer::getsym();
		break;
	case PRINTFSYM:		//printstate
		printfstate();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMI);
		}
		Lexer::getsym();
		break;
	case SCANFSYM:		//scanstate
		scanfstate();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMI);
		}
		Lexer::getsym();
		break;
	case SEMICOLON:		//blankstate
		Lexer::getsym();
		break;
	case SWITCHSYM:		//switchstate
		switchstate();
		Lexer::getsym();
		break;
	case RETURNSYM:		//returnstate
		returnstate();
		if (Lexer::sym != SEMICOLON) {
			error(MISSING_SEMI);
		}
		Lexer::getsym();
		break;
	default:
		break;
	}
	tab--;
}

//＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
void Parser::expression(string &exp_name, int &exp_type) {
	if (debug) {
		tab++;
		tabf();
		cout << "expression" << endl;
	}
	//声明计算用的相关变量
	vector<int> opstack;
	vector<string> varstack;
	int exp_value = 0;
	exp_type = INTSYM;
	int esign = 1;
	string term_name;
	int term_value,term_type;
	int op = 0;

	//将表达式的值存入符号表中,初值为0
	string exp_name = "$temp" + num2str(exp_index++);
	symbolTable.temp_insert(exp_name, exp_type);

	//正式开始分析语义
	if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
		op = Lexer::sym;
		Lexer::getsym();
	}
	term(term_name,term_type);
	if (op == MINUSSYM) {

	}
	
	while (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
		esign = 1;
		if (Lexer::sym == MINUSSYM) {
			esign = -1;
		}
		Lexer::getsym();
		term(term_name,term_type);
		exp_value += esign*term_value;
	}
	tab--;
	
}

//＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Parser::term(string &term_name, int &term_type) {
	if (debug) {
		tab++;
		tabf();
		cout << "term" << endl;
	}
	string factor_name;
	int factor_type;
	int factor_type, factor_namefactor_value;
	factor(factor_name,factor_type);
	while (Lexer::sym == MULTISYM || Lexer::sym == DEVIDESYM) {
		int sign = Lexer::sym;
		Lexer::getsym();
		factor(factor_name, factor_type);
		
	}
	tab--;
}

//＜因子＞ ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’|‘(’＜表达式＞‘)’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞ 
void Parser::factor(string &factor_name, int &factor_type) {
	if (debug) {
		tab++;
		tabf();
		cout << "factor" << endl;
	}
	switch (Lexer::sym) {
	case IDSYM:	//变量或数组或有返回值的函数调用
		name = Lexer::sym;
		if (!symbolTable.in_table(name)) {
			error(UNDEF_ID);
		}
		Lexer::backup();
		Lexer::getsym();
		if (Lexer::sym == LBRACK) {	//是数组
			Lexer::getsym();
			int offset;
			expression(factor_name,factor_type);
			if (Lexer::sym != RBRACK) {
				error(MISSING_RBRACK);
			}
			Lexer::getsym();
		}
		else if (Lexer::sym == LPARENT) {	//是函数
			Lexer::retrieve();
			calfunc();
		}
		else {	//是单一变量
			factor_name = name;
			factor_type = INTSYM;
		}
		break;
	case LPARENT:
		Lexer::getsym();
		expression(factor_name,factor_type);
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
		break;
	case PLUSSYM:
	case MINUSSYM:
		Lexer::getsym();
		if (Lexer::sym == NUMTY) {
			//not implement yet!
		}
		else {
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		break;
	case NUMTY:
		//todo:implement!
		Lexer::getsym();
		break;
	case CHARTY:
		//todo:implement!
		Lexer::getsym();
		break;
	default:
		error(0);	//正常情况下不可能执行至此分支
		break;
	}
	tab--;
}

//＜条件语句＞:: = if ‘(’＜条件＞‘)’＜语句＞
void Parser::ifstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "if statement" << endl;
	}
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
	tab--;
}

//＜条件＞::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
void Parser::condition()
{
	if (debug) {
		tab++;
		tabf();
		cout << "condition" << endl;
	}
	string cname;
	int ctype;
	expression(cname,ctype);
	if (Lexer::sym == LESS || Lexer::sym == LESSEQU || Lexer::sym == GREAT || Lexer::sym == GREATEQU || Lexer::sym == NEQUAL || Lexer::sym == EQUAL) {
		Lexer::getsym();
		expression(cname,ctype);	//todo:not right here
	}
	tab--;
}

//＜赋值语句＞:: = ＜标识符＞＝＜表达式＞ | ＜标识符＞‘[’＜表达式＞‘]’ = ＜表达式＞
void Parser::assignstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "assign statement" << endl;
	}
	string exname;
	int extype;
	if (Lexer::sym != IDSYM) {
		error(MISSING_IDENT);
	}
	else {
		name = Lexer::token;
		int kind = symbolTable.getkind(name);
		if (kind != VAR && kind != ARR) {
			error(ASSIGN_ERROR);	//给非变量类型赋值
		}
	}
	Lexer::getsym();
	if (Lexer::sym == LBRACK) {		//数组中的元素赋值
		Lexer::getsym();
		string offsetname;
		expression(offsetname,type);
		if (Lexer::sym != RBRACK) {
			error(MISSING_RBRACK);
		}
		Lexer::getsym();
	}
	if (Lexer::sym != ASSIGN) {
		error(0);
	}
	Lexer::getsym();
	expression(exname,extype);
	
	tab--;
}

//＜循环语句＞:: = while ‘(’＜条件＞‘)’＜语句＞
void Parser::whilestate() {
	if (debug) {
		tab++;
		tabf();
		cout << "while statement" << endl;
	}
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
	tab--;
}

//＜情况语句＞:: = switch ‘(’＜表达式＞‘)’ ‘ { ’＜情况表＞＜缺省＞ ‘ }’
void Parser::switchstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "switch statement" << endl;
	}
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
	tab--;
}

//＜情况子语句＞:: = case＜常量＞：＜语句＞
void Parser::casestate() {
	if (debug) {
		tab++;
		tabf();
		cout << "case statement" << endl;
	}
	if (Lexer::sym == CASESYM) {
		Lexer::getsym();
		//if (Lexer::sym != NUMSYM && Lexer::sym != QUOTE && Lexer::sym != PLUSSYM && Lexer::sym != MINUSSYM) {		//＜常量＞ ::= ＜整数＞|＜字符＞
		//	error(WRONG_TYPE);
		//}
		if (Lexer::sym == NUMTY || Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
			if (Lexer::sym == PLUSSYM) {
				Lexer::getsym();
			}
			else if (Lexer::sym == MINUSSYM) {
				Lexer::getsym();
			}
			Lexer::getsym();
		}
		else if (Lexer::sym == CHARTY) {
			Lexer::getsym();
		}
		else {
			error(CASE_TYPE_ERROR);
		}
		if (Lexer::sym != COLON) {
			error(MISSING_COLON);
		}
		Lexer::getsym();
		statement();
	}
	else {
		error(0);	//没有case
	}
	tab--;
}

//＜缺省＞::=  default : ＜语句＞|＜空＞
void Parser::defaultstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "default statement" << endl;
	}
	if (Lexer::sym == DEFAULTSYM) {
		Lexer::getsym();
		if (Lexer::sym != COLON) {
			error(MISSING_COLON);
		}
		Lexer::getsym();
		statement();
	}

	tab--;
}

//＜读语句＞::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
void Parser::scanfstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "scanf statement" << endl;
	}
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
		else {
			error(MISSING_LPARENT);
		}
	}
	else {
		error(0);
	}
	tab--;
}

//＜写语句＞::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’| printf ‘(’＜字符串＞ ‘)’
//				| printf ‘(’＜表达式＞‘)’
void Parser::printfstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "printf statement" << endl;
	}
	string str, exp, type;
	Quadruple q;
	if (Lexer::sym != PRINTFSYM) {
		error(0);	//正常情况不可能走到此分支
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);	//丢失大括号就不getsym了,尽量不影响后面的语句分析
	}
	else {
		Lexer::getsym();
	}
	if (Lexer::sym == STRING) {
		bool exist = false;
		map<string, string>::iterator it;
		string key;
		for (it = strTable.begin(); it != strTable.end(); it++) {
			if (Lexer::token == it->second) {
				key = it->first;
				exist = true;
				break;
			}
		}
		if (!exist) {	//表中不存在,添加操作
			key = "$string" + num2str(stbIndex++);
			strTable[key] = Lexer::token;
			str = key;
		}
		else {	//表中存在,直接使用其key
			str = key;
		}
		type = "0";
		Lexer::getsym();
		//todo:获得表达式的返回值
		if (Lexer::sym == COMMA) {
			type = "2";
			Lexer::getsym();
			expression();
		}
	}
	else {
		type = "1";
		expression();
	}
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);	//容错处理
	}
	else {
		Lexer::getsym();
	}
	q.fill("PRT", str, exp, type);
	quadTable.push_back(q);
	tab--;
}

//＜返回语句＞::=  return[‘(’＜表达式＞‘)’]     
void Parser::returnstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "return statement" << endl;
	}
	if (Lexer::sym != RETURNSYM) {
		error(0);	//正常情况无法达到此分支
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
	tab--;
}

//＜有返回值函数调用语句＞ :: = ＜标识符＞‘(’＜值参数表＞‘)’
//＜无返回值函数调用语句＞ :: = ＜标识符＞‘(’＜值参数表＞‘)’
//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Parser::calfunc() {
	if (debug) {
		tab++;
		tabf();
		cout << "call function" << endl;
	}
	if (Lexer::sym == IDSYM) {
		Lexer::getsym();
		if (Lexer::sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		Lexer::getsym();
		if (Lexer::sym == IDSYM || Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM || Lexer::sym == LPARENT || Lexer::sym == NUMTY || Lexer::sym == CHARTY) {
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
	else {
		error(0);
	}
	tab--;
}