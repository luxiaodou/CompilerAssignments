#pragma once
#include "global.h"
#include <string>
using namespace std;

class TableItem
{
public:
	string	name;	//符号的唯一标识
	int		addr;	//符号的地址,针对参数以及临时变量而言
	int		kind;	//符号所属的种类
	int		type;	//符号的数据类型
	int		value;	//符号的值
	int		number;	//参数或者数组中元素的个数
	int		level;	//符号所述层

	TableItem();
	TableItem(string na, int add, int kin, int typ, int val, int num,int level);
	~TableItem();
};

