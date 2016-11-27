#include "StdAfx.h"
#include "TableItem.h"


TableItem::TableItem()
{
	this->name = "";
	this->addr = 0;
	this->kind = 0;
	this->type = 0;
	this->value = 0;
	this->number = 0;
}

TableItem::TableItem(string na, int add, int kin, int typ, int val, int num,string father)
{
	this->name = na;
	this->addr = add;
	this->kind = kin;
	this->type = typ;
	this->value = val;
	this->number = num;
	this->father = father;
}


TableItem::~TableItem()
{
}
