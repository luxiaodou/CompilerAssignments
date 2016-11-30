#include "stdafx.h"
#include "global.h"
#include "Parser.h"
#include "Lexer.h"
#include "defination.h"
#include "Quadruple.h"

int tab = 0;
bool debug = false;	//是否开启调试模式,检查Parser中的错误
//int ttype = 0, tident = 0;
string name = "";
string zeroname = "$value0";
int addr, kind, type, number;
long long value;
int temp_index = 0, lab_index = 0;
bool retsign = false;
//debug用函数,只是为了console输出更美观,缩进功能
void tabf() {
	int i = 0;
	while (i < tab) {
		cout << "|---";
		i++;
	}
}

//将整数转换为字符串,方便生成四元式
string num2str(long long n) {
	return to_string(n);
}

//生成一个随机的临时变量名,todo:注意这里可能需要修改
string newtmpname() {
	string a = "$temp";
	string b = num2str(temp_index++);
	return a + b;
}

string newlab() {
	string a = "Lab";
	string b = num2str(lab_index++);
	return a + b;
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


string op2str(int op) {
	if (op == PLUSSYM) {
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
		cout << "Parser Error: line" << Lexer::line << ":" << "常量类型错误,常量只允许是int或者char类型！" << endl;
		break;
	case ERROR_PARA_NUM:
		cout << "Parser Error: line" << Lexer::line << ":" << "函数调用时参数数量异常！" << endl;
		break;
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
	quadTable.push_back(Quadruple("GLBSTR", "", "", ""));
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
			Lexer::retrieve();	//这里不再
		}
	}
	quadTable.push_back(Quadruple("GLBEND", "", "", ""));
	//存入一个固定量0
	symbolTable.con_insert(zeroname, INTSYM,0);
	symbolTable.setzero(zeroname);
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
			if (symbolTable.con_insert(name, INTSYM, value)) {	//返回值等于一说明已经声明了相同标识符的常/变量
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
				if (symbolTable.con_insert(name, INTSYM, value)) {
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
			error(MISSING_IDENT);
		}
		name = Lexer::token;
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
			if (symbolTable.con_insert(name, CHARSYM, value)) {
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
				error(MISSING_IDENT);
			}
			name = Lexer::token;
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
				if (symbolTable.con_insert(name, CHARSYM, value)) {
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
				//quadTable.push_back(Quadruple("PARA", name, type2str(type), ""));
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
						//quadTable.push_back(Quadruple("PARA", name, type2str(type), ""));
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
	retsign = false;
	if (Lexer::sym == INTSYM || Lexer::sym == CHARSYM) {
		int ftype = Lexer::sym;
		string fname = "";
		Lexer::getsym();
		if (Lexer::sym != IDSYM) {
			error(MISSING_IDENT);
		}
		else {
			fname = Lexer::token;
			if (symbolTable.func_insert(fname, ftype, 0)) {	//注意这时候还没有设置para的个数,但是curlevel已经改变
				error(MULTI_DEF);
			}
			else {
				quadTable.push_back(Quadruple("FUNC", fname, type2str(ftype), ""));	//函数开始,生成label+开辟栈空间+保存返回值
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
		symbolTable.setfuncnum(fname, number);	//此时给才正式给函数添加符号
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
	else {
		error(WRONG_TYPE);
	}
	if (!retsign) {	//当没有return的时候输出END来进行函数的返回
		quadTable.push_back(Quadruple("END", "", "", ""));
	}
	symbolTable.setfuncsize();
	tab--;
}

//＜无返回值函数定义＞:: = void＜标识符＞‘(’＜参数表＞‘)’‘ { ’＜复合语句＞‘ }’
void Parser::voidfdef() {
	if (debug) {
		tab++;
		tabf();
		cout << "voidfdef" << endl;
	}
	retsign = false;
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
			quadTable.push_back(Quadruple("FUNC", fname, "VOID", ""));	//函数开始,生成label+开辟栈空间+保存返回值
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
		symbolTable.setfuncnum(fname, number);
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
	else {
		error(MISSING_IDENT);
	}
	tab--;
	if (!retsign) {
		quadTable.push_back(Quadruple("END", "", "", ""));
	}
	symbolTable.setfuncsize();
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
	symbolTable.func_insert("main", VOIDSYM, 0);	//主函数只有这一种类型,更新curlevel
	quadTable.push_back(Quadruple("FUNC", "main", "VOID", ""));	//主函数只有这一种声明格式
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
	symbolTable.setfuncsize();
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
			string temp = "";
			int temp_type = 0;
			calfunc(temp, temp_type);
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
	int exp_value = 0;
	int op = 0;
	exp_type = NUMTY;
	exp_name = "";

	string term_name1 = "", term_name2 = "";
	int  term_type1=0, term_type2=0;

	//正式开始分析语义
	if (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
		op = Lexer::sym;
		Lexer::getsym();
	}
	term(term_name1, term_type1);
	if (op == MINUSSYM) {
		quadTable.push_back(Quadruple("~", term_name1, "", ""));	//进行取反操作
	}

	while (Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
		op = Lexer::sym;
		Lexer::getsym();
		term(term_name2, term_type2);
		if (op == PLUSSYM) {
			quadTable.push_back(Quadruple("+", term_name1, term_name2, term_name1));
		}
		else {
			quadTable.push_back(Quadruple("-", term_name1, term_name2, term_name1));
		}
	}
	if (term_type1 == INTSYM || term_type2 == INTSYM) {
		exp_type = INTSYM;
	}
	else {
		exp_type = CHARSYM;
	}
	tab--;
	exp_name = term_name1;
}

//＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Parser::term(string &term_name, int &term_type) {
	if (debug) {
		tab++;
		tabf();
		cout << "term" << endl;
	}
	string factor_name1 = "", factor_name2 = "";
	int factor_type1=0, factor_type2=0;
	term_name = "";
	term_type = 0;

	factor(factor_name1, factor_type1);
	while (Lexer::sym == MULTISYM || Lexer::sym == DEVIDESYM) {
		int sign = Lexer::sym;
		Lexer::getsym();
		factor(factor_name2, factor_type2);
		if (sign == MULTISYM) {
			quadTable.push_back(Quadruple("*", factor_name1, factor_name2, factor_name1));
		}
		else {
			quadTable.push_back(Quadruple("/", factor_name1, factor_name2, factor_name1));
		}
	}
	if (factor_type1 == INTSYM || factor_type2 == INTSYM) {	//给定termtype, 注意只要不是两个CHARSYM相加,结果均为INT型
		term_type = INTSYM;
	}
	else {
		term_type = CHARSYM;
	}
	term_name = factor_name1;
	tab--;
}

//＜因子＞ ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’|‘(’＜表达式＞‘)’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞ 
void Parser::factor(string &factor_name, int &factor_type) {
	if (debug) {
		tab++;
		tabf();
		cout << "factor" << endl;
	}
	string name = "";
	factor_name = "";
	factor_type = 0;
	switch (Lexer::sym) {
	case IDSYM:	//变量或数组或有返回值的函数调用
		name = Lexer::token;
		if (!symbolTable.in_table(name)) {
			error(UNDEF_ID);
		}
		Lexer::backup();
		Lexer::getsym();
		if (Lexer::sym == LBRACK) {	//是数组
			Lexer::getsym();
			factor_name = newtmpname();
			factor_type = symbolTable.find(name).type;
			symbolTable.var_insert(factor_name, factor_type);
			string arrindex = "";
			int arrtype;
			expression(arrindex, arrtype);
			quadTable.push_back(Quadruple("LODARR", name, arrindex, factor_name));	//将名为name的数组中第arrindex个元素的值存到factor_name对应的临时变量中
			if (Lexer::sym != RBRACK) {
				error(MISSING_RBRACK);
			}
			Lexer::getsym();
		}
		else if (Lexer::sym == LPARENT) {	//是函数,在calfunc中生成相关操作
			Lexer::retrieve();
			calfunc(factor_name, factor_type);
		}
		else {	//是单一变量 //todo:这里可进行优化
			factor_name = newtmpname();
			factor_type = symbolTable.find(name).type;
			symbolTable.var_insert(factor_name, factor_type);
			quadTable.push_back(Quadruple("LODV", factor_name, name, ""));		//将name的值保存到factor_name对应的临时变量中(位于Stack)
		}
		break;
	case LPARENT:
		Lexer::getsym();
		expression(factor_name, factor_type);
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
		break;
	case MINUSSYM:
		Lexer::getsym();
		if (Lexer::sym == NUMTY) {
			factor_name = newtmpname();
			value = -1 * Lexer::value;		//注意需要传value的相反数
			factor_type = INTSYM;
			symbolTable.var_insert(factor_name, INTSYM);
			quadTable.push_back(Quadruple("LODI", factor_name, num2str(value), ""));
		}
		else {
			error(WRONG_TYPE);
		}
		Lexer::getsym();
		break;
	case PLUSSYM:
		Lexer::getsym();
	case NUMTY:
		factor_name = newtmpname();
		value = Lexer::value;
		factor_type = INTSYM;
		symbolTable.var_insert(factor_name, INTSYM);
		quadTable.push_back(Quadruple("LODI", factor_name, num2str(value), ""));
		Lexer::getsym();
		break;
	case CHARTY:
		factor_name = newtmpname();
		factor_type = CHARSYM;
		symbolTable.var_insert(factor_name, CHARSYM);
		quadTable.push_back(Quadruple("LODI", factor_name, num2str(Lexer::value), ""));
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
		error(0);	//正常情况下永不可能走到此分支
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	string label = newlab();
	condition(label);
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	statement();
	quadTable.push_back(Quadruple("LAB", label, "", ""));
	tab--;
}

//＜条件＞::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
void Parser::condition(string label)
{
	if (debug) {
		tab++;
		tabf();
		cout << "condition" << endl;
	}
	int ctype;
	string exp_name1 = "", exp_name2 = "";
	expression(exp_name1, ctype);
	if (Lexer::sym == LESS || Lexer::sym == LESSEQU || Lexer::sym == GREAT || Lexer::sym == GREATEQU || Lexer::sym == NEQUAL || Lexer::sym == EQUAL) {
		int op = Lexer::sym;
		Lexer::getsym();
		expression(exp_name2, ctype);
		switch (op) {		//这里用了点小trick,减少了需要设置label的个数
		case LESS:
			quadTable.push_back(Quadruple("GTE", exp_name1, exp_name2, label));
			break;
		case LESSEQU:
			quadTable.push_back(Quadruple("GREAT", exp_name1, exp_name2, label));
			break;
		case GREAT:
			quadTable.push_back(Quadruple("LTE", exp_name1, exp_name2, label));
			break;
		case GREATEQU:
			quadTable.push_back(Quadruple("LESS", exp_name1, exp_name2, label));
			break;
		case NEQUAL:
			quadTable.push_back(Quadruple("EQU", exp_name1, exp_name2, label));
			break;
		case EQUAL:
			quadTable.push_back(Quadruple("NEQ", exp_name1, exp_name2, label));
			break;
		default:
			break;
		}
	}
	else {
		quadTable.push_back(Quadruple("EQU", exp_name1, "0", label));
	}
	tab--;
}

//＜赋值语句＞:: = ＜标识符＞＝＜表达式＞ | ＜标识符＞‘[’＜表达式＞‘]’ = ＜表达式＞	//todo:finish here
void Parser::assignstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "assign statement" << endl;
	}
	string exname = "";
	string name = "";
	int extype;
	string offsetname = zeroname;
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
		expression(offsetname, type);
		if (Lexer::sym != RBRACK) {
			error(MISSING_RBRACK);
		}
		Lexer::getsym();
	}
	else {

	}
	if (Lexer::sym != ASSIGN) {
		error(0);
	}
	Lexer::getsym();
	expression(exname, extype);
	quadTable.push_back(Quadruple("=", name, offsetname, exname));
	tab--;
}

//＜循环语句＞:: = while ‘(’＜条件＞‘)’＜语句＞
void Parser::whilestate() {
	if (debug) {
		tab++;
		tabf();
		cout << "while statement" << endl;
	}
	string label1 = newlab(), label2 = newlab();
	if (Lexer::sym != WHILESYM) {
		error(0);	//此分支不可达
	}
	quadTable.push_back(Quadruple("LAB", label2, "", ""));
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	condition(label1);
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	statement();
	quadTable.push_back(Quadruple("JMP", label2, "", ""));
	quadTable.push_back(Quadruple("LAB", label1, "", ""));
	tab--;
}

//＜情况语句＞:: = switch ‘(’＜表达式＞‘)’ ‘ { ’＜情况表＞＜缺省＞ ‘ }’
void Parser::switchstate() {
	if (debug) {
		tab++;
		tabf();
		cout << "switch statement" << endl;
	}
	int exp_type;
	string exp_name = "";
	string labfin = newlab();
	if (Lexer::sym != SWITCHSYM) {
		error(0);	//此分支不可达
	}
	Lexer::getsym();
	if (Lexer::sym != LPARENT) {
		error(MISSING_LPARENT);
	}
	Lexer::getsym();
	expression(exp_name, exp_type);
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);
	}
	Lexer::getsym();
	if (Lexer::sym == LBRACE) {		//＜情况表＞::=  ＜情况子语句＞{＜情况子语句＞}
		Lexer::getsym();
		while (Lexer::sym == CASESYM) {
			casestate(exp_name, labfin);
		}
		defaultstate();
	}
	if (Lexer::sym != RBRACE) {
		error(MISSING_RBRACE);
	}
	quadTable.push_back(Quadruple("LAB", labfin, "", ""));
	tab--;
}

