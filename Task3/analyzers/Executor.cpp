#include "Executor.h"

using namespace std;

Executor::Executor(IdentifiersTable& identifiersTable, vector<Lexeme>& rpn) {
	this->identifiersTable = identifiersTable;
	this->rpn = rpn;
}

string Executor::realToString(long double v) const {
	char res[50];
	int len = sprintf(res, "%.16Lf", v);

	do {
		len--;
	} while (len && res[len] != '.' && res[len] == '0');

	if (len)
		res[len + !(res[len] == '.')] = '\0';

	return res;
}

string Executor::intToString(long v) const {
	char res[50];
	sprintf(res, "%ld", v);

	return res;
}

long double Executor::stringToReal(const string& v) const {
	long double a;
	sscanf(v.c_str(), "%Lf", &a);

	return a;
}

long Executor::stringToInt(const string& v) const {
	long a;
	sscanf(v.c_str(), "%ld", &a);

	return a;
}

long Executor::lexemeToInt(Lexeme& lexeme) {
	LexemeType lexT = lexeme.getType();

	if (lexT == LexemeType::constant_int || lexT == LexemeType::rpn_label)
		return stringToInt(lexeme.getValue());
	
	throw string("Expected int lexeme");
}

long double Executor::lexemeToReal(Lexeme& lexeme) {
	LexemeType lexT = lexeme.getType();

	if (lexT == LexemeType::constant_real || lexT == LexemeType::constant_int)
		return stringToReal(lexeme.getValue());
	
	throw string("Expected real lexeme");
}

bool Executor::lexemeToBool(Lexeme& lexeme) {
	if (lexeme.getType()== LexemeType::constant_boolean)
		return lexeme.getValue() == LEX_TRUE;

	throw string("Expected boolean lexeme");
}

string Executor::lexemeToString(Lexeme& lexeme) {
	if (lexeme.getType() == LexemeType::constant_string)
		return lexeme.getValue();

	throw string("Expected string lexeme");
}

IdentifierType Executor::getLexemeType(Lexeme& lexeme) {
	LexemeType lexT = lexeme.getType();

	switch (lexT) {
		case LexemeType::constant_int:
			return IdentifierType::identifier_int;

		case LexemeType::constant_real:
			return IdentifierType::identifier_real;

		case LexemeType::constant_string:
			return IdentifierType::identifier_string;

		case LexemeType::constant_boolean:
			return IdentifierType::identifier_boolean;

		case LexemeType::identifier:
		case LexemeType::rpn_address:
			return identifiersTable.getByName(lexeme.getValue())->getType();

		default:
			return IdentifierType::identifier_unknown;
	}
}

void Executor::executeIdentifiers(string& name) {
	Identifier* ident = identifiersTable.getByName(name);

	if (ident != nullptr)
		stack.push(Lexeme(ident));
	else
		throw string("Undefined identifier");
}

void Executor::executeUnaryMinus() {
	string& v = stack.top().getValue();

	if (v.find(LEX_MINUS) == string::npos)
		v = LEX_MINUS + v;
	else
		v.erase(0, 1);

	stack.top().setValue(v);
}

void Executor::executeNot() {
	Lexeme& arg = stack.top();

	arg.setValue(lexemeToBool(arg) ? LEX_FALSE : LEX_TRUE);
}

void Executor::executeOr() {
	Lexeme arg1 = stack.top();
	stack.pop();

	Lexeme arg2 = stack.top();
	stack.pop();

	stack.push(Lexeme(LexemeType::constant_boolean, (lexemeToBool(arg1) || lexemeToBool(arg2)) ? LEX_TRUE : LEX_FALSE));
}

void Executor::executeAnd() {
	Lexeme arg1 = stack.top();
	stack.pop();

	Lexeme arg2 = stack.top();
	stack.pop();

	stack.push(Lexeme(LexemeType::constant_boolean, (lexemeToBool(arg1) && lexemeToBool(arg2)) ? LEX_TRUE : LEX_FALSE));
}

