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
	BigInt(const BigDecimal&);

	BigInt operator=(string);
	BigInt operator=(int);
	BigInt operator=(const BigDecimal&);


	BigInt operator+(const BigInt&) const;
	BigInt operator-(const BigInt&) const;
	BigInt operator*(const BigInt&) const;
	BigInt operator/(const BigInt&) const;
	BigInt operator%(const BigInt&) const;

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

	BigDecimal Power(BigDecimal&);
	BigInt Power(BigInt&);

	vector<short> Getval();
	string Getvalreal();
	long long int Getint();
	bool Getsign();
	void Setsign(bool);
	bool Getinf();
	bool Getundefined();
	int Getnumlength();
private:
	vector<short> val;
	bool isinf;
	bool isundefined;

	bool checkbig(const BigInt&, const BigInt&, bool isunsigned = false) const;//true = 前  false = 後
	short compair(const BigInt&, const BigInt&) const;//1 = 前 0 = 等於 -1 = 後
	void erasezero(BigInt&) const;
	string stringprettify(string);
	bool sign;//0 = 負號  1 = 正號
};