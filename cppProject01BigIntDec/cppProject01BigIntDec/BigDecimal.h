#pragma once
#include <vector>
#include <string>
#include "BigInt.h"

using namespace std;

class BigDecimal {
public:
	BigDecimal();
	BigDecimal(string);
	BigDecimal(const BigDecimal&);

	//BigDecimal operator=(BigDecimal);
	//BigDecimal operator=(BigDecimal);

	BigDecimal operator+(const BigDecimal&);
	BigDecimal operator-(const BigDecimal&);
	BigDecimal operator*(const BigDecimal&);
	BigDecimal operator/(const BigDecimal&);

	vector<int> Getvalup();
	vector<int> Getvaldown();
	BigInt GetBigIntup();
	BigInt GetBigIntdown();
	string Getvalreal(int precision=100);
private:
	bool checkbig(const BigDecimal&, const BigDecimal&, bool isunsigned = false);//true = лe  false = лс
	void erasezero(BigDecimal&);
	BigInt gcd(BigInt, BigInt);
	BigInt valup,valdown;
};