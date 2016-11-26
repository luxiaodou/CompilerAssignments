#include "StdAfx.h"
#include "Table.h"


Table::Table(string name)
{
	size = 0;
	tabtype = 0;
	curlevel = 0;
	tablename = name;
}


Table::~Table()
{
}

bool Table::in_table(string iname)
{
	/*for (auto item : this->items) {
		if (item.name == iname) {
			return true;
		}
	}*/
	for (int i = 0; i < items.size(); i++) {
		if (items[i].name == iname) {
			return true;
		}
	}
	return false;
}

void Table::settype(int type)
{
	tabtype = type;
}

void Table::symbol_insert(TableItem item)
{
	items.push_back(item);
}

TableItem Table::find(string name)
{
	TableItem item;
	int length = items.size();
	for (int i = 0; i < length; i++) {
		item = items[i];
		if (item.name == name) {
			break;
		}
	}
	return item;
}

bool Table::is_con(string name)
{
	if (in_table(name)) {
		TableItem ti = find(name);
		if (ti.kind == CON) {
			return true;
		}
	}
	return false;
}

bool Table::is_var(string name)
{
	if (in_table(name)) {
		TableItem ti = find(name);
		if (ti.kind == VAR) {
			return true;
		}
	}
	return false;
}

bool Table::is_arr(string name)
{
	if (in_table(name)) {
		TableItem ti = find(name);
		if (ti.kind == ARR) {
			return true;
		}
	}
	return false;
}

bool Table::is_para(string name)
{
	if (in_table(name)) {
		TableItem ti = find(name);
		if (ti.kind == PARA) {
			return true;
		}
	}
	return false;
}

bool Table::is_func(string name)
{
	if (in_table(name)) {
		TableItem ti = find(name);
		if (ti.kind == FUNC) {
			return true;
		}
	}
	return false;
}

int Table::getkind(string name)
{
	if (in_table(name)) {
		TableItem ti = find(name);
		return ti.kind;
	}
	return 0;
}

int Table::gettype(string name)
{
	if (in_table(name)) {
		return find(name).type;
	}
	return -1;
}

int Table::getvalue(string name)
{
	if (in_table(name)) {
		return find(name).value;
	}
	return 0;
}

int Table::setnum(string name,int number)
{
	if (!in_table(name)) {
		return 1;
	}
	for (int i = 0; i < items.size();i++) {
		if (items[i].name==name) {
			items[i].number = number;
			return 0;
		}
	}
	return 1;
}

void Table::poplevel()
{
	for (int i = items.size(); i >= 0;i--) {
		if (items[i].level>curlevel) {
			items.pop_back();
		}
	}
}

//向符号表中插入常量
int Table::con_insert(string name, int type, int value)
{
	if (in_table(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, 0, CON, type, value, 0,curlevel));
	return 0;
}


int Table::var_insert(string name, int type)
{
	if (in_table(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, 0, VAR, type, 0, 0, curlevel));
	return 0;
}

int Table::arr_insert(string name, int type, int num)
{
	if (in_table(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, 0, ARR, type, 0, num, curlevel));
	return 0;
}

int Table::para_insert(string name, int type)
{
	if (in_table(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, 0, PARA, type, 0, 0, curlevel));
	return 0;
}

int Table::func_insert(string name, int type, int num)
{
	if (in_table(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, 0, FUNC, type,0, num, curlevel));
	return 0;
}

int Table::temp_insert(string name, int type)
{
	if (in_table(name)) {
		return 1;
			}
	this->items.push_back(TableItem(name, 0, TEMP, type, 0, 0, curlevel));
	return 0;
}

void Table::addlevel()
{
	curlevel++;
}

void Table::decllevel()
{
	curlevel--;
}
