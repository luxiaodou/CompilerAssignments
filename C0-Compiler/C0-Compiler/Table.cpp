#include "StdAfx.h"
#include "Table.h"


Table::Table(string name)
{
	size = 0;
	tabtype = 0;
	tablename = name;
}


Table::~Table()
{
}

bool Table::in_table(string iname)
{
	for (auto item : items) {
		if (item.name == iname) {
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
