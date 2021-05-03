#include "BigDecimal.h"
#include "CommandParser.h"

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
BigDecimal::BigDecimal(int ind) {
	*this = double(ind);
	isinf = false;
	isundefined = false;
}
BigDecimal::BigDecimal(string in) {
	CommandParser temp;
	if (temp.IsCommand(in)) {
		*this = temp.Getcommandvalue(in, *this);
	}
	else {
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
}
BigDecimal::BigDecimal(const char* in) {
	*this = string(in);
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
	bool downsign = valdown.Getsign();
	valdown.Setsign(true);
	valup.Setsign(!(valup.Getsign() ^ downsign));
	isinf = valdown.Getnumlength() == 1 && valdown == 0 && valup != 0;
	isundefined = valup.Getundefined() || valdown.Getundefined() || valup.Getinf() && valdown.Getinf() || valdown.Getnumlength() == 1 && valdown == 0 && valup == 0;
	decreduce();
}


BigDecimal BigDecimal::operator=(double ind) {
	string in = stringprettify(to_string(ind));
	string down = "1";
	bool find = false;
	for (unsigned long long int i = 1; i < in.size(); i++) {
		if (find) {//find 到小數點了，分母開始補0
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
	CommandParser temp;
	if (temp.IsCommand(in)) {
		*this = temp.Getcommandvalue(in, *this);
	}
	else {
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
	}
	return *this;
}
BigDecimal BigDecimal::operator=(const BigInt& in) {
	valup = in;
	valdown = 1;
	return *this;
}

bool BigDecimal::operator==(BigDecimal& in) {
	if (isundefined && in.isundefined) {
		return true;
	}
	else if (isundefined || in.isundefined) {
		return false;
	}
	else if (isinf && in.isinf) {
		return valup.Getsign() == valdown.Getsign();
	}
	else if (isinf || in.isinf) {
		return false;
	}
	else {
		return valup == in.valup && valdown == in.valdown;
	}
}

BigDecimal BigDecimal::operator+(const BigDecimal& in) {
	BigDecimal cp(in), out;
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
	out.valup = valup * (cp.valdown / gcdvaldown) + cp.valup * (valdown / gcdvaldown);//分數加法
	out.valdown = valdown * (cp.valdown / gcdvaldown);
	out.decreduce();
	return out;
}
BigDecimal operator+(BigInt& in1, BigDecimal& in2) {
	return BigDecimal(in1) + in2;
}
BigDecimal operator+(const BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) + in2;
}
BigDecimal operator+(BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) + in2;
}

BigDecimal BigDecimal::operator-(const BigDecimal& in) {
	BigDecimal cp(in), out;
	if (isundefined || in.isundefined || isinf && !in.isinf && (!valup.Getsign() && !cp.valup.Getsign() || valup.Getsign() && cp.valup.Getsign())) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && valup.Getsign() || in.isinf && !cp.valup.Getsign()) {
		out.valup.Setsign(true);
		out.isinf = true;
		return out;
	}
	else if (isinf && !valup.Getsign() || in.isinf && cp.valup.Getsign()) {
		out.valup.Setsign(false);
		out.isinf = true;
		return out;
	}
	BigInt gcdvaldown(gcd(valdown, in.valdown));
	out.valup = valup * (cp.valdown / gcdvaldown) - cp.valup * (valdown / gcdvaldown);//分數減法
	out.valdown = valdown * (cp.valdown / gcdvaldown);
	return out;
}
BigDecimal operator-(BigInt& in1, BigDecimal& in2) {
	return BigDecimal(in1) - in2;
}
BigDecimal operator-(const BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) + in2;
}
BigDecimal operator-(BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) + in2;
}

BigDecimal BigDecimal::operator*(const BigDecimal& in) {
	BigDecimal cp(in), out;
	if (isundefined || in.isundefined) {
		out.isundefined = true;
		return out;
	}
	else if ((isinf || in.isinf) && (valup.Getsign() && cp.valup.Getsign() || !valup.Getsign() && !cp.valup.Getsign())) {
		out.valup.Setsign(true);
		out.isinf = true;
		return out;
	}
	else if (isinf || in.isinf) {
		out.valup.Setsign(false);
		out.isinf = true;
		return out;
	}
	BigInt gcdvalld(gcd(valdown, in.valup)),//left down
		gcdvallu(gcd(valup, in.valdown));//left up
	out.valup = (valup / gcdvallu) * (in.valup / gcdvalld);//分數乘法
	out.valdown = (valdown / gcdvalld) * (in.valdown / gcdvallu);
	return out;
}
BigDecimal operator*(BigInt& in1, BigDecimal& in2) {
	return BigDecimal(in1) * in2;
}
BigDecimal operator*(const BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) * in2;
}
BigDecimal operator*(BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) * in2;
}

