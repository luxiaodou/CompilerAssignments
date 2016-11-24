#pragma once
#include "global.h"
#include <string>
using namespace std;

class TableItem
{
public:
	string	name;	//符号的唯一标识
	int		addr;	//符号的地址
	int		kind;	//符号所属的种类
	int		type;	//符号的数据类型
	int		value;	//符号的值
	int		number;	//参数或者数组中元素的个数

	TableItem();
	~TableItem();
};

