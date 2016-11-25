#pragma once
/////////////////////////global include/////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "error.h"
#include "Quadruple.h"
#include "Table.h"
using namespace std;

extern int errornum;
extern int stbindex;
extern vector<Quadruple> quadtable;
extern map<string, string> strtable;
extern Table constTable;
extern Table symbolTable;




