#include "BigInt.h"

BigInt::BigInt() {
	val = vector<int>(1,0);
	sign = true;
}
BigInt::BigInt(string in) {
	sign = (in[0] != '-');
	if (!sign) {
		in.erase(in.begin());
	}
	else if (in[0] == '+') {
		in.erase(in.begin());
	}
	val = vector<int>(in.length());
	for (unsigned long long int i = 0; i < in.length(); i++) {
		val[i] = in[i] - '0';
	}
	erasezero(*this);
}
BigInt::BigInt(const BigInt& in) {
	val = vector<int>(in.val);
	sign = in.sign;
}
BigInt::BigInt(int in) {
	*this = in;
}

BigInt BigInt::operator=(string in) {
	return BigInt(in);
}
BigInt BigInt::operator=(int in) {
	sign = in >= 0;
	in = abs(in);
	val = vector<int>(0);
	string temp = to_string(in);
	for (long long int i = 0; i < temp.size(); i++) {
		val.push_back(temp[i] - '0');
	}
	return *this;
}

BigInt BigInt::operator+(const BigInt& in) {
	BigInt out;
	vector<int> result(0);
	if (!sign && in.sign) {
		BigInt temp(*this);
		BigInt temp2(in);
		temp.sign = true;
		out = temp2 - temp;
		return out;
	}
	else if (sign && !in.sign) {
		BigInt temp(in);
		temp.sign = true;
		out = *this - temp;
		return out;
	}
	long long int i = static_cast<long long>(val.size()) - 1, j = static_cast<long long>(in.val.size()) - 1;
	int hold = 0;
	while (i >= 0 || j >= 0) {
		result.insert(result.begin(), (i >= 0 ? val[i] : 0) + (j >= 0 ? in.val[j] : 0) + hold);
		if (result[0] > 9) {
			hold = result[0] / 10;
			result[0] -= hold * 10;
		}
		if (i >= 0) {
			i--;
		}
		if (j >= 0) {
			j--;
		}
	}
	if (hold > 0) {
		result.insert(result.begin(), hold);
	}
	out.sign = sign;
	out.val = result;
	return out;
}
BigInt BigInt::operator-(const BigInt& in) {
	BigInt out;
	vector<int> result(0);
	if (!sign && in.sign) {
		BigInt temp(*this);
		temp.sign = true;
		out = temp + in;
		out.sign = false;
		return out;
	}
	else if (sign && !in.sign) {
		BigInt temp(in);
		temp.sign = true;
		out = temp + *this;
		out.sign = true;
		return out;
	}
	out.sign = sign;
	int borrow = 0;
	BigInt temp1 = *this, temp2 = in;
	if (!checkbig(*this, in, true)) {
		temp1 = in;
		temp2 = *this;
		out.sign = !sign;
	}
	long long int i = static_cast<long long>(temp1.val.size()) - 1, j = static_cast<long long>(temp2.val.size()) - 1;
	while (i >= 0 || j >= 0) {
		result.insert(result.begin(), (i >= 0 ? temp1.val[i] : 0) - (j >= 0 ? temp2.val[j] : 0) - borrow);
		borrow = 0;
		if (result[0] < 0) {
			borrow = (-result[0]) / 10 + ((-result[0]) % 10 != 0);
			result[0] += borrow * 10;
		}
		if (i >= 0) {
			i--;
		}
		if (j >= 0) {
			j--;
		}
	}
	if (borrow > 0) {
		out.sign = !out.sign;
		result.insert(result.begin(), 10 - borrow);
	}
	out.val = result;
	erasezero(out);
	return out;
}
BigInt BigInt::operator*(const BigInt& in) {
	BigInt out;
	vector<int> res(val.size() + in.val.size());
	int hold = 0;
	for (long long int i = static_cast<unsigned long long>(val.size()) - 1; i >= 0; i--) {
		for (long long int j = static_cast<unsigned long long>(in.val.size()) - 1; j >= 0; j--) {
			res[i + j + 1] += val[i] * in.val[j] + hold;
			hold = res[i + j + 1] / 10;
			res[i + j + 1] %= 10;
		}
	}
	res[0] = hold;
	if (hold == 0) {
		res.erase(res.begin());
	}
	out.val = res;
	out.sign = (sign && in.sign || !sign && !in.sign);
	erasezero(out);
	return out;
}
BigInt BigInt::operator/(const BigInt& in) {
	BigInt a, out;
	a = *this;
	vector<int> result(0);
	for (long long int i = 0; i < static_cast<long long>(a.val.size()) - in.val.size() + 1; i++) {
		bool ok = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < in.val.size(); j++) {
				if (a.val[i + j] - in.val[j] < 0) {
					ok = false;
				}
				else if (a.val[i + j] - in.val[j] > 0) {
					break;
				}
			}
			if (!ok) {
				break;
			}
			for (int j = 0; j < in.val.size(); j++) {
				a.val[i + j] -= in.val[j];
				if (a.val[i + j] < 0) {
					a.val[i + j] += 10;
					a.val[i + j - 1]--;
				}
			}
			count++;
		}
		if (i < static_cast<long long>(a.val.size()) - in.val.size()) {
			a.val[i + 1] = a.val[i + 1] + a.val[i] * 10;
			a.val[i] = 0;
		}
		result.push_back(count);
	}
	if (result.size() == 0) {
		result.push_back(0);
	}
	out.val = result;
	out.sign = (sign && in.sign || !sign && !in.sign);
	erasezero(out);
	return out;
}
BigInt BigInt::operator%(const BigInt& in) {
	return *this - (BigInt(in) * (*this / in));
}
bool BigInt::operator==(const BigInt& in) {
	if (val != in.val) {
		return false;
	}
	if (sign != sign) {
		return false;
	}
	return true;
}
bool BigInt::operator!=(const BigInt& in) {
	return !(*this == in);
}

