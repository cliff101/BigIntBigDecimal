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
	//BigInt(分子)/BigInt(分母)
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

	//取得分子的底層值
	vector<short> Getvalup();
	//取得分母的底層值
	vector<short> Getvaldown();
	//取得分子的BigInt
	BigInt GetBigIntup();
	//取得分母的BigInt
	BigInt GetBigIntdown();
	//取得string輸出值
	string Getvalreal();
	//取得是否為infinity
	bool Getinf();
	//取得是否為undefined
	bool Getundefined();

	BigDecimal Power(BigDecimal&);
	BigDecimal Power(BigInt&);

	//精度，可隨時更動
	static unsigned int precision;

	//簡化分子分母，防止分子分母數值太大
	void Simplify();

	//取得分子數值長度
	int Getvaluplength();
	//取得分母數值長度
	int Getvaldownlength();

private:
	BigInt valup = 0,//定義valup為分子，資料型態為BigInt
		valdown = 1;//valdown為分母，資料型態為BigInt
	//定義inf狀態
	bool isinf;
	//定義undefined狀態
	bool isundefined;

	//大小比較
	bool checkbig(const BigDecimal&, const BigDecimal&, bool isunsigned = false);//true = 前  false = 後
	//美化BigDecimal
	void erasezero(BigDecimal&);
	//美化小數點格式的string
	string stringprettify(string);
	//gcd。
	BigInt gcd(BigInt, BigInt);
	//約分分子分母
	void decreduce();
protected:
	//給API整合operator<<用
	void Print(ostream& ccout)  const;
};