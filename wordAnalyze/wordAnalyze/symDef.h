#ifndef	_SYMDEF_H_
#define _SYMDEF_H_

//符号运算符类型
#define		PLUSSYM		1	// +
#define		MINUSSYM	2	// -
#define		MULTISYM	3	// *
#define		DEVIDESYM	4	// /
#define		QUOTE		5	//'
#define		LBRACK		6	//[
#define		RBRACK		7	//]
#define		LPARENT		8	//(
#define		RPARENT		9	//)
#define		LBRACE		10	//{
#define		RBRACE		11	//}
#define		COLON		12	//:
#define		COMMA		13	//,
#define		SEMICOLON	14	//;
#define		EQUAL		15	//==
#define		ASSIGN		16	//=
#define		LESS		17	//<
#define		LESSEQU		18	//<=
#define		GREAT		19	//>
#define		GREATEQU	20	//>=
#define		NEQUAL		21	//!=
#define		DOUQUOTE	22	//"

//符号值类型
#define		IDSYM		23	//单词是标识符
#define		NUMTY		24	//单词是整数(num type)
#define		CHARTY		25	//单词是字母(char type)
#define		STRING		26	//单词是字符串

//保留字
#define		MAINSYM		27	//main
#define		INTSYM		28	//int
#define		CHARSYM		29	//char
#define		CONSTSYM	30	//const
#define		VOIDSYM		31	//void
#define		IFSYM		32	//if
#define		WHILESYM	33	//else
#define		SWITCHSYM	34	//switch
#define		CASESYM		35	//case
#define		DEFAULTSYM	36	//default
#define		SCANFSYM	37	//scanf
#define		PRINTFSYM	38	//printf
#define		RETURNSYM	39	//return

#endif