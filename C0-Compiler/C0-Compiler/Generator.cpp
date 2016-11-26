#include "StdAfx.h"
#include "Generator.h"
Generator::Generator(string path)
{
	asmfile.open(path);
}


Generator::~Generator()
{
}

void Generator::quad2asm(Quadruple quad)
{
	//todo:先把.data写进去,然后在完成.text
	//.data需要写入consttab和strtable中的内容
	//常量声明
	if (quad.op == "CON") {	//para1 - name, para2 - type para3 - value. all string type
		if (quad.para2 == "INT") {
			asmfile << quad.para1 << ": .word " << quad.para3 << endl;
		}
		else if (quad.para2 == "CHAR") {
			asmfile << quad.para1 << ": .asciiz \"" << quad.para3 << "\"" << endl;
		}
	}
	//todo: 全局变量和局部变量应采取不同的处理方式,因为局部变量存在栈中
	else if (quad.op == "VAR") {	//para1 - name, para2 - type, para3 - no_use
		if (quad.para2 == "INT") {
			asmfile << quad.para1 << ": .space 4" << endl;
		}
		if (quad.para2 == "CHAR") {
			asmfile << quad.para1 << ": .space 4" << endl;	//todo:将来可以考虑精简char类型的存储空间,现在先保持
		}
	}
	else if (quad.op == "ARR") {	//para1 - name, para2 - type, para3 - ele_num
		if (quad.para2 == "INT") {
			asmfile << quad.para1 << ": .word 1:" << quad.para3 << endl;
		}
		if (quad.para2 == "CHAR") {
			asmfile << quad.para1 << ": .word 1:" << quad.para3 << endl;	//todo:char的储存空间是否可优化
		}
	}
	else if (quad.op == "PARA") {
		if (quad.para2 == "INT") {
			//todo:implement para generate, PARA存在运行栈中
		}
	}
	else if (quad.op == "+") {
		asmfile << "la $t1, " << quad.para1 << endl;
		asmfile << "la $t2, " << quad.para2 << endl;
		asmfile << "add $t1,$t2,$t3" << endl;
		asmfile << "la $t4, " << quad.para3 << endl;
		asmfile << "sw $t3,0($t4)" << endl; //todo:check if it is right
	}
	else if (quad.op == "-") {
		asmfile << "la $t1, " << quad.para1 << endl;
		asmfile << "la $t2, " << quad.para2 << endl;
		asmfile << "sub $t1,$t2,$t3" << endl;
		asmfile << "la $t4, " << quad.para3 << endl;
		asmfile << "sw $t3,0($t4)" << endl; //todo:check if it is right
	}
	else if (quad.op == "*") {
		//todo:finish here
	}
	else if (quad.op == "/") {
		//todo:finish here
	}
	else if (quad.op == "PRT") {
		if (quad.para3 == "0") {	//只用string
		}
		else if (quad.para3 == "1") {	//只用exp
		}
		else if (quad.para3 == "2") {	//二者兼有

		}
		else {
			cout << "error PRT" << endl;
		}
	}
}
