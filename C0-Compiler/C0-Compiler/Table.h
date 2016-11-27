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
	int		curlevel;			//符号表当前层的起始位置
	string	topfunction;			//全局层
	string	curfunction;		//当前函数名
	vector<TableItem> items;	//符号表表项
public:
	Table(string name);															
	~Table();
	int		offset;				//符号在栈中的偏移
	map<string, int>	funcloc;	//查找函数在符号表中对应的位置

	//建表
	bool in_table(string name);	//检查name是否已经存在于符号表中	//todo:改为存在于当前层的符号表中
	bool value_in_table(int val);	//检查常量表中是否存在value为val的项
	int con_insert(string name, int type, int value);	//向符号表中添加常量,返回0表示添加成功,1表示添加失败,下同
	int var_insert(string name, int type);				//向符号表中插入单一变量
	int arr_insert(string name, int type, int num);		//向符号表中插入数组变量,此文法中仅限一维数组
	int para_insert(string name, int type);			//向符号表中插入参数
	int func_insert(string name, int type, int num);	//向符号表中插入函数
	int temp_insert(string name, int type);	//用来存放临时变量的值,例如表达式的值等等

	TableItem find(string name);	//取出名字为name的表项

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

};