//＜情况子语句＞:: = case＜常量＞：＜语句＞
void Parser::casestate(string exp_name, string label) {
	if (debug) {
		tab++;
		tabf();
		cout << "case statement" << endl;
	}
	string caselab = newlab();
	string convalue = newtmpname();
	if (Lexer::sym == CASESYM) {
		Lexer::getsym();
		if (Lexer::sym == NUMTY || Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM) {
			if (Lexer::sym == PLUSSYM) {
				Lexer::getsym();
				value = Lexer::value;
			}
			else if (Lexer::sym == MINUSSYM) {
				Lexer::getsym();
				value = -1 * Lexer::value;
			}
			else {
				value = Lexer::value;
			}
			symbolTable.var_insert(convalue, INTSYM);
			quadTable.push_back(Quadruple("LODI", convalue, num2str(value), ""));
			Lexer::getsym();
		}
		else if (Lexer::sym == CHARTY) {
			Lexer::getsym();
			symbolTable.var_insert(convalue, INTSYM);
			quadTable.push_back(Quadruple("LODI", convalue, num2str(value), ""));
		}
		else {
			error(CASE_TYPE_ERROR);
		}
		if (Lexer::sym != COLON) {
			error(MISSING_COLON);
		}
		quadTable.push_back(Quadruple("NEQ", exp_name, convalue, caselab));
		Lexer::getsym();
		statement();
	}
	else {
		error(0);	//没有case
	}
	quadTable.push_back(Quadruple("JMP", label, "", ""));
	quadTable.push_back(Quadruple("LAB", caselab, "", ""));
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
	string name = "";
	if (Lexer::sym == SCANFSYM) {
		Lexer::getsym();
		if (Lexer::sym == LPARENT) {
			Lexer::getsym();
			if (Lexer::sym != IDSYM) {
				error(MISSING_IDENT);
			}
			name = Lexer::token;
			quadTable.push_back(Quadruple("SCF", name, "", ""));
			Lexer::getsym();
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				if (Lexer::sym != IDSYM) {
					error(MISSING_IDENT);
				}
				name = Lexer::token;
				quadTable.push_back(Quadruple("SCF", name, "", ""));
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
	string str = "", exp_name = "", type = "";
	int exp_type;
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
		string key = "";
		map<string, string>::iterator it;
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
		if (Lexer::sym == COMMA) {
			type = "2";
			Lexer::getsym();
			expression(exp_name, exp_type);
		}
	}
	else {
		type = "1";
		expression(exp_name, exp_type);
	}
	if (Lexer::sym != RPARENT) {
		error(MISSING_RPARENT);	//容错处理
	}
	else {
		Lexer::getsym();
	}
	q.fill("PRT", str, exp_name, type);
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
	string exp_name = "";
	int exp_type;
	if (Lexer::sym != RETURNSYM) {
		error(0);	//正常情况无法达到此分支
	}
	Lexer::getsym();
	if (Lexer::sym == LPARENT) {
		Lexer::getsym();
		expression(exp_name, exp_type);
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		quadTable.push_back(Quadruple("RET", exp_name, "", ""));
		Lexer::getsym();
	}
	else {	//return后无表达式
		quadTable.push_back(Quadruple("RET", "", "", ""));
	}
	retsign = true;
	tab--;
}

//＜有返回值函数调用语句＞ :: = ＜标识符＞‘(’＜值参数表＞‘)’
//＜无返回值函数调用语句＞ :: = ＜标识符＞‘(’＜值参数表＞‘)’
//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Parser::calfunc(string &fac_value, int &fac_type) {
	if (debug) {
		tab++;
		tabf();
		cout << "call function" << endl;
	}
	int exp_type;
	string func_name;
	string para_name;
	fac_value = newtmpname();	
	if (Lexer::sym == IDSYM) {
		func_name = Lexer::token;
		fac_type = symbolTable.find(func_name).type;
		symbolTable.var_insert(fac_value, fac_type);
		Lexer::getsym();
		if (Lexer::sym != LPARENT) {
			error(MISSING_LPARENT);
		}
		Lexer::getsym();
		//预先设置参数，这里有些不科学= =
		int paracount = 0;
		int maxpara = symbolTable.find(func_name).number;
		if (Lexer::sym == IDSYM || Lexer::sym == PLUSSYM || Lexer::sym == MINUSSYM || Lexer::sym == LPARENT || Lexer::sym == NUMTY || Lexer::sym == CHARTY) {

			expression(para_name, exp_type);
			symbolTable.var_insert(para_name, exp_type);
			paracount += 1;
			quadTable.push_back(Quadruple("PARA", para_name, num2str(paracount), func_name));
			while (Lexer::sym == COMMA) {
				Lexer::getsym();
				expression(para_name, exp_type);
				symbolTable.var_insert(para_name, exp_type);
				paracount += 1;
				quadTable.push_back(Quadruple("PARA", para_name, num2str(paracount), func_name));
			}
		}
		if (paracount != maxpara) {
			error(ERROR_PARA_NUM);
		}
		if (Lexer::sym != RPARENT) {
			error(MISSING_RPARENT);
		}
		Lexer::getsym();
		quadTable.push_back(Quadruple("CALL", func_name, "", ""));
	}
	else {
		error(0);
	}
	quadTable.push_back(Quadruple("LODR", fac_value, "$v0", ""));	//在函数的返回值(位于$v0)写回到栈中
	tab--;
}