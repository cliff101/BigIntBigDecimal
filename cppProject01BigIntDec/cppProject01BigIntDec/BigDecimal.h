#pragma once
#include <vector>
#include <string>
#include "BigInt.h"

using namespace std;


class BigDecimal {
public:
	BigDecimal();
	BigDecimal(double);
	BigDecimal(string);
	BigDecimal(const BigDecimal&);
	BigDecimal(const BigInt&);
	BigDecimal(const BigInt&, const BigInt&);

	BigDecimal operator=(string);
	BigDecimal operator=(double);
	BigDecimal operator=(const BigInt&);

	BigDecimal operator+(const BigDecimal&);
	BigDecimal operator-(const BigDecimal&);
	BigDecimal operator*(const BigDecimal&);
	BigDecimal operator/(const BigDecimal&);

	vector<short> Getvalup();
	vector<short> Getvaldown();
	BigInt GetBigIntup();
	BigInt GetBigIntdown();
	string Getvalreal();
	bool Getinf();
	bool Getundefined();

	BigDecimal Power(BigDecimal);

	static unsigned int precision;

private:
	BigInt valup, valdown;
	bool isinf;
	bool isundefined;

	bool checkbig(const BigDecimal&, const BigDecimal&, bool isunsigned = false);//true = лe  false = лс
	void erasezero(BigDecimal&);
	string stringprettify(string);
	BigInt gcd(BigInt, BigInt);
	void decreduce();
};