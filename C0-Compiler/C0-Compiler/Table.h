#pragma once
#include "TableItem.h"
#include <vector>
#include <string>
using namespace std;
class Table
{
	//todo: not here
	int		size;				//当前符号表的大小
	int		tabtype;			//符号表的类型
	string	tablename;			//符号表的名称
	vector<TableItem> items;	//符号表表项
public:
	Table(string name);															
	~Table();
	bool in_table(string name);	//检查name是否已经存在于符号表中
	void settype(int type);		//设置符号表类型
	void symbol_insert(TableItem item);	//插入符号
	TableItem find(string name);	//取出名字为name的表项
	bool value_in_table(int val);	//检查常量表中是否存在value为val的项

};

