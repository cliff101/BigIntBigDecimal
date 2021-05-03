#include "BigInt.h"
#include "BigDecimal.h"
#include "CommandParser.h"

BigInt::BigInt() {
	val = vector<short>(1, 0);
	sign = true;
	isinf = false;
	isundefined = false;
}
BigInt::BigInt(string in) {
	CommandParser temp;
	if (temp.IsCommand(in)) {//判斷輸入是純數值還是command
		*this = temp.Getcommandvalue(in, *this);//輸入是command，吃command parser的回傳值
	}
	else {
		in = stringprettify(in);//先美化輸入的string
		string strsign = "";
		if (in[0] == '+' || in[0] == '-') {
			strsign = in[0];
			in.erase(in.begin());
		}
		val = vector<short>(in.length());
		for (unsigned long long int i = 0; i < in.length(); i++) {
			val[i] = in[i] - '0';
		}
		if (strsign == "-") {
			sign = false;
		}
		else {
			sign = true;
		}
		erasezero(*this);
		isinf = false;
		isundefined = false;
	}
}
BigInt::BigInt(const char* in) {
	*this = string(in);
}
BigInt::BigInt(const BigInt& in) {
	val = vector<short>(in.val);
	sign = in.sign;
	isinf = in.isinf;
	isundefined = in.isundefined;
}
BigInt::BigInt(int ini) {
	string in = stringprettify(to_string(ini));
	string strsign = "";
	if (in[0] == '+' || in[0] == '-') {
		strsign = in[0];
		in.erase(in.begin());
	}
	val = vector<short>(in.length());
	for (unsigned long long int i = 0; i < in.length(); i++) {
		val[i] = in[i] - '0';
	}
	if (strsign == "-") {
		sign = false;
	}
	else {
		sign = true;
	}
	erasezero(*this);
	isinf = false;
	isundefined = false;
}
BigInt::BigInt(const BigDecimal& in) {
	BigDecimal cp(in);
	*this = (cp.GetBigIntup() / cp.GetBigIntdown());
	isinf = cp.Getinf();
	isundefined = cp.Getundefined();
}

BigInt BigInt::operator=(string in) {
	CommandParser temp;
	if (temp.IsCommand(in)) {
		*this = temp.Getcommandvalue(in, *this);
	}
	else {
		in = stringprettify(in);
		string strsign = "";
		if (in[0] == '+' || in[0] == '-') {//處理正負號
			strsign = in[0];
			in.erase(in.begin());
		}
		val = vector<short>(in.length());
		for (unsigned long long int i = 0; i < in.length(); i++) {//將string填入vector中
			val[i] = in[i] - '0';
		}
		if (strsign == "-") {
			sign = false;
		}
		else {
			sign = true;
		}
		erasezero(*this);
		isinf = false;
		isundefined = false;
	}
	return *this;
}
BigInt BigInt::operator=(int ini) {
	string in = stringprettify(to_string(ini));//先美化輸入的string
	string strsign = "";
	if (in[0] == '+' || in[0] == '-') {
		strsign = in[0];
		in.erase(in.begin());
	}
	val = vector<short>(in.length());
	for (unsigned long long int i = 0; i < in.length(); i++) {//將string存到vector中
		val[i] = in[i] - '0';
	}
	if (strsign == "-") {
		sign = false;
	}
	else {
		sign = true;
	}
	erasezero(*this);
	isinf = false;
	isundefined = false;
	return *this;
}
BigInt BigInt::operator=(const BigDecimal& in) {
	BigDecimal cp(in);
	*this = (cp.GetBigIntup() / cp.GetBigIntdown());
	isinf = cp.Getinf();
	isundefined = cp.Getundefined();
	return *this;
}

