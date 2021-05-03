#pragma once
#include <vector>
#include <string>
#include <cmath>
#include "BigValue.h"

using namespace std;

class BigDecimal;
class CommandParser;
class BigInt :public BigValue {
public:
	BigInt();
	BigInt(int);
	BigInt(string);
	BigInt(const BigInt&);
	//BigDecimal轉BigInt，執行除法
	BigInt(const BigDecimal&);
	BigInt(const char*);

	BigInt operator=(string);
	BigInt operator=(int);
	//BigDecimal轉BigInt，執行除法
	BigInt operator=(const BigDecimal&);


	BigInt operator+(const BigInt&) const;
	BigInt operator-(const BigInt&) const;
	BigInt operator*(const BigInt&) const;
	BigInt operator/(const BigInt&) const;
	BigInt operator%(const BigInt&) const;

	BigInt operator+(const BigInt&);
	BigInt operator-(const BigInt&);
	BigInt operator*(const BigInt&);
	BigInt operator/(const BigInt&);
	BigInt operator%(const BigInt&);

	BigInt operator+(BigInt&);
	BigInt operator-(BigInt&);
	BigInt operator*(BigInt&);
	BigInt operator/(BigInt&);
	BigInt operator%(BigInt&);

	BigInt operator+=(const BigInt&);
	BigInt operator-=(const BigInt&);
	BigInt operator*=(const BigInt&);
	BigInt operator/=(const BigInt&);
	BigInt operator%=(const BigInt&);

	BigInt operator++(int);

	bool operator==(const BigInt&)const;
	bool operator!=(const BigInt&)const;
	bool operator>(const BigInt&)const;
	bool operator>=(const BigInt&)const;
	bool operator<(const BigInt&)const;
	bool operator<=(const BigInt&)const;

	friend ostream& operator<<(ostream& ccout,const BigInt& in);
	friend istream& operator>>(istream& ccin, BigInt& in);

	BigDecimal Power(BigDecimal&);
	BigInt Power(BigInt&);
	//BigDecimal Power(BigDecimal&);
	//BigInt Power(BigInt&);

	vector<short> Getval();
	string Getvalreal();
	long long int Getint();
	bool Getsign();
	void Setsign(bool);
	bool Getinf();
	bool Getundefined();
	int Getnumlength();
	int Getvallength();
private:
	//定義BigInt底層資料結構 val 為 vector<short>
	vector<short> val;
	//紀錄inf狀態
	bool isinf;
	//紀錄undefined狀態
	bool isundefined;

	//檢查是否大於
	bool checkbig(const BigInt&, const BigInt&, bool isunsigned = false) const;//true = 前  false = 後
	//比較大小
	short compair(const BigInt&, const BigInt&) const;//1 = 前 0 = 等於 -1 = 後
	//美化BigInt
	void erasezero(BigInt&) const;
	//美化格式為整數的string
	string stringprettify(string);
	//儲存正負號狀態
	bool sign;//0 = 負號  1 = 正號
protected:
	//給API整合operator<<用
	void Print(ostream& ccout) const;
};