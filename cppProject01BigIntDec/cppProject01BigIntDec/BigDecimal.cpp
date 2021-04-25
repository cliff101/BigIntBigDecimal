#include "BigDecimal.h"

unsigned int BigDecimal::precision = 100;

BigDecimal::BigDecimal() {
	valup = BigInt();
	valdown = BigInt();
	isinf = false;
	isundefined = false;
}
BigDecimal::BigDecimal(double ind) {
	string in = stringprettify(to_string(ind));
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
	decreduce();
	isinf = false;
	isundefined = false;
}
BigDecimal::BigDecimal(string in) {
	in = stringprettify(in);
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
	decreduce();
	isinf = false;
	isundefined = false;
}
BigDecimal::BigDecimal(const BigDecimal& in) {
	valup = BigInt(in.valup);
	valdown = BigInt(in.valdown);
	isinf = in.isinf;
	isundefined = in.isundefined;
}
BigDecimal::BigDecimal(const BigInt& in) {
	valup = in;
	valdown = 1;
	isinf = valup.Getinf();
	isundefined = valup.Getundefined();
}
BigDecimal::BigDecimal(const BigInt& in1, const BigInt& in2) {
	valup = in1;
	valdown = in2;
	isinf = false;
	isundefined = valup.Getundefined() || valdown.Getundefined() || valup.Getinf() && valdown.Getinf() || valup.Getnumlength()==1 && valup==0 && valdown.Getinf();
	decreduce();
}


BigDecimal BigDecimal::operator=(double ind) {
	string in = stringprettify(to_string(ind));
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
	decreduce();
	return *this;
}
BigDecimal BigDecimal::operator=(string in) {
	in = stringprettify(in);
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
	decreduce();
	return *this;
}
BigDecimal BigDecimal::operator=(const BigInt& in) {
	valup = in;
	valdown = 1;
	return *this;
}

BigDecimal BigDecimal::operator+(const BigDecimal& in) {
	BigDecimal cp(in),out;
	if (isundefined || in.isundefined || isinf && in.isinf && (!valup.Getsign() && cp.valup.Getsign() || valup.Getsign() && !cp.valup.Getsign())) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && valup.Getsign() || in.isinf && cp.valup.Getsign()) {
		out.valup.Setsign(true);
		out.isinf = true;
		return out;
	}
	else if (isinf && !valup.Getsign() || in.isinf && !cp.valup.Getsign()) {
		out.valup.Setsign(false);
		out.isinf = true;
		return out;
	}
	BigInt gcdvaldown(gcd(valdown, in.valdown));
	out.valup = valup * (cp.valdown / gcdvaldown) + cp.valup * (valdown / gcdvaldown);
	out.valdown = valdown * (cp.valdown / gcdvaldown);
	out.decreduce();
	return out;
}

vector<short> BigDecimal::Getvalup() {
	return valup.Getval();
}
vector<short> BigDecimal::Getvaldown() {
	return valdown.Getval();
}
BigInt BigDecimal::GetBigIntup() {
	return valup;
}
BigInt BigDecimal::GetBigIntdown() {
	return valdown;
}
bool BigDecimal::Getinf() {
	return isinf;
}
bool BigDecimal::Getundefined() {
	return isundefined;
}

string BigDecimal::Getvalreal() {
	if (isundefined) {
		return "nan";
	}
	string result = "";
	if (isinf) {
		if (!valup.Getsign()) {
			result += "-";
		}
		result += "inf";
		return result;
	}
	vector<short> upreal, downreal;
	upreal = valup.Getval();
	unsigned long long int uprealsize = upreal.size();
	for (int i = 0; i < precision; i++) {
		upreal.push_back(0);
	}
	downreal = valdown.Getval();
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
	if (!valup.Getsign()) {
		result = "-" + result;
	}
	result = stringprettify(result);
	return result;
}


//BigDecimal BigDecimal::Power(BigDecimal in) {
//	BigInt out = *this;
//	return BigDecimal(out);
//}

string BigDecimal::stringprettify(string in) {
	string sign = "";
	if (in[0] == '+' || in[0] == '-') {
		sign = in[0];
		in.erase(in.begin());
	}
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
	if (in[in.size()-1] == '.') {
		in.push_back('0');
	}
	in = sign + in;
	return in;
}
BigInt BigDecimal::gcd(BigInt a, BigInt b) {
	BigInt temp;
	a.Setsign(true);
	b.Setsign(true);
	while (a % b != 0) {
		temp = a % b;
		a = b;
		b = temp;
	}
	return b;
}
void BigDecimal::decreduce() {
	BigInt gcdval = gcd(valup, valdown);
	valup /= gcdval;
	valdown /= gcdval;
}

