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

	BigDecimal pow(BigDecimal, int precision = 100);

	vector<short> Getval();
	string Getvalreal();
	long long int Getint();
	bool Getsign();
private:
	vector<short> val;

	bool checkbig(const BigInt&, const BigInt&, bool isunsigned = false);//true = �e  false = ��
	short compair(const BigInt&, const BigInt&);//1 = �e 0 = ���� -1 = ��
	void erasezero(BigInt&);
	string inputprettify(string);
	bool sign;//0 = �t��  1 = ����
};