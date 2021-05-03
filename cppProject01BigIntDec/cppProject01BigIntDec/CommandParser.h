#pragma once
#include "BigDecimal.h"
#include "BigInt.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include <signal.h>
#include <csetjmp>

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
	//��Jcommand
	void Inputcommand(string);

	//�N�����k�䪺�Ƚᤩ���������䪺�ܼ�
	BigDecimal Getcommandvalue(string, BigDecimal&);
	BigInt Getcommandvalue(string, BigInt&);

	//�ˬd�O�_��command
	bool IsCommand(string&);
private:
	struct BigValue {//�Ω�P���x�sBigInt��BigDecimal
		bool IsInt = true;
		BigInt BigIntVal = 0;
		BigDecimal BigDecimalVal;
	};
	struct retmsg {//retmsg���Τ@�^�Ǫ��榡
	public:
		short mode = 0;//0 = normal  1 = set  2 = delete
		BigValue value;
		int priority = 0;
		bool ok = true;
		string errmsg = "";
		string description = "";
		string varname = "";
	};
	//�Ω��x�s�ܼ�
	map<string, BigValue> BigValueVar;
	//string getallvar();

	//�D�ѪRcommand���{��
	retmsg ProceedCommand(string& cmd, unsigned long long& position, int priority, bool isgiveval1 = false, BigValue giveval1 = BigValue());
	//��command�����ƭ�
	retmsg getvalue(string& cmd, unsigned long long& position,bool varerr = true);
	//��command����operator
	retmsg getoperator(string& cmd, unsigned long long& position);
	//�p��ƭ�
	retmsg calculate(BigValue& val1, string& operatorval, BigValue& val2);
	//�p�ⶥ��
	retmsg stagecalc(BigValue& val1);
	bool isnumeric(char c);
	bool isalphabetic(char c);
	bool isint(string& in);
	//�ˬd�A���O�_���k���
	int checkb(string& in, unsigned long long int& i);
	//�ˬd��J�O�_���ܼƳ]�w�Ҧ�
	retmsg checkset(string& in, unsigned long long int& i);
	//�ť��Ʋ�
	void blankslide(string& cmd, unsigned long long& position);
	//�Ыؿ��~�T��
	retmsg createerrmsg(unsigned long long int i, string info);
};