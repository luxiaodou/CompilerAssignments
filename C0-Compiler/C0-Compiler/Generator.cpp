#include "StdAfx.h"
#include "Generator.h"
#include "Table.h"
Generator::Generator(string path)
{
	asmfile.open(path);
}


Generator::~Generator()
{
}

void Generator::quad2asm(Quadruple quad)
{
	//todo:先把.data写进去,然后再完成.text,.data需要写入consttab和strtable中的内容
	//常量声明
	if (quad.op == "CON") {	//para1 - name, para2 - type para3 - value. all string type
		asmfile << quad.para1 << ": .word " << quad.para3 << endl;
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
		else {

		}
	}
	else if (quad.op == "FUNC") {
		int funcsize = symbolTable.find(quad.para1).value;
		string funcname = symbolTable.find(quad.para1).name;
		asmfile << funcname << ":" << endl;		//给出label
		asmfile << "subi $sp, $sp, " << funcsize << endl;	//开辟栈空间
		asmfile << "sw $ra,0($sp)" << endl;		//保存ra
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
	else if (quad.op == "=") {	//取地址→赋值→存回原地址
		int addr1 = symbolTable.find(quad.para1).addr;
		int addr2 = symbolTable.find(quad.para2).addr;
		if (addr2) {

		}
		else {	//addr2为0,说明是全局变量

		}
		//asmfile << 
	}
	else if (quad.op == "PRT") {
		if (quad.para3 == "0") {	//只用string
			asmfile << "li $v0, 4" << endl;			//todo :check 输出字符串是不是v0 = 4
			asmfile << "la $a0," << quad.para1 << endl;
			asmfile << "syscall" << endl;
		}
		else if (quad.para3 == "1") {	//只用exp
		}
		else if (quad.para3 == "2") {	//二者兼有

		}
		else {
			cout << "error PRT" << endl;
		}
	}
	else if (quad.op == "CALL") {
		asmfile << "jal " << quad.para1 << endl;
	}
	else if (quad.op == "END") {
		asmfile << "lw $ra,0($sp)" << endl;
		//todo: 将函数所开辟的空间退栈
		asmfile << "jr $ra" << endl;
	}
	else if (quad.op == "LAB") {
		asmfile << quad.para1 << ":" << endl;
	}
	else if (quad.op == "GLBEND") {
		map<string, string>::iterator it;
		for (it = strTable.begin(); it != strTable.end(); it++) {	//写入所有字符串常量
			asmfile << it->first << ": .asciiz \"" << it->second << "\"" << endl;
		}
		asmfile << ".text" << endl;
		asmfile << "j main" << endl;;
	}
	else if (quad.op == "GLBSTR") {
		asmfile << ".data" << endl;
	}
}

void Generator::work()
{
	int index = 0;
	int quadnum = quadTable.size();
	bool global = true;		//声明全局变量
	for (; index < quadnum; index++) {
		Quadruple q = quadTable[index];
		if (q.op == "GLBEND") {
			global = false;	//全局常变量声明结束
		}
		quad2asm(q);
	}
}

void Generator::close()
{
	asmfile.close();
}
