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
	
	/*BigInt x;
	BigDecimal y;

	cin >> x; // 輸入 123456789
	cin >> y; // 輸入 3.1415926

	cout << x + y << endl;
	cout << x - y << endl;
	cout << x * y << endl;
	cout << x / y << endl;
	
	BigInt x = "123 * 8 + 456";
	BigDecimal y = "-1.0 / 3 - 45 / 13.0";

	vector<BigValue*> nums;
	nums.push_back(&x);
	nums.push_back(&y);
	for (const auto& num : nums)
		cout << *num << endl;
	*/
	/*
	vector<BigValue*> nums;
	nums.push_back(&a);
	nums.push_back(&aa);
	for (const auto& num : nums)
		cout << *num << endl;
	*/

	string cmd;
	CommandParser cmdps;

	//BigDecimal::precision = 100;

	while (true) {
		cout << "( ·▽·)∕ ";
		getline(cin, cmd);
		//
		//cmdps.Inputcommand("15 * 20");
		cmdps.Inputcommand(cmd);
		//cin >> cmd;
	}
	return 0;
}