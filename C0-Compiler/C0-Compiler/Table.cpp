#include "StdAfx.h"
#include "Table.h"


Table::Table(string name)
{
	offset = 0;
	curlevel = 0;
	topfunction = name;
	curfunction = topfunction;
	func_insert(curfunction, VOIDSYM, 0);
}


Table::~Table()
{
}

bool Table::in_table(string iname)
{
	for (int i = curlevel; i < items.size();i++) {
		if (items[i].name == iname) {
			return true;
		}
		if (items[i].kind == FUNC && i != curlevel) {
			break;
		}
	}
	for (int i = 1; i < items.size(); i++) {
		if (items[i].name == iname && items[i].father == topfunction) {
			return true;
		}
	}
	return false;
}

bool Table::in_cur_level(string name)
{
	for (int i = curlevel + 1; i < items.size(); i++) {	//todo:check here
		if (items[i].name == name) {
			return true;
		}
		if (items[i].kind == FUNC && i != curlevel) {
			break;
		}
	}
	return false;
}

TableItem Table::find(string name)
{
	TableItem item;
	int length = items.size();
	for (int i = curlevel + 1; i < length; i++) {	//查找当前层
		item = items[i];
		if (item.name == name) {
			return item;
		}
		if (item.kind == FUNC) {
			break;
		}
	}
	for (int i = 1; i < length; i++) {	//查找全局变量
		item = items[i];
		if (item.name == name && item.father == topfunction) {
			return item;
		}		
	}
	return item;
}

TableItem Table::findf(string name)
{
	map<string, int>::iterator it;
	int loc;
	for (it = funcloc.begin(); it != funcloc.end(); it++) {
		if (it->first == name) {
			loc = it->second;
			break;
		}
	}
	return items[loc];
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

int Table::setfuncnum(string name, int number)
{
	if (!in_table(name)) {
		return 1;
	}
	for (int i = curlevel; i < items.size(); i++) {
		if (items[i].name == name && items[i].kind == FUNC) {
			items[i].number = number;
			return 0;
		}
	}
	return 1;
}

int Table::setfuncsize()
{
	items[curlevel].value = offset;
	return 0;
}

int Table::setzero(string name)
{
	for (int i = 0; i < items.size();i++) {
		if (items[i].name == name) {
			items[i].addr = 0;
			items[i].value = 0;
		}
	}
	return 0;
}

//向符号表中插入常量
int Table::con_insert(string name, int type, int value)
{
	if (in_cur_level(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, offset, CON, type, value, 0, curfunction));
	offset += 4;
	return 0;
}

//向符号表中插入变量
int Table::var_insert(string name, int type)
{
	if (in_cur_level(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, offset, VAR, type, 0, 0, curfunction));
	offset += 4;
	return 0;
}

int Table::arr_insert(string name, int type, int num)
{
	if (in_cur_level(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, offset, ARR, type, 0, num, curfunction));
	offset += 4 * num;
	return 0;
}

int Table::para_insert(string name, int type)
{
	if (in_cur_level(name)) {
		return 1;
	}
	this->items.push_back(TableItem(name, offset, PARA, type, 0, 0, curfunction));
	offset += 4;
	return 0;
}

int Table::func_insert(string name, int type, int num)
{
	if (in_table(name)) {
		return 1;
	}
	funcloc[name] = items.size();
	offset = 0;
	items.push_back(TableItem(name, 0, FUNC, type, 0, num, topfunction));
	curlevel = funcloc[name];
	curfunction = name;
	offset += 4;
	return 0;
}

