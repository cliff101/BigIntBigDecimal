#pragma once
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class BigDecimal;
class BigInt {
public:
	BigInt();
	BigInt(int);
	BigInt(string);
	BigInt(const BigInt&);
	BigInt(const BigDecimal&);

	BigInt operator=(string);
	BigInt operator=(int);
	BigInt operator=(const BigDecimal&);

	BigInt operator+(const BigInt&);
	BigInt operator-(const BigInt&);
	BigInt operator*(const BigInt&);
	BigInt operator/(const BigInt&);
	BigInt operator%(const BigInt&);

	BigInt operator+=(const BigInt&);
	BigInt operator-=(const BigInt&);
	BigInt operator*=(const BigInt&);
	BigInt operator/=(const BigInt&);
	BigInt operator%=(const BigInt&);

	BigInt operator++(int);

	bool operator==(const BigInt&);
	bool operator!=(const BigInt&);
	bool operator>(const BigInt&);
	bool operator<(const BigInt&);

	BigDecimal pow(BigDecimal);

	vector<short> Getval();
	string Getvalreal();
	long long int Getint();
	bool Getsign();
	void Setsign(bool);
private:
	vector<short> val;

	bool checkbig(const BigInt&, const BigInt&, bool isunsigned = false);//true = 前  false = 後
	short compair(const BigInt&, const BigInt&);//1 = 前 0 = 等於 -1 = 後
	void erasezero(BigInt&);
	string stringprettify(string);
	bool sign;//0 = 負號  1 = 正號
};