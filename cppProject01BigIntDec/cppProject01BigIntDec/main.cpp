#include <iostream>
#include "BigInt.h"
#include "BigDecimal.h"

using namespace std;

int main() {
	//BigInt a("100"), b("10"),c;
	//a %= 15;
	//c = a % b;
	BigDecimal a("+1.2425345"), b("-1.45435345"),c;
	//cout << a.Getvalreal(100) << endl;
	//cout << b.Getvalreal(100) << endl;
	c = a + b;
	cout<<c.Getvalreal(100);
	
	return 0;
}

