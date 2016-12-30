#pragma once
#include "TableItem.h"
#include "defination.h"
#include <vector>
#include <string>
using namespace std;
class TableItem;
class Quadruple;
class Table
{
public:
	string	topfunction;			//全局层
	vector<TableItem> items;	//符号表表项
	Table(string name);															
	~Table();
	int		offset;				//符号在栈中的偏移
	map<string, int>	funcloc;	//查找函数在符号表中对应的位置
	int		curlevel;			//符号表当前层的起始位置
	string	curfunction;		//当前函数名

	//建表
	bool in_table(string name);	//检查name是否已经存在于符号表中(当前层或者全局变量)	
	bool fin_table(string name); //检查是否存在函数名为name的函数
	bool in_cur_level(string name);;	//检查当前层有没有name变量
	bool value_in_table(int val);	//检查表中是否存在value为val的项
	int con_insert(string name, int type, int value);	//向符号表中添加常量,返回0表示添加成功,1表示添加失败,下同
	int var_insert(string name, int type);				//向符号表中插入单一变量
	int arr_insert(string name, int type, int num);		//向符号表中插入数组变量,此文法中仅限一维数组
	int para_insert(string name, int type);			//向符号表中插入参数
	int func_insert(string name, int type, int num);	//向符号表中插入函数

	TableItem find(string name);	//取出名字为name的表项
	TableItem findf(string name);	//取出名字为name的函数

	bool is_con(string name);
	bool is_var(string name);
	bool is_arr(string name);
	bool is_para(string name);
	bool is_func(string name);

	int getkind(string name);
	int gettype(string name);
	int getvalue(string name);

	int setfuncnum(string name,int number);	//返回0表示设置成功,否则失败
	int setfuncsize();	//设置函数的所占用栈空间的大小
	int setzero(string name);
};

