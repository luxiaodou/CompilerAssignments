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
	//todo:finish quad2asm
	if (quad.op	== "PRT") {
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
