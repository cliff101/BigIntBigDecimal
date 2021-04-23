#include "CommandParser.h"

CommandParser::CommandParser() {
	BigIntVar = map<string, BigInt>();
	BigDecimalVar = map<string, BigDecimal>();
}

void CommandParser::Inputcommand(string in) {
	unsigned long long i = 0;
	retmsg msg = ProceedCommand(in, i, 8);
	if (!msg.ok) {
		cout << msg.errmsg << endl;
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
1 (
2 ! 由左至右
3 ^ 由右至左
4 +- (正負號) 由右至左
5 * / 由左至右
6 + - 由左至右
7 )
8 (empty)
*/
CommandParser::retmsg CommandParser::ProceedCommand(string& cmd, unsigned long long& i, int prevpriority,bool isgiveval1, BigValue giveval1) {
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
			i++;
			msg = ProceedCommand(cmd, i, 1);//return時，i停留在operator上
			if (!msg.ok) {
				return msg;
			}
			value1 = msg.value;
		}
		else {
			value1 = msg.value;
		}
	}
	else {
		value1 = giveval1;
	}

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
		cmd.insert(cmd.begin() + i, '*');
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
			msg.errmsg = msg.errmsg = "位置： " + to_string(i) + msg.errmsg;
			return msg;
		}
		else {
			value1 = msg.value;
		}
		i++;
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
			cmd.insert(cmd.begin() + i, '*');
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
			msg.ok = false;
			msg.errmsg = msg.errmsg = "位置： " + to_string(i) + "  不支援連續階層";;
			return msg;
		}
		else {
			operatorval = msg.description;
			thispriority = msg.priority;
		}
	}
	else {
		operatorval = msg.description;
		thispriority = msg.priority;
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
			return msg2;
		}
		value1 = msg2.value;
		if (msg.description == "endofcmd") {
			return msg2;
		}
		msg = ProceedCommand(cmd, i, 8, true, value1);
		return msg;
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
	if (cmd[i] != '+' && cmd[i] != '-' && cmd[i] != '*' && cmd[i] != '/' && cmd[i] != '^' && cmd[i] != '!' && cmd[i] != '(') {
		msg.ok = false;
		msg.errmsg = "位置： "+to_string(i)+"  無法辨認運算符";
		return msg;
	}
	if (cmd[i] == '(') {
		msg.priority = 1;
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
	if (first) {
		msg.ok = false;
		msg.errmsg = "位置： " + to_string(i) + "  不是數字";
		return msg;
	}
	msg.ok = true;
	if (isintbool) {
		msg.value.IsInt = true;
		msg.value.BigIntVal = BigInt(res);
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
		msg.value.BigIntVal = val1.BigIntVal + val2.BigIntVal;
	}
	return msg;
}
CommandParser::retmsg CommandParser::stagecalc(BigValue& val1) {
	retmsg msg;
	if (!val1.IsInt) {
		msg.ok = false;
		msg.description = "  不支援符點數階層運算";
		return msg;
	}
	for (BigInt i = val1.BigIntVal - 1; i > 1; i -= 1) {
		val1.BigIntVal *= i;
	}
	msg.ok = true;
	msg.value = val1;
	return msg;
}

void CommandParser::blandslide(string& cmd, unsigned long long& i) {
	while (cmd[i] == ' ') {
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