void Executor::executeAriphmetics(string& lexV) {
	Lexeme arg1 = stack.top();
	stack.pop();

	Lexeme arg2 = stack.top();
	stack.pop();

	IdentifierType t1 = getLexemeType(arg1);
	IdentifierType t2 = getLexemeType(arg2);	

	if (t2 == IdentifierType::identifier_string) {
		string& res = arg2.getValue();
		res += arg1.getValue();

		stack.push(Lexeme(LexemeType::constant_string, res));
	}
	else if (t1 == IdentifierType::identifier_real || t2 == IdentifierType::identifier_real) {
		long double v1 = lexemeToReal(arg1);
		long double v2 = lexemeToReal(arg2);
		long double res;

		if (lexV == LEX_PLUS)
			res = v2 + v1;
		else if (lexV == LEX_MINUS)
			res = v2 - v1;
		else if (lexV == LEX_MULT)
			res = v2 * v1;
		else if (v1 != 0)
			res = v2 / v1;
		else
			throw string("Division by zero");

		stack.push(Lexeme(LexemeType::constant_real, realToString(res)));
	}
	else if (t2 == IdentifierType::identifier_int) {
		long v1 = lexemeToInt(arg1);
		long v2 = lexemeToInt(arg2);
		long res;

		if (lexV == LEX_PLUS)
			res = v2 + v1;
		else if (lexV == LEX_MINUS)
			res = v2 - v1;
		else if (lexV == LEX_MULT)
			res = v2 * v1;
		else if (v1 != 0)
			res = (lexV == LEX_DIV) ? (v2 / v1) : (v2 % v1);
		else
			throw string("Division by zero");

		stack.push(Lexeme(LexemeType::constant_int, intToString(res)));
	}
	else
		throw string("Unknown lexeme at ariphmetics process");
}

void Executor::executeEqualities(string& lexV) {
	Lexeme arg1 = stack.top();
	stack.pop();

	Lexeme arg2 = stack.top();
	stack.pop();

	IdentifierType t1 = getLexemeType(arg1);
	IdentifierType t2 = getLexemeType(arg2);

	bool res;

	if ((t1 == IdentifierType::identifier_real || t1 == IdentifierType::identifier_int) &&
		(t2 == IdentifierType::identifier_real || t2 == IdentifierType::identifier_int)) {
		long double v1 = lexemeToReal(arg1), v2 = lexemeToReal(arg2);

		res = (lexV == LEX_EQUAL) ? (v1 == v2) : (v1 != v2);
	}
	else
		res = (lexV == LEX_EQUAL) ? (arg1.getValue() == arg2.getValue()) : (arg1.getValue() != arg2.getValue());

	stack.push(Lexeme(LexemeType::constant_boolean, res ? LEX_TRUE : LEX_FALSE));
}

void Executor::executeComparsions(string& lexV) {
	Lexeme arg1 = stack.top();
	stack.pop();

	Lexeme arg2 = stack.top();
	stack.pop();

	bool res = false;

	IdentifierType t1 = getLexemeType(arg1);
	IdentifierType t2 = getLexemeType(arg2);	

	if (t1 == IdentifierType::identifier_real || t2 == IdentifierType::identifier_real) {
		long double v1 = lexemeToReal(arg1);
		long double v2 = lexemeToReal(arg2);

		if (lexV == LEX_LESS)
			res = v2 < v1;
		else if (lexV == LEX_LESS_EQUAL)
			res = v2 <= v1;
		else if (lexV == LEX_GREATER)
			res = v2 > v1;
		else
			res = v2 >= v1;		
	}
	else if (t1 == IdentifierType::identifier_int) {
		long v1 = lexemeToInt(arg1);
		long v2 = lexemeToInt(arg2);

		if (lexV == LEX_LESS)
			res = v2 < v1;
		else if (lexV == LEX_LESS_EQUAL)
			res = v2 <= v1;
		else if (lexV == LEX_GREATER)
			res = v2 > v1;
		else
			res = v2 >= v1;
	}
	else if (t1 == IdentifierType::identifier_string) {
		if (lexV == LEX_LESS)
			res = lexemeToString(arg2) < lexemeToString(arg1);
		else
			res = lexemeToString(arg2) > lexemeToString(arg1);
	}

	stack.push(Lexeme(LexemeType::constant_boolean, res ? LEX_TRUE : LEX_FALSE));
}

void Executor::executeGo(unsigned long& rpnIndex) {
	rpnIndex = lexemeToInt(stack.top()) - 1;
	stack.pop();
}

void Executor::executeFgo(unsigned long& rpnIndex) {
	long i = lexemeToInt(stack.top());
	stack.pop();

	Lexeme arg2 = stack.top();
	stack.pop();

	if (!lexemeToBool(arg2))
		rpnIndex = i - 1;
}

