#include "Lexeme.h"

using namespace std;

Lexeme::Lexeme(LexemeType type, string value) {
	this->type = type;
	this->value = value;
}

Lexeme::Lexeme(Identifier* ident) {
	this->value = ident->getValue();

	IdentifierType identT = ident->getType();

	if (identT == IdentifierType::identifier_int) {
		this->type = LexemeType::constant_int;
	}
	else if (identT == IdentifierType::identifier_real) {
		this->type = LexemeType::constant_real;	
	}
	else if (identT == IdentifierType::identifier_boolean) {
		this->type = LexemeType::constant_boolean;	
	}
	else if (identT == IdentifierType::identifier_string) {
		this->type = LexemeType::constant_string;
	}
	else
		throw string("Can't convert identifier to constant lexeme");
}

void Lexeme::setType(LexemeType type) {
	this->type = type;
}

LexemeType Lexeme::getType() const {
	return type;
}


void Lexeme::setValue(const string& value) {
	this->value = value;
}

string& Lexeme::getValue() {
	return value;
}

bool Lexeme::check(LexemeType type, const string &value) const {
	return this->type == type && this->value == value;
}

bool Lexeme::isConstant() const {
	return type == LexemeType::constant_int || type == LexemeType::constant_real || type == LexemeType::constant_string || type == LexemeType::constant_boolean;
}

bool Lexeme::isDatatype() const {
	return type == LexemeType::datatype_int || type == LexemeType::datatype_real || type == LexemeType::datatype_string || type == LexemeType::datatype_boolean;
}

bool Lexeme::isComparison() const {
	return type == LexemeType::delimeter && 
		(value == LEX_EQUAL || value == LEX_NEQUAL || 
		 value == LEX_LESS || value == LEX_GREATER ||
		 value == LEX_LESS_EQUAL || value == LEX_GREATER_EQUAL);
}

bool Lexeme::isAriphmeticOp() const {
	return type == LexemeType::delimeter && (value == LEX_PLUS || value == LEX_MINUS || value == LEX_MULT || value == LEX_DIV);
}

IdentifierType Lexeme::constantToIdentifier() const {
	if (type == LexemeType::constant_int)
		return IdentifierType::identifier_int;
	
	if (type == LexemeType::constant_real)
		return IdentifierType::identifier_real;
	
	if (type == LexemeType::constant_string)
		return IdentifierType::identifier_string;
	
	if (type == LexemeType::constant_boolean)
		return IdentifierType::identifier_boolean;
	 
	return IdentifierType::identifier_unknown;
}

IdentifierType Lexeme::datatypeToIdentifier() const {
	if (type == LexemeType::datatype_int)
		return IdentifierType::identifier_int;
	
	if (type == LexemeType::datatype_real)
		return IdentifierType::identifier_real;
	
	if (type == LexemeType::datatype_string)
		return IdentifierType::identifier_string;
	
	if (type == LexemeType::datatype_boolean)
		return IdentifierType::identifier_boolean;
	
	return IdentifierType::identifier_unknown;
}

bool Lexeme::equalTypes(IdentifierType identT) const {
	return ((type == LexemeType::constant_int || type == LexemeType::datatype_int) && (identT == IdentifierType::identifier_int || identT == IdentifierType::identifier_real)) ||
		((type == LexemeType::constant_real || type == LexemeType::datatype_real) && identT == IdentifierType::identifier_real) ||
		((type == LexemeType::constant_string || type == LexemeType::datatype_string) && identT == IdentifierType::identifier_string) ||
		((type == LexemeType::constant_boolean || type == LexemeType::datatype_boolean) && identT == IdentifierType::identifier_boolean);
}

string Lexeme::printForRPN() const {
	string out = "[";

	switch (type) {
		case LexemeType::rpn_minus:
			out += "#";			
			break;

		case LexemeType::rpn_go:
			out += "go";			
			break;

		case LexemeType::rpn_fgo:
			out += "!go";			
			break;

		case LexemeType::rpn_label:
			out += "L" + value;
			break;

		case LexemeType::rpn_address:
			out += "&" + value;
			break;

		case LexemeType::constant_int:
			out += value + " (int)";
			break;

		case LexemeType::constant_real:
			out += value + " (real)";
			break;

		case LexemeType::constant_boolean:
			out += value + " (boolean)";
			break;

		default:
			for (size_t i = 0; i < value.length(); i++)
				if (value[i] == '\n')
					out += "\\n";
				else
					out += value[i];
			break;
	}
	
	return out + "]";
}

ostream &operator<<(ostream &os, const Lexeme &lexeme) {
	os << "value: " << lexeme.value << "\ttype: ";

	switch (lexeme.type) {
		case LexemeType::keyword:
			os << "keyword";
			break;

		case LexemeType::delimeter:
			os << "delimeter";
			break;

		case LexemeType::identifier:
			os << "identifier";
			break;

		case LexemeType::constant_int:
			os << "constant (int)";
			break;

		case LexemeType::constant_real:
			os << "constant (real)";
			break;

		case LexemeType::constant_string:
			os << "constant (string)";
			break;

		case LexemeType::constant_boolean:
			os << "constant (boolean)";
			break;

		case LexemeType::datatype_int:
			os << "datatype (int)";
			break;

		case LexemeType::datatype_real:
			os << "datatype (real)";
			break;

		case LexemeType::datatype_string:
			os << "datatype (string)";
			break;

		case LexemeType::datatype_boolean:
			os << "datatype (boolean)";
			break;

		case LexemeType::comment:
			os << "comment";
			break;

		case LexemeType::null:
			os << "null";
			break;

		case LexemeType::rpn_go:
			os << "go";
			break;

		case LexemeType::rpn_fgo:
			os << "!go";
			break;

		case LexemeType::rpn_label:
			os << "label";
			break;

		case LexemeType::rpn_address:
			os << "address";
			break;

		case LexemeType::rpn_minus:
			os << "#";
			break;

		case LexemeType::unknown:
		default:
			os << "unknown";
			break;
	}

    return os;
}