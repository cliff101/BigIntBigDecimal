#pragma once
#include <string>
#include <iostream>

using namespace std;

class BigValue {
public:
	friend ostream& operator<<(ostream& ccout,const BigValue& in) {
		in.Print(ccout);
		return ccout;
	}
protected:
	virtual void Print(ostream& ccout) const {};
};