void Executor::executeRead() {
	Identifier* ident = identifiersTable.getByName(stack.top().getValue());
	IdentifierType identT = ident->getType();
	stack.pop();

	if (identT == IdentifierType::identifier_real) {
		long double r;
		cin >> r;
		cin.ignore(); // skip '\n' character

		ident->setValue(realToString(r));
	}
	else if (identT == IdentifierType::identifier_int) {
		long k;
		cin >> k;
		cin.ignore(); // skip '\n' character

		ident->setValue(intToString(k));
	}
	else if (identT == IdentifierType::identifier_string) {
		string s;
		getline(cin, s);

		ident->setValue(s);
	}
	else {
		string s;
		getline(cin, s);

		transform(s.begin(), s.end(), s.begin(), ::tolower);

		ident->setValue(s == LEX_TRUE ? LEX_TRUE : LEX_FALSE);
	}
}

void Executor::executeWrite() {
	if (stack.top().getType() == LexemeType::constant_real)
		printf("%.16Lf", stringToReal(stack.top().getValue()));
	else
		cout << stack.top().getValue();

	stack.pop();
}

void Executor::executeAssign() {
	Lexeme arg1 = stack.top();
	stack.pop();	

	Identifier* ident = identifiersTable.getByName(stack.top().getValue());
	IdentifierType identT = ident->getType();
	stack.pop();

	if (identT == IdentifierType::identifier_int && getLexemeType(arg1) == IdentifierType::identifier_real)
		ident->setValue(intToString((long) lexemeToReal(arg1)));
	else
		ident->setValue(arg1.getValue());
}

void Executor::printStackDump(unsigned long rpnIndex, const string& lexV) {
	std::stack<Lexeme> dump = stack;

	cout << endl << endl << "+----------------------------------------------------------------------------------------------------+" << endl;
	string out = string("Stack (index: ") + to_string((signed long) rpnIndex) + ", current lexeme: " + lexV + ")";
	cout << "|" << string((100 - out.length()) / 2, ' ') << out << string((101 - out.length()) / 2, ' ') << "|" << endl;
	cout << "+----------------------------------------------------------------------------------------------------+" << endl;

	if (dump.empty())
		cout << "|                                           stack is empty                                           |" << endl;
	else
		while (!dump.empty()) {
			out = dump.top().printForRPN();

			cout << "|" << string((100 - out.length()) / 2, ' ') << out << string((101 - out.length()) / 2, ' ') << "|" << endl;

			dump.pop();
		}

	cout << "+----------------------------------------------------------------------------------------------------+" << endl;
}

void Executor::execute(int printStack) {
	try {
		unsigned long rpnIndex = 0;
		unsigned long size = rpn.size();

		while (rpnIndex < size) {
			Lexeme& lex = rpn[rpnIndex];
			string& lexV = lex.getValue();

			switch (lex.getType()) {
				case LexemeType::constant_int:
				case LexemeType::constant_real:
				case LexemeType::constant_string:
				case LexemeType::constant_boolean:
				case LexemeType::rpn_address:
				case LexemeType::rpn_label:
					stack.push(lex);
					break;

				case LexemeType::identifier:
					executeIdentifiers(lexV);
					break;

				case LexemeType::rpn_minus:
					executeUnaryMinus();
					break;


				case LexemeType::keyword:
					if (lexV == LEX_NOT)
						executeNot();
					else if (lexV == LEX_OR)
						executeOr();
					else if (lexV == LEX_AND)
						executeAnd();
					else if (lexV == LEX_WRITE)
						executeWrite();
					else if (lexV == LEX_READ)
						executeRead();
					break;

				case LexemeType::delimeter:
					if (lex.isAriphmeticOp()) {
						executeAriphmetics(lexV);
					}
					else if (lexV == LEX_EQUAL || lexV == LEX_NEQUAL) {
						executeEqualities(lexV);
					}
					else if (lexV == LEX_LESS || lexV == LEX_GREATER || lexV == LEX_LESS_EQUAL || lexV == LEX_GREATER_EQUAL) {
						executeComparsions(lexV);
					}
					else if (lexV == LEX_ASSIGN) {
						executeAssign();					
					}
					break;

				case LexemeType::rpn_fgo:
					executeFgo(rpnIndex);					
					break;

				case LexemeType::rpn_go:
					executeGo(rpnIndex);
					break;

				default:
					break;
			}

			if (printStack) {
				printStackDump(rpnIndex, lex.printForRPN());
				cout << endl << identifiersTable << endl;

				if (printStack > 1) {
					cout << "Press enter for next step";
					getchar();
				}
			}

			rpnIndex++;
		}
	}
	catch (string e) {
		cout << "Execution error: " << e << endl;
	}
}