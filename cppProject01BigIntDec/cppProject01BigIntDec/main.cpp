#include <iostream>
#include "BigInt.h"
#include "BigDecimal.h"
#include "CommandParser.h"
#include <cmath>
#include <string>
#include "BigValue.h"

using namespace std;

//1. Decimal -*/^ << >>
//2. BigInt << >>
//3. CommandParser debug

int main() {
	BigInt aa("13317+5"), bb("5"),dd(13322.12345),ee(5);
	//BigInt cc;
	//BigDecimal ff("172361.123");
	//BigValue f;
	//cc = dd * ee;
	//BigDecimal::precision = 100;
	//for (int i = 0; i < 1; i++) {
	//	ff = aa.pow(1.5);
	//	cout << ff.Getvalreal() << endl;
	//}

	//cc = aa-bb;
	//cc = cc + ff;
	//cc = "100!";
	cout << aa.Getvalreal() << endl;
	//cout << bb.Getvalreal() << endl;
	//cout << cc.Getvalreal() << endl;
	//d = (c != 0);
	//a %= 15;
	//c = a % b;
	//BigDecimal a(BigInt(0)/BigInt(0)), b("-1.45435345"), c;
	//BigDecimal d(1.234);
	//d = a;
	//a = 5;
	//cout << a.Getvalreal() << endl;
	//cout << b.Getvalreal() << endl;
	//c = a + b;
	//c = "123";
	//cout << a.Getvalreal() << endl;
	//cout << b.Getvalreal() << endl;
	//cout << c.Getvalreal() << endl;

	string cmd;
	CommandParser cmdps;


	while (true) {
		cout << ">>> ";
		getline(cin, cmd);
		//
		//cmdps.Inputcommand("15 * 20");
		cmdps.Inputcommand(cmd);
		//cin >> cmd;
	}
	return 0;
}