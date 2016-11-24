#include "stdafx.h"
#include "global.h"
#include "Quadruple.h"


Quadruple::Quadruple()
{
}

Quadruple::Quadruple(string a, string b, string c, string d)
{
	op = a;
	para1 = b;
	para2 = c;
	para3 = d;
}

Quadruple::~Quadruple()
{
}

void Quadruple::fill(string op,string para1, string para2, string para3)
{
	this->op = op;
	this->para1 = para1;
	this->para2 = para2;
	this->para3 = para3;
}

