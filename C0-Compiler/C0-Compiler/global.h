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
class Table;
using namespace std;

extern int errorNum;
extern int stbIndex;
extern vector<Quadruple> quadTable;
extern map<string, string> strTable;
extern Table symbolTable;




