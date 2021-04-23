#include "BigDecimal.h"

BigDecimal::BigDecimal() {
	valup = BigInt();
	valdown = BigInt();
}
BigDecimal::BigDecimal(double ind) {
	string in = inputprettify(to_string(ind));
	bool sign = (in[0] != '-');
	if (!sign) {
		in.erase(in.begin());
	}
	else if (in[0] == '+') {
		in.erase(in.begin());
	}
	string down = "1";
	bool find = false;
	for (unsigned long long int i = 1; i < in.size(); i++) {
		if (find) {
			down += "0";
		}
		else if (in[i] == '.') {
			in.erase(in.begin() + i);
			find = true;
			i--;
		}
	}
	valup = BigInt(in);
	valdown = BigInt(down);
	BigInt gcdval = gcd(valup, valdown);
	valup /= gcdval;
	valdown /= gcdval;
	if (!sign) {
		valup *= -1;
	}
}
BigDecimal::BigDecimal(string in) {
	bool sign = (in[0] != '-');
	if (!sign) {
		in.erase(in.begin());
	}
	else if (in[0] == '+') {
		in.erase(in.begin());
	}
	string down = "1";
	bool find = false;
	for (unsigned long long int i = 1; i < in.size(); i++) {
		if (find) {
			down += "0";
		}
		else if (in[i] == '.') {
			in.erase(in.begin() + i);
			find = true;
			i--;
		}
	}
	valup = BigInt(in);
	valdown = BigInt(down);
	BigInt gcdval = gcd(valup, valdown);
	valup /= gcdval;
	valdown /= gcdval;
	if (!sign) {
		valup *= -1;
	}
}
BigDecimal::BigDecimal(const BigDecimal& in) {
	valup = BigInt(in.valup);
	valdown = BigInt(in.valdown);
}
BigDecimal::BigDecimal(const BigInt& in) {
	valup = in;
	valdown = 1;
}
BigDecimal::BigDecimal(const BigInt& in1, const BigInt& in2) {
	valup = in1;
	valdown = in2;
}


BigDecimal BigDecimal::operator=(double in) {
	*this = inputprettify(to_string(in));
	return *this;
}
BigDecimal BigDecimal::operator=(string in) {
	bool sign = (in[0] != '-');
	if (!sign) {
		in.erase(in.begin());
	}
	else if (in[0] == '+') {
		in.erase(in.begin());
	}
	string down = "1";
	bool find = false;
	for (unsigned long long int i = 1; i < in.size(); i++) {
		if (find) {
			down += "0";
		}
		else if (in[i] == '.') {
			in.erase(in.begin() + i);
			find = true;
			i--;
		}
	}
	valup = BigInt(in);
	valdown = BigInt(down);
	BigInt gcdval = gcd(valup, valdown);
	valup /= gcdval;
	valdown /= gcdval;
	if (!sign) {
		valup *= -1;
	}
	return *this;
}
BigDecimal BigDecimal::operator=(const BigInt& in) {
	valup = in;
	valdown = 1;
	return *this;
}


BigDecimal BigDecimal::operator+(const BigDecimal& in) {
	BigDecimal cp(in), out;
	BigInt gcdvaldown(gcd(valdown, in.valdown));
	out.valup = valup * (cp.valdown / gcdvaldown) + cp.valup * (valdown / gcdvaldown);
	out.valdown = valdown * (cp.valdown / gcdvaldown);
	*this = BigDecimal(out);
	return *this;
}

vector<short> BigDecimal::Getvalup() {
	return valup.Getval();
}
vector<short> BigDecimal::Getvaldown() {
	return valdown.Getval();
}
string BigDecimal::inputprettify(string in) {
	while (in.length() > 0 && in[0] == '0') {
		in.erase(in.begin());
	}
	if (in[0] == '.') {
		in = '0' + in;
	}
	while (in.length() > 0 && in[in.length() - 1] == '0') {
		in.pop_back();
	}
	if (in.length() == 0) {
		in.push_back('0');
	}
	if (in[0] == '.') {
		in.pop_back();
	}
	return in;
}
BigInt BigDecimal::GetBigIntup() {
	return valup;
}
BigInt BigDecimal::GetBigIntdown() {
	return valdown;
}
BigInt BigDecimal::gcd(BigInt a, BigInt b) {
	BigInt temp;
	while (a % b != 0) {
		temp = a % b;
		a = b;
		b = temp;
	}
	return b;
}

string BigDecimal::Getvalreal(int precision) {
	vector<short> upreal, downreal;
	upreal = valup.Getval();
	unsigned long long int uprealsize = upreal.size();
	for (int i = 0; i < precision; i++) {
		upreal.push_back(0);
	}
	downreal = valdown.Getval();
	string result = "";
	bool find = false;
	for (long long int i = 0; i < uprealsize - downreal.size() + 1 + precision; i++) {
		bool ok = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < downreal.size(); j++) {
				if (upreal[i + j] - downreal[j] < 0) {
					ok = false;
				}
				else if (upreal[i + j] - downreal[j] > 0) {
					break;
				}
			}
			if (!ok) {
				break;
			}
			for (int j = 0; j < downreal.size(); j++) {
				upreal[i + j] -= downreal[j];
				if (upreal[i + j] < 0) {
					upreal[i + j] += 10;
					upreal[i + j - 1]--;
				}
			}
			count++;
			find = true;
		}
		if (i < uprealsize - downreal.size() + precision) {
			upreal[i + 1] = upreal[i + 1] + upreal[i] * 10;
			upreal[i] = 0;
		}
		if (find) {
			result += count + '0';
		}

	}
	while (result.length() < precision) {
		result = "0" + result;
	}
	int decimalloc = result.length() - precision;
	result.insert(result.begin() + decimalloc, '.');
	if (decimalloc == 0) {
		result = "0" + result;
	}
	while (result[result.length() - 1] == '0' || result[result.length() - 1] == '.') {
		result.pop_back();
	}
	if (!valup.Getsign()) {
		result = "-" + result;
	}
	return result;
}