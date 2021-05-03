#include "CommandParser.h"

#define BIAS_BLANK string("         ")

bool keeprun = true;

void setConsoleColor(WORD c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void signal_callback_handler(int signum) {
	keeprun = false;
}
CommandParser::retmsg CommandParser::createerrmsg(unsigned long long int i, string info) {
	retmsg msg;
	msg.ok = false;
	msg.errmsg = "";
	for (unsigned long long int j = 0; j < i; j++) {
		msg.errmsg += " ";
	}
	msg.errmsg += BIAS_BLANK + "^~~  " + info;
	return msg;
}

CommandParser::CommandParser() {
	BigValueVar = map<string, CommandParser::BigValue>();
}

void CommandParser::Inputcommand(string in) {
	BigDecimal::precision = 120;///////////////////////////////////////
	signal(SIGINT, signal_callback_handler);

	unsigned long long i = 0;

	blankslide(in, i);
	if (i == in.length()) {
		return;
	}
	int checkbb = checkb(in, i);
	if (checkbb != 0) {
		setConsoleColor(FOREGROUND_RED);
		cout << createerrmsg(checkbb == 1 ? i : 0, "括弧左右不對秤").errmsg << endl;
		setConsoleColor(7);
		return;
	}
	i = 0;

	keeprun = true;
	retmsg setmode = checkset(in, i);
	if (setmode.mode == 0) {
		i = 0;
	}
	else if (setmode.mode == 3) {
		if (BigValueVar.find(setmode.varname) == BigValueVar.end()) {
			setConsoleColor(FOREGROUND_RED);
			cout << createerrmsg(i - 1, "嘗試刪除未命名的變數").errmsg << endl;
			setConsoleColor(7);
		}
		else {
			BigValueVar.erase(setmode.varname);
		}
		return;
	}
	retmsg msg = ProceedCommand(in, i, 8);
	if (!keeprun) {
		setConsoleColor(6);
		cout << "SIGNAL CAUGHT." << endl;
		setConsoleColor(7);
	}
	else if (!msg.ok) {
		setConsoleColor(FOREGROUND_RED);
		cout << msg.errmsg << endl;
		setConsoleColor(7);
	}
	else {
		if (setmode.mode == 1) {
			if (BigValueVar.find(setmode.varname) != BigValueVar.end()) {
				if (BigValueVar[setmode.varname].IsInt) {
					if (msg.value.IsInt) {
						BigValueVar[setmode.varname] = msg.value;
					}
					else {
						msg.value.IsInt = true;
						msg.value.BigIntVal = msg.value.BigDecimalVal;
						BigValueVar[setmode.varname] = msg.value;
					}
				}
				else {
					if (msg.value.IsInt) {
						msg.value.IsInt = false;
						msg.value.BigDecimalVal = msg.value.BigIntVal;
						BigValueVar[setmode.varname] = msg.value;
					}
					else {
						BigValueVar[setmode.varname] = msg.value;
					}
				}
			}
			else {
				BigValueVar[setmode.varname] = msg.value;
			}
		}
		else if (setmode.mode == 2) {
			BigValueVar[setmode.varname] = msg.value;
			if (setmode.value.IsInt && !BigValueVar[setmode.varname].IsInt) {
				BigValueVar[setmode.varname].IsInt = true;
				BigValueVar[setmode.varname].BigIntVal = BigValueVar[setmode.varname].BigDecimalVal;
			}
			else if (!setmode.value.IsInt && BigValueVar[setmode.varname].IsInt) {
				BigValueVar[setmode.varname].IsInt = false;
				BigValueVar[setmode.varname].BigDecimalVal = BigValueVar[setmode.varname].BigIntVal;
			}
		}
		else {
			BigDecimal::precision = 100;///////////////////////////////////////
			setConsoleColor(10);
			if (msg.value.IsInt) {
				cout << msg.value.BigIntVal.Getvalreal() << endl;
			}
			else {
				cout << msg.value.BigDecimalVal.Getvalreal() << endl;
			}
			setConsoleColor(7);
		}
	}
}
BigDecimal CommandParser::Getcommandvalue(string in, BigDecimal&) {
	signal(SIGINT, signal_callback_handler);

	unsigned long long i = 0;
	int checkbb = checkb(in, i);
	if (checkbb != 0) {
		cout << BIAS_BLANK + in << endl;
		setConsoleColor(FOREGROUND_RED);
		cout << createerrmsg(checkbb == 1 ? i : 0, "括弧左右不對秤").errmsg << endl;
		setConsoleColor(7);
		return 0;
	}
	i = 0;
	keeprun = true;
	retmsg setmode = checkset(in, i);
	if (setmode.mode == 0) {
		i = 0;
	}
	else {
		cout << BIAS_BLANK + in << endl;
		setConsoleColor(FOREGROUND_RED);
		cout << createerrmsg(i - 1, "非std input模式不支援設定變數").errmsg << endl;
		setConsoleColor(7);
		return 0;
	}
	retmsg msg = ProceedCommand(in, i, 8);
	if (!keeprun) {
		cout << in << endl;
		setConsoleColor(6);
		cout << "SIGNAL CAUGHT." << endl;
		setConsoleColor(7);
	}
	else if (!msg.ok) {
		cout << BIAS_BLANK + in << endl;
		setConsoleColor(FOREGROUND_RED);
		cout << msg.errmsg << endl;
		setConsoleColor(7);
	}
	else {
		if (msg.value.IsInt) {
			return msg.value.BigIntVal;
		}
		else {
			return msg.value.BigDecimalVal;
		}
	}
}
BigInt CommandParser::Getcommandvalue(string in, BigInt&) {
	signal(SIGINT, signal_callback_handler);

	unsigned long long i = 0;
	int checkbb = checkb(in, i);
	if (checkbb != 0) {
		cout << BIAS_BLANK + in << endl;
		setConsoleColor(FOREGROUND_RED);
		cout << createerrmsg(checkbb == 1 ? i : 0, "括弧左右不對秤").errmsg << endl;
		setConsoleColor(7);
		return 0;
	}
	i = 0;
	keeprun = true;
	retmsg setmode = checkset(in, i);
	if (setmode.mode == 0) {
		i = 0;
	}
	else {
		cout << BIAS_BLANK + in << endl;
		setConsoleColor(FOREGROUND_RED);
		cout << createerrmsg(i - 1, "非std input模式不支援設定變數").errmsg << endl;
		setConsoleColor(7);
		return 0;
	}
	retmsg msg = ProceedCommand(in, i, 8);
	if (!keeprun) {
		cout << in << endl;
		setConsoleColor(6);
		cout << "SIGNAL CAUGHT." << endl;
		setConsoleColor(7);
	}
	else if (!msg.ok) {
		cout << BIAS_BLANK + in << endl;
		setConsoleColor(FOREGROUND_RED);
		cout << msg.errmsg << endl;
		setConsoleColor(7);
	}
	else {
		if (msg.value.IsInt) {
			return msg.value.BigIntVal;
		}
		else {
			return msg.value.BigDecimalVal;
		}
	}
}

bool CommandParser::IsCommand(string& in)
{
	unsigned long long int i = 0;
	blankslide(in, i);
	if (i >= in.size()) {
		return true;
	}

	bool dot = false;
	if (in[i] == '.') {
		dot = true;
	}
	else if (in[i] != '+' && in[i] != '-' && !isnumeric(in[i])) {
		return true;
	}
	i++;
	for (; i < in.length(); i++) {
		if (!isnumeric(in[i]) && (in[i] != '.' || in[i] == '.' && dot)) {
			return true;
		}
		else if (in[i] == '.') {
			dot = true;
		}
	}
	return false;
}

/*
2 ! 由左至右
3 ^ 由右至左
4 +- (正負號) 由右至左
5 * / 由左至右
6 + - 由左至右
7 ()
8 (empty)
*/
CommandParser::retmsg CommandParser::ProceedCommand(string& cmd, unsigned long long& i, int prevpriority, bool isgiveval1, BigValue giveval1) {

	while (keeprun) {
		string operatorval = "";
		BigValue value1, value2;
		retmsg msg;
		int thispriority = 0;
		if (!isgiveval1) {
			msg = getvalue(cmd, i);
			if (!msg.ok) {
				return msg;
			}
			else if (msg.description == "(") {//發現(，強制下一層遞迴
				value1 = msg.value;
				i++;
				msg = ProceedCommand(cmd, i, 7);//return時，i停留在operator上
				if (!msg.ok) {
					return msg;
				}
				retmsg msg2;
				string operatorvaltemp = "*";
				msg2 = calculate(value1, operatorvaltemp, msg.value);
				value1 = msg2.value;
			}
			else {
				value1 = msg.value;
			}
			if (msg.description == "endofcmd") {
				return msg;
			}
		}
		else {
			value1 = giveval1;
		}

		while (keeprun) {
			msg = getoperator(cmd, i);
			if (!msg.ok) {
				return msg;
			}
			else if (msg.description == "endofcmd") {
				msg.ok = true;
				msg.value = value1;
				msg.errmsg = "";
				msg.priority = 8;
				return msg;
			}
			else if (msg.description == "(") {
				operatorval = "*";
				thispriority = 5;
			}
			else if (msg.description == ")") {
				msg.ok = true;
				msg.errmsg = "";
				msg.priority = 7;
				msg.value = value1;
				i++;
				return msg;//return時，i停留在operator上
			}
			else if (msg.description == "!") {
				msg = stagecalc(value1);
				if (!msg.ok) {
					return createerrmsg(i, msg.description);
				}
				else {
					value1 = msg.value;
				}
				i++;
				continue;
			}
			else {
				operatorval = msg.description;
				thispriority = msg.priority;
			}
			break;
		}

		if (thispriority > prevpriority || thispriority == prevpriority && operatorval != "^") {
			msg.ok = true;
			msg.errmsg = "";
			msg.priority = thispriority;
			msg.value = value1;
			return msg;//return時，i停留在operator上
		}
		else {
			i++;
			msg = ProceedCommand(cmd, i, thispriority);//return時，i停留在operator上
			if (!msg.ok) {
				return msg;
			}
			value2 = msg.value;
			retmsg msg2;
			msg2 = calculate(value1, operatorval, value2);//return時，i停留在operator上
			if (!msg2.ok) {
				return createerrmsg(i - 1, msg2.description);
			}
			else if (msg.description == "endofcmd" || cmd[i - 1] == ')') {//end of command 或出現)， 直接return
				return msg2;
			}
			isgiveval1 = true;
			giveval1 = msg2.value;
			prevpriority = 8;
		}
	}
	retmsg msg;
	return msg;//impossibel to happen, just for reduce warning
}
CommandParser::retmsg CommandParser::getoperator(string& cmd, unsigned long long& i) {
	blankslide(cmd, i);
	retmsg msg;
	if (i >= static_cast<unsigned long long>(cmd.length())) {
		msg.ok = true;
		msg.description = "endofcmd";
		return msg;
	}
	if (cmd[i] != '+' && cmd[i] != '-' && cmd[i] != '*' && cmd[i] != '/' && cmd[i] != '^' && cmd[i] != '!' && cmd[i] != '(' && cmd[i] != ')') {
		return createerrmsg(i, "無法辨識運算符");
	}
	if (cmd[i] == '(') {
		msg.priority = 1;
	}
	else if (cmd[i] == ')') {
		msg.priority = 7;
	}
	else if (cmd[i] == '!') {
		msg.priority = 2;
	}
	else if (cmd[i] == '^') {
		msg.priority = 3;
	}
	else if (cmd[i] == '*' || cmd[i] == '/') {
		msg.priority = 5;
	}
	else if (cmd[i] == '+' || cmd[i] == '-') {
		msg.priority = 6;
	}
	msg.ok = true;
	msg.description = cmd[i];
	return msg;
}
CommandParser::retmsg CommandParser::getvalue(string& cmd, unsigned long long& i, bool varerr) {
	blankslide(cmd, i);
	retmsg msg;
	if (i < cmd.length() && cmd[i] == '(') {
		msg.ok = true;
		msg.description = "(";
		msg.value.IsInt = true;
		msg.value.BigIntVal = BigInt(1);
		return msg;
	}
	string res = "";
	while (i < cmd.length() && (cmd[i] == '+' || cmd[i] == '-')) {
		if (cmd[i] == '-' && res == "-" || cmd[i] == '+' && (res == "+" || res == "")) {
			res = '+';
		}
		else {
			res = '-';
		}
		i++;
		blankslide(cmd, i);
	}
	bool first = true,
		isintbool = true,
		isvar = false;
	if (i < cmd.length() && isalphabetic(cmd[i])) {
		isvar = true;
	}
	while (i < cmd.length()) {
		if (!isvar) {
			if (isnumeric(cmd[i])) {
				res += cmd[i];
				first = false;
			}
			else if (isintbool && cmd[i] == '.') {
				res += '.';
				isintbool = false;
			}
			else {
				break;
			}
		}
		else {
			if (isalphabetic(cmd[i]) || isnumeric(cmd[i])) {
				res += cmd[i];
				first = false;
			}
			else {
				break;
			}
		}
		i++;
	}
	if (cmd[i] == '(') {
		msg.description = "(";
	}
	else if (first) {
		return createerrmsg(i, "須為數值");
	}

	if (!isvar) {
		msg.ok = true;
		if (isintbool) {
			msg.value.IsInt = true;
			if (res.length() == 1 && res[0] == '+') {
				msg.value.BigIntVal = BigInt(1);
			}
			else if (res.length() == 1 && res[0] == '-') {
				msg.value.BigIntVal = BigInt(-1);
			}
			else {
				msg.value.BigIntVal = BigInt(res);
			}
		}
		else {
			msg.value.IsInt = false;
			msg.value.BigDecimalVal = BigDecimal(res);
		}
		return msg;
	}
	else {
		msg.varname = res;
		bool sign = true;
		bool find = false;
		if (res[0] == '+' || res[0] == '-') {
			sign = res[0] == '+';
			res.erase(res.begin());
			find = true;
		}

		if (BigValueVar.find(res) == BigValueVar.end()) {
			if (!varerr && !find) {
				msg.ok = true;
				msg.errmsg = "";
				return msg;
			}
			else {
				return createerrmsg(i - 1, "未命名的變數");
			}

		}
		else {
			msg.ok = true;
			msg.value = BigValueVar[res];
			if (!sign) {
				if (msg.value.IsInt) {
					msg.value.BigIntVal = msg.value.BigIntVal * -1;
				}
				else {
					msg.value.BigDecimalVal = msg.value.BigDecimalVal * -1;
				}
			}
			return msg;
		}

	}
}
CommandParser::retmsg CommandParser::calculate(BigValue& val1, string& operatorval, BigValue& val2) {
	retmsg msg;
	if (val1.IsInt && val2.IsInt) {
		msg.ok = true;
		msg.value.IsInt = true;
		if (operatorval == "+") {
			msg.value.BigIntVal = val1.BigIntVal + val2.BigIntVal;
		}
		else if (operatorval == "-") {
			msg.value.BigIntVal = val1.BigIntVal - val2.BigIntVal;
		}
		else if (operatorval == "*") {
			msg.value.BigIntVal = val1.BigIntVal * val2.BigIntVal;
		}
		else if (operatorval == "/") {
			msg.value.BigIntVal = val1.BigIntVal / val2.BigIntVal;
		}
		else if (operatorval == "^") {
			msg.value.BigIntVal = val1.BigIntVal.Power(val2.BigIntVal);

		}

	}
	else {
		msg.ok = true;
		msg.value.IsInt = false;
		if (operatorval == "+") {
			msg.value.BigDecimalVal = (val1.IsInt ? BigDecimal(val1.BigIntVal) : val1.BigDecimalVal) + (val2.IsInt ? BigDecimal(val2.BigIntVal) : val2.BigDecimalVal);
		}
		else if (operatorval == "-") {
			msg.value.BigDecimalVal = (val1.IsInt ? BigDecimal(val1.BigIntVal) : val1.BigDecimalVal) - (val2.IsInt ? BigDecimal(val2.BigIntVal) : val2.BigDecimalVal);
		}
		else if (operatorval == "*") {
			msg.value.BigDecimalVal = (val1.IsInt ? BigDecimal(val1.BigIntVal) : val1.BigDecimalVal) * (val2.IsInt ? BigDecimal(val2.BigIntVal) : val2.BigDecimalVal);
		}
		else if (operatorval == "/") {
			msg.value.BigDecimalVal = (val1.IsInt ? BigDecimal(val1.BigIntVal) : val1.BigDecimalVal) / (val2.IsInt ? BigDecimal(val2.BigIntVal) : val2.BigDecimalVal);
		}
		else if (operatorval == "^") {
			BigDecimal temp(val2.IsInt ? BigDecimal(val2.BigIntVal) : val2.BigDecimalVal);
			if (temp.GetBigIntdown() > 2) {
				msg.ok = false;
				msg.description = "不支援非0.5倍數的次方";
				return msg;
			}
			msg.value.BigDecimalVal = (val1.IsInt ? BigDecimal(val1.BigIntVal) : val1.BigDecimalVal).Power(temp);
		}
	}

	return msg;
}
CommandParser::retmsg CommandParser::stagecalc(BigValue& val1) {
	retmsg msg;
	BigValue cp = val1;
	if (!cp.IsInt && cp.BigDecimalVal.GetBigIntdown() != 1) {
		msg.ok = false;
		msg.description = "不支援浮點數階乘運算";
		return msg;
	}
	if (cp.IsInt) {
		if (!cp.BigIntVal.Getsign()) {
			msg.ok = false;
			msg.description = "不支援浮負數階乘運算";
			return msg;
		}
		else if (cp.BigIntVal < 1) {
			cp.BigIntVal = 1;
		}
		for (BigInt i = cp.BigIntVal - 1; i > 1 && keeprun; i -= 1) {
			cp.BigIntVal *= i;
		}
		msg.ok = true;
		msg.value = cp;
	}
	else {
		BigInt valup = val1.BigDecimalVal.GetBigIntup();
		if (!valup.Getsign()) {
			msg.ok = false;
			msg.description = "不支援浮負數階乘運算";
			return msg;
		}
		else if (valup < 1) {
			valup = 1;
		}
		for (BigInt i = valup - 1; i > 1 && keeprun; i -= 1) {
			valup *= i;
		}
		cp.BigDecimalVal = valup;
		msg.ok = true;
		msg.value = cp;
	}

	return msg;
}

void CommandParser::blankslide(string& cmd, unsigned long long& i) {
	while (i < cmd.length() && cmd[i] == ' ') {
		i++;
	}
}
bool CommandParser::isnumeric(char c) {
	if (c > '9' || c < '0') {
		return false;
	}
	return true;
}
bool CommandParser::isalphabetic(char c) {
	if (c > 'z' || c < 'a' && c > 'Z' || c < 'A') {
		return false;
	}
	return true;
}
bool CommandParser::isint(string& in) {
	for (unsigned long long int i = 0; i < in.length(); i++) {
		if (in[i] == '.') {
			return false;
		}
	}
	return true;
}
int CommandParser::checkb(string& in, unsigned long long int& i) {
	int total = 0;
	for (i; i < in.length(); i++) {
		if (in[i] == '(') {
			total++;
		}
		else if (in[i] == ')') {
			total--;
		}
		if (total < 0) {
			return -1;
		}
	}
	return total != 0;
}
//mode = 0:normal  mode = 1:set  mode = 2:set specific  mode = 3:del
CommandParser::retmsg CommandParser::checkset(string& cmd, unsigned long long int& i) {
	retmsg msg, msg2;
	msg = getvalue(cmd, i, false);
	msg.mode = 0;
	if (i < cmd.length()) {
		if (msg.varname == "Del") {
			msg2 = getvalue(cmd, i, false);
			if (msg2.varname != "") {
				msg.mode = 3;
				msg.varname = msg2.varname;
			}
			return msg;
		}
		else if (msg.varname == "Set") {
			msg2 = getvalue(cmd, i, false);
			if (msg2.varname == "Integer") {
				msg.value.IsInt = true;
			}
			else if (msg2.varname == "Decimal") {
				msg.value.IsInt = false;
			}
			else {
				return msg;
			}
			msg2 = getvalue(cmd, i, false);
			blankslide(cmd, i);
			if (msg2.varname != "" && i < cmd.length() && cmd[i] == '=') {
				i++;
				msg.mode = 2;
				msg.varname = msg2.varname;
			}
			return msg;
		}
		else if (msg.varname != "") {
			blankslide(cmd, i);
			if (i < cmd.length() && cmd[i] == '=') {
				i++;
				msg.mode = 1;
			}
		}
	}

	return msg;
}