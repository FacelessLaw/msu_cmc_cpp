#include "Identifier.h"

using namespace std;

Identifier::Identifier(IdentifierType type, const string& name, const string& value, bool declared) {
	this->type = type;
	this->name = name;
	this->value = value;
	this->isDeclared = declared;
}

string Identifier::getName() const {
	return name;
}

void Identifier::setName(const string& name) {
	this->name = name;
}

string Identifier::getValue() const {
	return value;
}

void Identifier::setValue(const string& value) {
	this->value = value;
}

void Identifier::setDefaultValue() {
	switch (type) {
		case IdentifierType::identifier_int:
		case IdentifierType::identifier_real:
			value = "0";
			break;

		case IdentifierType::identifier_boolean:
			value = LEX_FALSE;
			break;

		case IdentifierType::identifier_string:
			value = "";
			break;

		default:
			value = "";
			break;
	}
}

IdentifierType Identifier::getType() const {
	return type;
}

void Identifier::setType(IdentifierType type) {
	this->type = type;
}

bool Identifier::getDeclared() const {
	return isDeclared;
}

void Identifier::setDeclared(bool declared) {
	this->isDeclared = declared;
}

LexemeType Identifier::toConstantLexemeType() const {
	switch (type) {
		case IdentifierType::identifier_int:
			return LexemeType::constant_int;

		case IdentifierType::identifier_real:
			return LexemeType::constant_real;

		case IdentifierType::identifier_string:
			return LexemeType::constant_string;

		case IdentifierType::identifier_boolean:
			return LexemeType::constant_boolean;

		case IdentifierType::identifier_unknown:
		default:
			return LexemeType::unknown;
	}
}

const string Identifier::printType() const {
	if(type == IdentifierType::identifier_int)
		return LEX_INT;

	if(type == IdentifierType::identifier_real)
		return LEX_REAL;
	
	if (type == IdentifierType::identifier_string)
		return LEX_STRING;
	
	if (type == IdentifierType::identifier_boolean)
		return LEX_BOOLEAN;
	 
	return "unknown";
}