#pragma once
#include <vector>
#include <string>
#include "BigInt.h"
#include "BigValue.h"

using namespace std;

class CommandParser;

class BigDecimal :public BigValue {
public:
	BigDecimal();
	BigDecimal(double);
	BigDecimal(int);
	BigDecimal(string);
	BigDecimal(const BigDecimal&);
	BigDecimal(const BigInt&);
	//BigInt(���l)/BigInt(����)
	BigDecimal(const BigInt&, const BigInt&);
	BigDecimal(const char*);

	BigDecimal operator=(string);
	BigDecimal operator=(double);
	BigDecimal operator=(const BigInt&);

	bool operator==(BigDecimal&);

	BigDecimal operator+(const BigDecimal&);
	friend BigDecimal operator+(BigInt&, BigDecimal&);
	friend BigDecimal operator+(const BigInt&, const BigDecimal&);
	friend BigDecimal operator+(BigInt&, const BigDecimal&);

	BigDecimal operator-(const BigDecimal&);
	friend BigDecimal operator-(BigInt&, BigDecimal&);
	friend BigDecimal operator-(const BigInt&, const BigDecimal&);
	friend BigDecimal operator-(BigInt&, const BigDecimal&);

	BigDecimal operator*(const BigDecimal&);
	friend BigDecimal operator*(BigInt&, BigDecimal&);
	friend BigDecimal operator*(const BigInt&, const BigDecimal&);
	friend BigDecimal operator*(BigInt&, const BigDecimal&);

	BigDecimal operator/(const BigDecimal&);
	friend BigDecimal operator/(BigInt&, BigDecimal&);
	friend BigDecimal operator/(const BigInt&, const BigDecimal&);
	friend BigDecimal operator/(BigInt&, const BigDecimal&);

	friend ostream& operator<<(ostream& ccout, const BigDecimal& in);
	friend istream& operator>>(istream& ccin, BigDecimal& in);

	//���o���l�����h��
	vector<short> Getvalup();
	//���o���������h��
	vector<short> Getvaldown();
	//���o���l��BigInt
	BigInt GetBigIntup();
	//���o������BigInt
	BigInt GetBigIntdown();
	//���ostring��X��
	string Getvalreal();
	//���o�O�_��infinity
	bool Getinf();
	//���o�O�_��undefined
	bool Getundefined();

	BigDecimal Power(BigDecimal&);
	BigDecimal Power(BigInt&);

	//��סA�i�H�ɧ��
	static unsigned int precision;

	//²�Ƥ��l�����A������l�����ƭȤӤj
	void Simplify();

	//���o���l�ƭȪ���
	int Getvaluplength();
	//���o�����ƭȪ���
	int Getvaldownlength();

private:
	BigInt valup = 0,//�w�qvalup�����l�A��ƫ��A��BigInt
		valdown = 1;//valdown�������A��ƫ��A��BigInt
	//�w�qinf���A
	bool isinf;
	//�w�qundefined���A
	bool isundefined;

	//�j�p���
	bool checkbig(const BigDecimal&, const BigDecimal&, bool isunsigned = false);//true = �e  false = ��
	//����BigDecimal
	void erasezero(BigDecimal&);
	//���Ƥp���I�榡��string
	string stringprettify(string);
	//gcd�C
	BigInt gcd(BigInt, BigInt);
	//�������l����
	void decreduce();
protected:
	//��API��Xoperator<<��
	void Print(ostream& ccout)  const;
};