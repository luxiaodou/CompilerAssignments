#include "stdafx.h"
#include "global.h"

void quad2asm();	//将生成的四元式转化为汇编代码
void insertasmsym();	//生成汇编符号表
void findsym(const string &);	//找到符号在符号表中的位置
void	pushstack(int len);	//进行压栈操作
void setlab();	//生成新的label
void jumpasm();	//生成跳转指令
void assignasm();	//生成赋值语句
void ifasm(); //生成条件语句
void switchasm();	//生成分支语句
void constdeclasm();	//常量定义语句
void vardeclasm();	//变量定义语句
void callasm();	//函数调用语句
void paraasm();	//参数处理语句
void scanfasm();	//处理scanf
void printfasm();	//处理printf
void returnasm();	//生成return语句