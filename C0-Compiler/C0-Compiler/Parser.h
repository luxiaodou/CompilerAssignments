#pragma once
#include "defination.h"
#include "error.h"
#include <set>
using namespace std;
class Parser {
public:
	static void program();	//处理程序的递归子程序
private:
	static void error(int errorid);
	//static void enter();		//todo: 修改文档,说明此方法已被转移
	static void skip(set<int> symset);	//错误处理的跳读方法
	static void conststate();		//处理常量说明的递归子程序
	static void constdef();	//处理常量定义的递归子程序
	static void varstate();		//处理变量声明的递归子程序
	static void vardef();		//处理变量定义的递归子程序
	static void paralist();		//处理函数声明时参数表的递归子程序
	static void compound();	//处理符合语句的递归子程序
	static void funcdef();	//处理有返回值函数声明的递归子程序
	static void voidfdef();	//处理无返回值的函数声明的递归子程序
	static void mainfdef();	//处理主函数声明的递归子程序
	static void statement();		//处理语句的递归子程序
	static void expression(string &, int &);	//处理表达式的递归子程序
	static void term(string &,int &);	//处理项的递归子程序
	static void factor(string &,int &);	//处理因子的递归子程序
	static void assignstate();	//处理赋值语句的递归子程序
	static void ifstate();	//处理if语句的递归子程序
	static void condition(string );	//处理条件语句的递归子程序
	static void whilestate();	//处理while循环语句的递归子程序
	static void switchstate();	//处理switch的递归子程序
	static void casestate();	//处理case的递归子程序
	static void defaultstate();	//处理default的递归子程序
	static void printfstate();		//处理printf的递归子程序
	static void scanfstate();		//处理scanf的递归子程序
	static void returnstate();	//处理return的递归子程序
	static void calfunc(string &);	//处理函数调用的递归子程序
};