BigInt BigInt::operator+=(const BigInt& in) {
	vector<int> result(0);
	if (!sign && in.sign) {
		BigInt temp(*this);
		BigInt temp2(in);
		temp.sign = true;
		*this = temp2 - temp;
		return *this;
	}
	else if (sign && !in.sign) {
		BigInt temp(in);
		temp.sign = true;
		*this = *this - temp;
		return *this;
	}
	long long int i = static_cast<long long>(val.size()) - 1, j = static_cast<long long>(in.val.size()) - 1;
	int hold = 0;
	while (i >= 0 || j >= 0) {
		result.insert(result.begin(), (i >= 0 ? val[i] : 0) + (j >= 0 ? in.val[j] : 0) + hold);
		if (result[0] > 9) {
			hold = result[0] / 10;
			result[0] -= hold * 10;
		}
		if (i >= 0) {
			i--;
		}
		if (j >= 0) {
			j--;
		}
	}
	if (hold > 0) {
		result.insert(result.begin(), hold);
	}
	sign = sign;
	val = result;
	return *this;
}
BigInt BigInt::operator-=(const BigInt& in) {
	vector<int> result(0);
	if (!sign && in.sign) {
		BigInt temp(*this);
		temp.sign = true;
		*this = temp + in;
		sign = false;
		return *this;
	}
	else if (sign && !in.sign) {
		BigInt temp(in);
		temp.sign = true;
		*this = temp + *this;
		sign = true;
		return *this;
	}
	int borrow = 0;
	BigInt temp1 = *this, temp2 = in;
	if (!checkbig(*this, in, true)) {
		temp1 = in;
		temp2 = *this;
		sign = !sign;
	}
	long long int i = static_cast<long long>(temp1.val.size()) - 1, j = static_cast<long long>(temp2.val.size()) - 1;
	while (i >= 0 || j >= 0) {
		result.insert(result.begin(), (i >= 0 ? temp1.val[i] : 0) - (j >= 0 ? temp2.val[j] : 0) - borrow);
		borrow = 0;
		if (result[0] < 0) {
			borrow = (-result[0]) / 10 + ((-result[0]) % 10 != 0);
			result[0] += borrow * 10;
		}
		if (i >= 0) {
			i--;
		}
		if (j >= 0) {
			j--;
		}
	}
	if (borrow > 0) {
		sign = !sign;
		result.insert(result.begin(), 10 - borrow);
	}
	val = result;
	erasezero(*this);
	return *this;
}
BigInt BigInt::operator*=(const BigInt& in) {
	vector<int> res(val.size() + in.val.size());
	int hold = 0;
	for (long long int i = static_cast<unsigned long long>(val.size()) - 1; i >= 0; i--) {
		for (long long int j = static_cast<unsigned long long>(in.val.size()) - 1; j >= 0; j--) {
			res[i + j + 1] += val[i] * in.val[j] + hold;
			hold = res[i + j + 1] / 10;
			res[i + j + 1] %= 10;
		}
	}
	res[0] = hold;
	if (hold == 0) {
		res.erase(res.begin());
	}
	val = res;
	sign = (sign && in.sign || !sign && !in.sign);
	erasezero(*this);
	return *this;
}
BigInt BigInt::operator/=(const BigInt& in) {
	BigInt a;
	a = *this;
	vector<int> result(0);
	for (long long int i = 0; i < static_cast<long long>(a.val.size()) - in.val.size() + 1; i++) {
		bool ok = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < in.val.size(); j++) {
				if (a.val[i + j] - in.val[j] < 0) {
					ok = false;
				}
				else if (a.val[i + j] - in.val[j] > 0) {
					break;
				}
			}
			if (!ok) {
				break;
			}
			for (int j = 0; j < in.val.size(); j++) {
				a.val[i + j] -= in.val[j];
				if (a.val[i + j] < 0) {
					a.val[i + j] += 10;
					a.val[i + j - 1]--;
				}
			}
			count++;
		}
		if (i < static_cast<long long>(a.val.size()) - in.val.size()) {
			a.val[i + 1] = a.val[i + 1] + a.val[i] * 10;
			a.val[i] = 0;
		}
		result.push_back(count);
	}
	if (result.size() == 0) {
		result.push_back(0);
	}
	val = result;
	sign = (sign && in.sign || !sign && !in.sign);
	erasezero(*this);
	return *this;
}
BigInt BigInt::operator%=(const BigInt& in) {
	*this = *this - (BigInt(in) * (*this / in));
	return *this;
}

bool BigInt::checkbig(const BigInt& a, const BigInt& b, bool isunsigned) {
	if (!isunsigned && a.sign && !b.sign) {
		return true;
	}
	if (!isunsigned && !a.sign && b.sign) {
		return false;
	}
	bool res = true;
	if (a.val.size() > b.val.size()) {
		res = true;
	}
	else if (a.val.size() < b.val.size()) {
		res = false;
	}
	else {
		for (unsigned long long int i = 0; i < a.val.size(); i++) {
			if (a.val[i] > b.val[i]) {
				res = true;
				break;
			}
			else if (a.val[i] < b.val[i]) {
				res = false;
				break;
			}
		}
	}
	if (!a.sign && !b.sign && !isunsigned) {
		res = !res;
	}
	return res;
}
void BigInt::erasezero(BigInt& a) {
	while (a.val.size() > 0 && a.val[0] == 0) {
		a.val.erase(a.val.begin());
	}
	if (a.val.size() == 0) {
		a.val.push_back(0);
		a.sign = true;
	}
}
vector<int> BigInt::Getval() {
	return val;
}
bool BigInt::Getsign() {
	return sign;
}