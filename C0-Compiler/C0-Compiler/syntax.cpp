#include "stdafx.h"
#include "global.h"

void enter();		//将符号存入符号表
void program();	//处理程序的递归子程序
void condecl();		//处理常量说明的递归子程序
void constdef();	//处理常量定义的递归子程序
void vardecl();		//处理变量声明的递归子程序
void vardef();		//处理变量定义的递归子程序
void headdef();	//处理头声明的递归子程序
void paralist();		//处理函数声明时参数表的递归子程序
void funcdecl();	//处理函数声明的递归子程序
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
