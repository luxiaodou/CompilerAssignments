// C0-Compiler.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "global.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//getpath();
	path = "C:/Users/luxiaodou/Desktop/13071079_test.txt";
	bool flag = false;
	bool nflag = true;
	//path = "C:/Users/luxiaodou/Desktop/a.txt";
	infile.open(path, ios::in);
	while (infile.good()) {
		getsym();

		if (line == 1 && nflag) {
			backup();
			nflag = false;
		}
		if (line == 3 && !flag) {
			retrieve();
			flag = true;
		}
		output();
	}
	cout << "Word Analyze complete!" << endl;
	infile.close();
	outfile.close();
	return 0;
}

