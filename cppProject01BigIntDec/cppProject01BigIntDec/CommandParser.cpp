#include "CommandParser.h"

#define BIAS_BLANK string("    ")

bool keeprun = true;

void setConsoleColor(WORD c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void signal_callback_handler(int signum) {
	keeprun = false;
}

CommandParser::CommandParser() {
	BigIntVar = map<string, BigInt>();
	BigDecimalVar = map<string, BigDecimal>();
}
void CommandParser::Inputcommand(string in) {
	signal(SIGINT, signal_callback_handler);

	unsigned long long i = 0;
	keeprun = true;
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
	else if (msg.haveoutput) {
		if (msg.value.IsInt) {
			cout << msg.value.BigIntVal.Getvalreal() << endl;
		}
		else {
			cout << msg.value.BigDecimalVal.Getvalreal() << endl;
		}

	}

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
			else if (msg.description == "(") {
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
					msg.errmsg = "";
					for (unsigned long long int j = 0; j < i; j++) {
						msg.errmsg += " ";
					}
					msg.errmsg += BIAS_BLANK + "^~~  " + msg.description;
					return msg;
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
				msg2.errmsg = "";
				for (unsigned long long int j = 0; j < i; j++) {
					msg2.errmsg += " ";
				}
				msg2.errmsg += BIAS_BLANK + "^~~  " + msg2.description;
				return msg2;
			}
			else if (msg.description == "endofcmd" || cmd[i - 1] == ')') {
				return msg2;
			}
			isgiveval1 = true;
			giveval1 = msg2.value;
			prevpriority = 8;
		}
	}
}
CommandParser::retmsg CommandParser::getoperator(string& cmd, unsigned long long& i) {
	blandslide(cmd, i);
	retmsg msg;
	if (i >= static_cast<unsigned long long>(cmd.length())) {
		msg.ok = true;
		msg.description = "endofcmd";
		return msg;
	}
	if (cmd[i] != '+' && cmd[i] != '-' && cmd[i] != '*' && cmd[i] != '/' && cmd[i] != '^' && cmd[i] != '!' && cmd[i] != '(' && cmd[i] != ')') {
		msg.ok = false;
		msg.errmsg = "";
		for (unsigned long long int j = 0; j < i; j++) {
			msg.errmsg += " ";
		}
		msg.errmsg += BIAS_BLANK + "^~~  無法辨識運算符";
		return msg;
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
CommandParser::retmsg CommandParser::getvalue(string& cmd, unsigned long long& i) {
	blandslide(cmd, i);
	retmsg msg;
	if (cmd[i] == '(') {
		msg.ok = true;
		msg.description = "(";
		msg.value.IsInt = true;
		msg.value.BigIntVal = BigInt(1);
		return msg;
	}
	string res = "+";
	while (cmd[i] == '+' || cmd[i] == '-') {
		if (cmd[i] == '-' && res[0] == '-' || cmd[i] == '+' && res[0] == '+') {
			res[0] = '+';
		}
		else {
			res[0] = '-';
		}
		i++;
		blandslide(cmd, i);
	}
	bool first = true,
		isintbool = true;
	while (true) {
		if (isnumeric(cmd[i])) {
			res += cmd[i];
			first = false;
		}
		else if (cmd[i] == '.') {
			res += '.';
			isintbool = false;
		}
		else {
			break;
		}
		i++;
	}
	if (cmd[i] == '(') {
		msg.description = "(";
	}
	else if (first) {
		msg.ok = false;
		msg.errmsg = "";
		for (unsigned long long int j = 0; j < i; j++) {
			msg.errmsg += " ";
		}
		msg.errmsg += BIAS_BLANK + "^~~  須為數值";
		return msg;
	}

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
CommandParser::retmsg CommandParser::calculate(BigValue& val1, string& operatorval, BigValue& val2) {
	retmsg msg;
	if (val1.IsInt && val2.IsInt) {
		msg.ok = true;
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
		/*
		msg.ok = true;
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
		*/
	}

	return msg;
}
CommandParser::retmsg CommandParser::stagecalc(BigValue& val1) {
	retmsg msg;
	if (!val1.IsInt) {
		msg.ok = false;
		msg.description = "不支援浮點數階層運算";
		return msg;
	}
	if (val1.BigIntVal < 1) {
		val1.BigIntVal = 1;
	}
	for (BigInt i = val1.BigIntVal - 1; i > 1 && keeprun; i -= 1) {
		val1.BigIntVal *= i;
	}
	msg.ok = true;
	msg.value = val1;
	return msg;
}

void CommandParser::blandslide(string& cmd, unsigned long long& i) {
	while (i < cmd.length() && cmd[i] == ' ') {
		i++;
	}
}
bool CommandParser::isnumeric(char c) {
	if (c - '0' > 9 || c - '0' < 0) {
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