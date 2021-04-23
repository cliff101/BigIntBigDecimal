#include <iostream>
#include "BigInt.h"
#include "BigDecimal.h"
#include "CommandParser.h"
#include <cmath>

using namespace std;

int main() {
	//BigInt aa("13322"), bb("5"),cc,dd(177475684),ee(13322);
	//BigDecimal ff;
	//cc = dd * ee;
	//ff = aa.pow(1.5);
	//cout << ff.Getvalreal();
	//cc = aa-bb;
	//cout << cc.Getvalreal() << endl;
	//d = (c != 0);
	//a %= 15;
	//c = a % b;
	//BigDecimal a("+1.2425345"), b("-1.45435345"),c;
	//BigDecimal d(1.234);
	//d = a;
	//a = 5;
	//cout << a.Getvalreal(100) << endl;
	//cout << b.Getvalreal(100) << endl;
	//c = a + b;
	//c = "123";
	//cout<<c.Getvalreal();
	
	CommandParser cmdps;
	cmdps.Inputcommand("15 + 20");

	return 0;
}

