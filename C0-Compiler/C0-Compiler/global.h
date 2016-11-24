#pragma once
/////////////////////////global include/////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "error.h"
#include "Quadruple.h"
using namespace std;

//vector<Quadruple> quadtable;
extern int errornum;

/////////////////////////symbol table/////////////////////////////////
#define TABLESIZE 1000 //符号表大小
#define MAXLAYER 2	//符号表最大层次
typedef struct	//定义表项
{
	string name;	//符号的名字
	int addr;	//符号的地址
	int kind;	//符号的大类别
	int type;	//符号的数据类型
	int value;	//符号的值
	int number;	//参数或者数组中元素的个数
} tableitem;

typedef struct		//定义符号表
{
	tableitem item[TABLESIZE];	//建立数组存储单个表项
	int levelindex[MAXLAYER];
	int top;		//符号表标顶位置
}symtable;

///////////////////////////error.cpp//////////////////////////////////
void error(int errid);

/////////////////////////quad.cpp////////////////////////////////////


/////////////////////////quad2asm.cpp//////////////////////////////


