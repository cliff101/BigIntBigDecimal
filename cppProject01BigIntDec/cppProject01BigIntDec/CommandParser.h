#pragma once
#include "BigDecimal.h"
#include "BigInt.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;


/*
1 (
2 ! �ѥ��ܥk
3 ^ �ѥk�ܥ�
4 +- (���t��) �ѥk�ܥ�
5 * / �ѥ��ܥk
6 + - �ѥ��ܥk
7 )
*/

class CommandParser {
public:
	CommandParser();
	void Inputcommand(string);
private:
	struct BigValue {
		bool IsInt = true;
		BigInt BigIntVal;
		BigDecimal BigDecimalVal;
	};
	struct retmsg {
	public:
		BigValue value;
		int priority = 0;
		bool haveoutput = true;
		bool ok = true;
		string errmsg = "";
		string description = "";
	};
	map<string, BigInt> BigIntVar;
	map<string, BigDecimal> BigDecimalVar;
	string getallvar();

	retmsg ProceedCommand(string& cmd, unsigned long long& position, int priority, bool isgiveval1 = false, BigValue giveval1 = BigValue());
	retmsg getvalue(string& cmd, unsigned long long& position);
	retmsg getoperator(string& cmd, unsigned long long& position);
	retmsg calculate(BigValue& val1, string& operatorval, BigValue& val2);
	retmsg stagecalc(BigValue& val1);
	bool isnumeric(char c);
	bool isint(string& in);
	void blandslide(string& cmd, unsigned long long& position);
};