BigDecimal BigDecimal::operator/(const BigDecimal& in) {
	BigDecimal cp(in), out;
	unsigned long long int thisvalupsize = valup.Getval().size(),
		invalupsize = cp.valup.Getval().size();
	if (isundefined || in.isundefined || isinf && in.isinf || (!in.isinf && !isinf || isinf) && invalupsize == 1 && in.valup == 0 && thisvalupsize == 1 && valup == 0) {
		out.isundefined = true;
		return out;
	}
	else if (!isinf && thisvalupsize == 1 && valup == 0 || in.isinf) {
		out = 0;
		return out;
	}
	else if (isinf && cp.valup.Getsign() && valup.Getsign() || valup.Getsign() && invalupsize == 1 && in.valup == 0) {
		out.valup.Setsign(true);
		out.isinf = true;
		return out;
	}
	else if (isinf || !valup.Getsign() && invalupsize == 1 && in.valup == 0) {
		out.valup.Setsign(false);
		out.isinf = true;
		return out;
	}
	else if (in.isinf) {
		out = 0;
		return out;
	}
	BigInt gcdvalld(gcd(valdown, in.valdown)),//left down
		gcdvallu(gcd(valup, in.valup));//left up
	out.valup = (valup / gcdvallu) * (in.valdown / gcdvalld);//分數除法
	out.valdown = (valdown / gcdvalld) * (in.valup / gcdvallu);
	return out;
}
BigDecimal operator/(BigInt& in1, BigDecimal& in2) {
	return BigDecimal(in1) / in2;
}
BigDecimal operator/(const BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) / in2;
}
BigDecimal operator/(BigInt& in1, const  BigDecimal& in2) {
	return BigDecimal(in1) / in2;
}

BigDecimal BigDecimal::Power(BigDecimal& in) {
	BigDecimal out = *this;
	if (isundefined || in.isundefined || isinf && in.valup.Getval().size() == 1 && in.valup == 0 || isinf && !valup.Getsign()) {
		out.isundefined = true;
		return out;
	}
	else if (in.isinf && !valup.Getsign()) {
		out = 0;
		return out;
	}
	else if (isinf || in.isinf) {
		out.isinf = true;
		return out;
	}
	BigInt up = in.GetBigIntup();
	BigInt down = in.GetBigIntdown();
	bool upsign = up.Getsign();
	up.Setsign(true);
	if (up > 1) {//up>1，乘up次
		BigDecimal cp = out;
		for (BigInt i = 0; i < up - BigInt(1); i += 1) {
			out = out * cp;
			if (down > 2) {//如果down>2，可能需要用牛頓法來猜，為避免不必要數值精度過高，進行簡化
				out.Simplify();
			}
		}
	}
	else if (up == 0) {
		out = 1;
	}
	if (down > 1) {
		BigDecimal temp = BigDecimal(1, down);
		BigDecimal newvalup = out.valup.Power(temp),//對分子分母分別power
			newvaldown = out.valdown.Power(temp);

		BigInt gcdvalld(gcd(valdown, in.valdown)),//left down
			gcdvallu(gcd(valup, in.valup));//left up
		out.valup = newvalup.valup * newvaldown.valdown;
		out.valdown = newvaldown.valup * newvalup.valdown;
	}
	if (!upsign) {
		out = BigDecimal(out.GetBigIntdown(), out.GetBigIntup());
	}
	return out;
}
BigDecimal BigDecimal::Power(BigInt& in) {
	BigDecimal temp(in);
	return Power(temp);//call BigDecimal::Power(BigDecimal& in)
}

ostream& operator<<(ostream& ccout, const BigDecimal& in)
{
	ccout << BigDecimal(in).Getvalreal();
	return ccout;
}
istream& operator>>(istream& ccin, BigDecimal& in)
{
	string temp;
	ccin >> temp;
	in = temp;
	return ccin;
}

void BigDecimal::Print(ostream& ccout)const {
	ccout << BigDecimal(*this).Getvalreal();
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
	if (valdown == 0) {
		isinf = true;
	}
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
		bool ok = true,
			last = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < downreal.size(); j++) {
				if (upreal[i + j] - downreal[j] < 0) {
					ok = false;
					last = false;
				}
				else if (upreal[i + j] - downreal[j] > 0) {
					last = false;
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
			if (last) {
				break;
			}
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
	return result;
}

int BigDecimal::Getvaluplength() {
	return valup.Getvallength();
}
int BigDecimal::Getvaldownlength() {
	return valdown.Getvallength();
}

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
	if (in[in.size() - 1] == '.') {
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
	if (b != 0) {
		return b;
	}
	return 1;
}
void BigDecimal::decreduce() {
	if (valup == 0) {//分母為0，直接指定分子為1
		valdown = 1;
	}
	else {
		BigInt gcdval = gcd(valup, valdown);//取的gcd value
		valup /= gcdval;//約分
		valdown /= gcdval;//約分
	}
}
void BigDecimal::Simplify() {
	//int intlength = Getvaluplength() - Getvaldownlength();
	//Getvaluplength() - intlength > precision * 2 || Getvaldownlength() - intlength > precision * 2
	if (Getvaldownlength() > precision * 2 || Getvaldownlength() * 2 - Getvaluplength() > precision * 2) {
		*this = BigDecimal(Getvalreal());//簡化
	}
}

