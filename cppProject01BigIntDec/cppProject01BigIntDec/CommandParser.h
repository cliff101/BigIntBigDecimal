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
2 ! 由左至右
3 ^ 由右至左
4 +- (正負號) 由右至左
5 * / 由左至右
6 + - 由左至右
7 )
*/

class CommandParser {
public:
	CommandParser();
	//輸入command
	void Inputcommand(string);

	//將等號右邊的值賦予給等號左邊的變數
	BigDecimal Getcommandvalue(string, BigDecimal&);
	BigInt Getcommandvalue(string, BigInt&);

	//檢查是否為command
	bool IsCommand(string&);
private:
	struct BigValue {//用於同時儲存BigInt及BigDecimal
		bool IsInt = true;
		BigInt BigIntVal = 0;
		BigDecimal BigDecimalVal;
	};
	struct retmsg {//retmsg為統一回傳的格式
	public:
		short mode = 0;//0 = normal  1 = set  2 = delete
		BigValue value;
		int priority = 0;
		bool ok = true;
		string errmsg = "";
		string description = "";
		string varname = "";
	};
	//用於儲存變數
	map<string, BigValue> BigValueVar;
	//string getallvar();

	//主解析command的程式
	retmsg ProceedCommand(string& cmd, unsigned long long& position, int priority, bool isgiveval1 = false, BigValue giveval1 = BigValue());
	//由command提取數值
	retmsg getvalue(string& cmd, unsigned long long& position,bool varerr = true);
	//由command提取operator
	retmsg getoperator(string& cmd, unsigned long long& position);
	//計算數值
	retmsg calculate(BigValue& val1, string& operatorval, BigValue& val2);
	//計算階乘
	retmsg stagecalc(BigValue& val1);
	bool isnumeric(char c);
	bool isalphabetic(char c);
	bool isint(string& in);
	//檢查括號是否左右對稱
	int checkb(string& in, unsigned long long int& i);
	//檢查輸入是否為變數設定模式
	retmsg checkset(string& in, unsigned long long int& i);
	//空白飄移
	void blankslide(string& cmd, unsigned long long& position);
	//創建錯誤訊息
	retmsg createerrmsg(unsigned long long int i, string info);
};