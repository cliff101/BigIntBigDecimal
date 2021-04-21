#pragma once
#include <vector>
#include <string>

using namespace std;

class BigInt {
public:
	BigInt();
	BigInt(int);
	BigInt(string);
	BigInt(const BigInt&);
	
	BigInt operator=(string);
	BigInt operator=(int);

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

	bool operator==(const BigInt&);
	bool operator!=(const BigInt&);

	vector<int> Getval();
	bool Getsign();
private:
	bool checkbig(const BigInt&, const BigInt&,bool isunsigned = false);//true = 前  false = 後
	void erasezero(BigInt&);
	vector<int> val;
	bool sign;//0 = 負號  1 = 正號
};