#pragma once
#include <string>
using namespace std;
class Quadruple {	
public:
	string op;
	string para1;
	string para2;
	string para3;
	Quadruple();
	Quadruple(string a, string b, string c, string d);
	~Quadruple();
	void fill(string op,string para1,string para2, string para3);
private:
};