BigInt BigInt::operator+(const BigInt& in) const {
	BigInt out;
	if (isundefined || in.isundefined || isinf && in.isinf && (!sign && in.sign || sign && !in.sign)) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && sign || in.isinf && in.sign) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf && !sign || in.isinf && !in.sign) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> result(0);
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
		hold = 0;
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
BigInt BigInt::operator-(const BigInt& in) const {
	BigInt out;
	if (isundefined || in.isundefined || isinf && in.isinf && (sign && in.sign || !sign && !in.sign)) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && sign || in.isinf && !in.sign) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf && !sign || in.isinf && in.sign) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> result(0);
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
BigInt BigInt::operator*(const BigInt& in) const {
	BigInt out;
	if (isundefined || in.isundefined) {
		out.isundefined = true;
		return out;
	}
	else if ((isinf || in.isinf) && (in.sign && sign || !in.sign && !sign)) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf || in.isinf) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> res(val.size() + in.val.size());
	int hold = 0;
	for (long long int i = static_cast<unsigned long long>(val.size()) - 1; i >= 0; i--) {
		long long int j = static_cast<unsigned long long>(in.val.size()) - 1;
		for (; j >= 0; j--) {
			res[i + j + 1] += val[i] * in.val[j] + hold;
			hold = res[i + j + 1] / 10;
			res[i + j + 1] %= 10;
		}
		res[i + j + 1] += hold;
		hold = 0;
	}
	if (res[0] == 0) {
		res.erase(res.begin());
	}
	out.val = res;
	out.sign = (sign && in.sign || !sign && !in.sign);
	erasezero(out);
	return out;
}
BigInt BigInt::operator/(const BigInt& in) const {
	BigInt a, out;
	if (isundefined || in.isundefined || isinf && in.isinf || (!in.isinf && !isinf || isinf) && in.val.size() == 1 && BigInt(in) == 0 && val.size() == 1 && *this == 0) {
		out.isundefined = true;
		return out;
	}
	else if (!isinf && val.size() == 1 && *this == 0 || in.isinf) {
		out = 0;
		return out;
	}
	else if (isinf && in.sign && sign || sign && in.val.size() == 1 && BigInt(in) == 0) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf || !sign && in.val.size() == 1 && BigInt(in) == 0) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	else if (in.isinf) {
		out = 0;
		return out;
	}
	a = *this;
	vector<short> result(0);
	for (long long int i = 0; i < static_cast<long long>(a.val.size()) - in.val.size() + 1; i++) {
		bool ok = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < in.val.size(); j++) {
				if (a.val[i + j] - in.val[j] < 0) {
					ok = false;
					break;
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
					for (int k = 1; k <= i + j; k++) {
						a.val[i + j - k + 1] += 10;
						a.val[i + j - k]--;
						if (a.val[i + j - k] >= 0) {
							break;
						}
					}

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
BigInt BigInt::operator%(const BigInt& in) const {
	return *this - (BigInt(in) * (*this / in));
}

BigInt BigInt::operator+(const BigInt& in) {
	BigInt out;
	if (isundefined || in.isundefined || isinf && in.isinf && (!sign && in.sign || sign && !in.sign)) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && sign || in.isinf && in.sign) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf && !sign || in.isinf && !in.sign) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> result(0);
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
		hold = 0;
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
	if (isundefined || in.isundefined || isinf && in.isinf && (sign && in.sign || !sign && !in.sign)) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && sign || in.isinf && !in.sign) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf && !sign || in.isinf && in.sign) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> result(0);
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
	if (isundefined || in.isundefined) {
		out.isundefined = true;
		return out;
	}
	else if ((isinf || in.isinf) && (in.sign && sign || !in.sign && !sign)) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf || in.isinf) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> res(val.size() + in.val.size());
	int hold = 0;
	for (long long int i = static_cast<unsigned long long>(val.size()) - 1; i >= 0; i--) {
		long long int j = static_cast<unsigned long long>(in.val.size()) - 1;
		for (; j >= 0; j--) {
			res[i + j + 1] += val[i] * in.val[j] + hold;
			hold = res[i + j + 1] / 10;
			res[i + j + 1] %= 10;
		}
		res[i + j + 1] += hold;
		hold = 0;
	}
	if (res[0] == 0) {
		res.erase(res.begin());
	}
	out.val = res;
	out.sign = (sign && in.sign || !sign && !in.sign);
	erasezero(out);
	return out;
}
BigInt BigInt::operator/(const BigInt& in) {
	BigInt a, out;
	if (isundefined || in.isundefined || isinf && in.isinf || (!in.isinf && !isinf || isinf) && in.val.size() == 1 && BigInt(in) == 0 && val.size() == 1 && *this == 0) {
		out.isundefined = true;
		return out;
	}
	else if (!isinf && val.size() == 1 && *this == 0 || in.isinf) {
		out = 0;
		return out;
	}
	else if (isinf && in.sign && sign || sign && in.val.size() == 1 && BigInt(in) == 0) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf || !sign && in.val.size() == 1 && BigInt(in) == 0) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	else if (in.isinf) {
		out = 0;
		return out;
	}
	a = *this;
	vector<short> result(0);
	for (long long int i = 0; i < static_cast<long long>(a.val.size()) - in.val.size() + 1; i++) {
		bool ok = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < in.val.size(); j++) {
				if (a.val[i + j] - in.val[j] < 0) {
					ok = false;
					break;
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
					for (int k = 1; k <= i + j; k++) {
						a.val[i + j - k + 1] += 10;
						a.val[i + j - k]--;
						if (a.val[i + j - k] >= 0) {
							break;
						}
					}

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

BigInt BigInt::operator+(BigInt& in) {
	BigInt out;
	if (isundefined || in.isundefined || isinf && in.isinf && (!sign && in.sign || sign && !in.sign)) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && sign || in.isinf && in.sign) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf && !sign || in.isinf && !in.sign) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> result(0);
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
		hold = 0;
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
BigInt BigInt::operator-(BigInt& in) {
	BigInt out;
	if (isundefined || in.isundefined || isinf && in.isinf && (sign && in.sign || !sign && !in.sign)) {
		out.isundefined = true;
		return out;
	}
	else if (isinf && sign || in.isinf && !in.sign) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf && !sign || in.isinf && in.sign) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> result(0);
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
BigInt BigInt::operator*(BigInt& in) {
	BigInt out;
	if (isundefined || in.isundefined) {
		out.isundefined = true;
		return out;
	}
	else if ((isinf || in.isinf) && (in.sign && sign || !in.sign && !sign)) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf || in.isinf) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	vector<short> res(val.size() + in.val.size());
	int hold = 0;
	for (long long int i = static_cast<unsigned long long>(val.size()) - 1; i >= 0; i--) {
		long long int j = static_cast<unsigned long long>(in.val.size()) - 1;
		for (; j >= 0; j--) {
			res[i + j + 1] += val[i] * in.val[j] + hold;
			hold = res[i + j + 1] / 10;
			res[i + j + 1] %= 10;
		}
		res[i + j + 1] += hold;
		hold = 0;
	}
	if (res[0] == 0) {
		res.erase(res.begin());
	}
	out.val = res;
	out.sign = (sign && in.sign || !sign && !in.sign);
	erasezero(out);
	return out;
}
BigInt BigInt::operator/(BigInt& in) {
	BigInt a, out;
	if (isundefined || in.isundefined || isinf && in.isinf || (!in.isinf && !isinf || isinf) && in.val.size() == 1 && BigInt(in) == 0 && val.size() == 1 && *this == 0) {
		out.isundefined = true;
		return out;
	}
	else if (!isinf && val.size() == 1 && *this == 0 || in.isinf) {
		out = 0;
		return out;
	}
	else if (isinf && in.sign && sign || sign && in.val.size() == 1 && BigInt(in) == 0) {
		out.sign = true;
		out.isinf = true;
		return out;
	}
	else if (isinf || !sign && in.val.size() == 1 && BigInt(in) == 0) {
		out.sign = false;
		out.isinf = true;
		return out;
	}
	else if (in.isinf) {
		out = 0;
		return out;
	}
	a = *this;
	vector<short> result(0);
	for (long long int i = 0; i < static_cast<long long>(a.val.size()) - in.val.size() + 1; i++) {
		bool ok = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < in.val.size(); j++) {
				if (a.val[i + j] - in.val[j] < 0) {
					ok = false;
					break;
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
					for (int k = 1; k <= i + j; k++) {
						a.val[i + j - k + 1] += 10;
						a.val[i + j - k]--;
						if (a.val[i + j - k] >= 0) {
							break;
						}
					}

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
BigInt BigInt::operator%(BigInt& in) {
	return *this - (BigInt(in) * (*this / in));
}

BigInt BigInt::operator+=(const BigInt& in) {
	if (isundefined || in.isundefined || isinf && in.isinf && (!sign && in.sign || sign && !in.sign)) {
		isundefined = true;
		return *this;
	}
	else if (isinf && sign || in.isinf && in.sign) {
		sign = true;
		isinf = true;
		return *this;
	}
	else if (isinf && !sign || in.isinf && !in.sign) {
		sign = false;
		isinf = true;
		return  *this;
	}
	vector<short> result(0);
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
		hold = 0;
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
	if (isundefined || in.isundefined || isinf && in.isinf && (sign && in.sign || !sign && !in.sign)) {
		isundefined = true;
		return *this;
	}
	else if (isinf && sign || in.isinf && !in.sign) {
		sign = true;
		isinf = true;
		return *this;
	}
	else if (isinf && !sign || in.isinf && in.sign) {
		sign = false;
		isinf = true;
		return *this;
	}
	vector<short> result(0);
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
	if (isundefined || in.isundefined) {
		isundefined = true;
		return *this;
	}
	else if ((isinf || in.isinf) && in.sign && sign) {
		sign = true;
		isinf = true;
		return *this;
	}
	else if (isinf || in.isinf) {
		sign = false;
		isinf = true;
		return *this;
	}
	vector<short> res(val.size() + in.val.size());
	int hold = 0;
	for (long long int i = static_cast<unsigned long long>(val.size()) - 1; i >= 0; i--) {
		long long int j = static_cast<unsigned long long>(in.val.size()) - 1;
		for (; j >= 0; j--) {
			res[i + j + 1] += val[i] * in.val[j] + hold;
			hold = res[i + j + 1] / 10;
			res[i + j + 1] %= 10;
		}
		res[i + j + 1] += hold;
		hold = 0;
	}
	if (res[0] == 0) {
		res.erase(res.begin());
	}
	val = res;
	sign = (sign && in.sign || !sign && !in.sign);
	erasezero(*this);
	return *this;
}
BigInt BigInt::operator/=(const BigInt& in) {
	if (isundefined || in.isundefined || isinf && in.isinf || (!in.isinf && !isinf || isinf) && in.val.size() == 1 && BigInt(in) == 0 && val.size() == 1 && *this == 0) {
		isundefined = true;
		return *this;
	}
	else if (!isinf && val.size() == 1 && *this == 0 || in.isinf) {
		*this = 0;
		return *this;
	}
	else if (isinf && in.sign && sign || sign && in.val.size() == 1 && BigInt(in) == 0) {
		sign = true;
		isinf = true;
		return *this;
	}
	else if (isinf || !sign && in.val.size() == 1 && BigInt(in) == 0) {
		sign = false;
		isinf = true;
		return *this;
	}
	else if (in.isinf) {
		*this = 0;
		return *this;
	}
	BigInt a;
	a = *this;
	vector<short> result(0);
	for (long long int i = 0; i < static_cast<long long>(a.val.size()) - in.val.size() + 1; i++) {
		bool ok = true;
		int count = 0;
		while (ok) {
			for (int j = 0; j < in.val.size(); j++) {
				if (a.val[i + j] - in.val[j] < 0) {
					ok = false;
					break;
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
					for (int k = 1; k <= i + j; k++) {
						a.val[i + j - k + 1] += 10;
						a.val[i + j - k]--;
						if (a.val[i + j - k] >= 0) {
							break;
						}
					}

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

bool BigInt::operator==(const BigInt& in) const {
	if (isundefined || in.isundefined) {
		return false;
	}
	else if (isinf && in.isinf) {
		return sign == in.sign;
	}
	if (val != in.val) {
		return false;
	}
	if (sign != sign) {
		return false;
	}
	return true;
}
bool BigInt::operator!=(const BigInt& in) const {
	if (isundefined || in.isundefined) {
		return false;
	}
	return !(*this == in);
}
bool BigInt::operator>(const BigInt& in)const {
	if (isundefined || in.isundefined) {
		return false;
	}
	else if (isinf && in.isinf) {
		return sign > in.sign;
	}
	return compair(*this, in) == 1;
}
bool BigInt::operator<(const BigInt& in) const {
	if (isundefined || in.isundefined) {
		return false;
	}

	else if (isinf && in.isinf) {
		return sign < in.sign;
	}
	return compair(*this, in) == -1;
}
bool BigInt::operator<=(const BigInt& in) const {
	if (isundefined || in.isundefined) {
		return false;
	}
	else if (isinf && in.isinf) {
		return sign <= in.sign;
	}
	return compair(*this, in) <= 0;
}
bool BigInt::operator>=(const BigInt& in) const {
	if (isundefined || in.isundefined) {
		return false;
	}
	else if (isinf && in.isinf) {
		return sign >= in.sign;
	}
	return compair(*this, in) >= 0;
}

BigInt BigInt::operator++(int) {
	BigInt temp(*this);
	*this += 1;
	return temp;
}

BigDecimal BigInt::Power(BigDecimal& in) {
	BigInt out = *this;
	BigDecimal outreal;
	BigInt cpup(in.GetBigIntup());
	if (isundefined || in.Getundefined() || isinf && in.Getvalup().size() == 1 && cpup == 0 || isinf && !sign) {
		out.isundefined = true;
		return out;
	}
	else if (in.Getinf() && !cpup.sign) {
		out = 0;
		return out;
	}
	else if (isinf || cpup.isinf) {
		out.isinf = true;
		return out;
	}
	else if (out == 1) {
		return out;
	}
	BigInt up = in.GetBigIntup();//取得要Power的分子
	BigInt down = in.GetBigIntdown();//取得要Power的分母(開根號)
	bool upsign = up.sign;//先將sign儲存起來
	up.sign = true;
	if (up > 1) {//處理分子部分的刺次方
		BigInt cp = out;
		for (BigInt i = 0; i < up - 1; i += 1) {
			out *= cp;
		}
	}
	else if (up == 0) {
		out = 1;
	}

	if (val.size() == 1 && *this == 1) {
		return *this;
	}

	if (down == 2) {//處理分母部分的次方(開2次)
		vector<BigInt> part(out.val.size() / 2 + out.val.size() % 2);
		BigInt besub(0), sub(0), res(0), resdown(1), base(0), thisres(0);
		for (long long int i = static_cast<long long>(out.val.size()) - 1; i >= 0; i -= 2) {
			if (i - 1 >= 0) {
				part[i / 2] = out.val[i] + 10 * out.val[i - 1];
			}
			else {
				part[0] = out.val[i];
			}
		}
		res += long long(std::pow(part[0].Getint(), 0.5));
		besub = part[0] - res * res;
		for (unsigned long long int i = 1; i < (static_cast<unsigned long long>(out.val.size()) + 1) / 2; i++) {
			base = res * 20;
			besub = besub * 100 + part[i];

			thisres = besub / base;
			sub = (base + thisres) * thisres;
			while (sub > besub) {
				thisres -= 1;
				sub = (base + thisres) * thisres;
			}

			res = res * 10 + thisres;
			besub -= sub;
		}
		if (besub == 0) {
			outreal = BigDecimal(res);
		}
		else {
			for (int i = 0; i < BigDecimal::precision; i++) {
				resdown.val.push_back(0);
				base = res * 20;
				besub.val.push_back(0);
				besub.val.push_back(0);

				thisres = besub / base;
				sub = (base + thisres) * thisres;
				while (sub > besub) {
					thisres -= 1;
					sub = (base + thisres) * thisres;
				}

				res.val.push_back(0);
				res += thisres;
				besub -= sub;
			}
			outreal = BigDecimal(res, resdown);
		}
	}
	else if(down!=0){//分母大於2次，用牛頓法來猜
		BigDecimal temp1, temp2, temp3, temp4, temp5;
		BigDecimal x0 = out / BigDecimal(down), old;
		BigInt bigintdown = in.GetBigIntdown();
		while (true) {
			old = x0;

			x0 = x0 * (BigDecimal(1) - (BigDecimal(1) - out / x0.Power(bigintdown)) / 3);

			x0.Simplify();

			cout << old.Getvalreal() << endl << x0.Getvalreal() << endl;
			if (old == x0) {
				break;
			}
		}
		outreal = x0;
	}
	else {
		outreal = out;
	}
	if (!upsign) {
		return BigDecimal(outreal.GetBigIntdown(), outreal.GetBigIntup());
	}
	return outreal;
}
BigInt BigInt::Power(BigInt& in) {
	BigInt out = *this;
	if (isundefined || in.Getundefined() || isinf && !sign) {
		out.isundefined = true;
		return out;
	}
	else if (in.Getinf() && in.sign) {
		out = 0;
		return out;
	}
	else if (isinf || in.isinf) {
		out.isinf = true;
		return out;
	}
	if (in > 0) {
		BigInt cp = out;
		for (BigInt i = 0; i < in - 1; i += 1) {
			out *= cp;
		}
	}
	else if(in == 0){
		out = 1;
	}
	else if (out!=0){
		out = 0;
	}
	else {
		out.isinf = true;
	}
	return out;
}

ostream& operator<<(ostream& ccout, const BigInt& in)
{
	ccout << BigInt(in).Getvalreal();
	return ccout;
}
istream& operator>>(istream& ccin, BigInt& in)
{
	string temp;
	ccin >> temp;
	in = temp;
	return ccin;
}

void BigInt::Print(ostream& ccout) const {
	ccout << BigInt(*this).Getvalreal();
}

//等於回傳true
bool BigInt::checkbig(const BigInt& a, const  BigInt& b, bool isunsigned) const {
	if (!isunsigned && a.sign && !b.sign || BigInt(a) == BigInt(0) && BigInt(b) == 0) {
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
short BigInt::compair(const BigInt& a, const  BigInt& b) const {
	if (BigInt(a) == BigInt(0) && BigInt(b) == 0) {
		return 0;
	}
	if (a.sign && !b.sign) {
		return 1;
	}
	if (!a.sign && b.sign) {
		return -1;
	}
	short res = 0;
	if (a.val.size() > b.val.size()) {
		res = 1;
	}
	else if (a.val.size() < b.val.size()) {
		res = -1;
	}
	else {
		for (unsigned long long int i = 0; i < a.val.size(); i++) {
			if (a.val[i] > b.val[i]) {
				res = 1;
				break;
			}
			else if (a.val[i] < b.val[i]) {
				res = -1;
				break;
			}
		}
	}
	if (!a.sign && !b.sign) {
		res *= -1;
	}
	return res;
}
void BigInt::erasezero(BigInt& a) const {
	while (a.val.size() > 0 && a.val[0] == 0) {
		a.val.erase(a.val.begin());
	}
	if (a.val.size() == 0) {
		a.val.push_back(0);
		a.sign = true;
	}
}
string BigInt::stringprettify(string in) {
	string sign = "";
	if (in[0] == '+' || in[0] == '-') {
		sign = in[0];
		in.erase(in.begin());
	}
	while (in.length() > 0 && in[0] == '0') {
		in.erase(in.begin());
	}
	for (unsigned long long int i = 0; i < in.size(); i++) {
		if (in[i] == '.') {
			in = in.substr(0, i);
			break;
		}
	}
	if (in.length() == 0) {
		in.push_back('0');
	}
	in = sign + in;
	return in;
}

vector<short> BigInt::Getval() {
	return val;
}
string BigInt::Getvalreal() {
	if (isundefined) {
		return "nan";
	}
	string result = "";
	if (!sign) {
		result += '-';
	}
	if (isinf) {
		result += "inf";
		return result;
	}
	for (unsigned long long int i = 0; i < val.size(); i++) {
		result += val[i] + '0';
	}
	return result;
}
long long BigInt::Getint() {
	long long res = 0;
	for (unsigned long long i = 0; i < val.size(); i++) {
		res *= 10;
		res += val[i];
	}
	if (!sign) {
		res *= -1;
	}
	return res;
}
bool BigInt::Getsign() {
	return sign;
}
void BigInt::Setsign(bool in) {
	sign = in;
}
bool BigInt::Getinf() {
	return isinf;
}
bool BigInt::Getundefined() {
	return isundefined;
}
int BigInt::Getnumlength() {
	return val.size();
}

int BigInt::Getvallength()
{
	return val.size();
}
