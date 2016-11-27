#pragma once
#include "global.h"
#include <string>
using namespace std;

class TableItem
{
public:
	string	name;	//符号的唯一标识
	int		addr;	//符号在运行中的地址
	int		kind;	//符号所属的种类
	int		type;	//符号的数据类型
	int		value;	//符号的值,对函数保存函数占用栈空间的大小
	int		number;	//参数或者数组中元素的个数
	string	father;	//对符号是函数,对函数是指向其父函数

	TableItem();
	TableItem(string na, int add, int kin, int typ, int val, int num,string father);
	~TableItem();
};

