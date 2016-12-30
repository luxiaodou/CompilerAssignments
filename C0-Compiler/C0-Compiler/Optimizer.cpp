#include "stdafx.h"
#include "global.h"

void DAG();
void constfolding();	//常数合并
void dsanalyse();	//数据流分析
void registeropt();	//寄存器优化
void loopopt();			//循环优化