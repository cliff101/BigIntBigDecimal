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
	//BigDecimal��BigInt�A���氣�k
	BigInt(const BigDecimal&);
	BigInt(const char*);

	BigInt operator=(string);
	BigInt operator=(int);
	//BigDecimal��BigInt�A���氣�k
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
	//�w�qBigInt���h��Ƶ��c val �� vector<short>
	vector<short> val;
	//����inf���A
	bool isinf;
	//����undefined���A
	bool isundefined;

	//�ˬd�O�_�j��
	bool checkbig(const BigInt&, const BigInt&, bool isunsigned = false) const;//true = �e  false = ��
	//����j�p
	short compair(const BigInt&, const BigInt&) const;//1 = �e 0 = ���� -1 = ��
	//����BigInt
	void erasezero(BigInt&) const;
	//���Ʈ榡����ƪ�string
	string stringprettify(string);
	//�x�s���t�����A
	bool sign;//0 = �t��  1 = ����
protected:
	//��API��Xoperator<<��
	void Print(ostream& ccout) const;
};