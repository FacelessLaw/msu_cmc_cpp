#pragma once

#include <iostream>
#include <string>

#include "../config.h"
#include "types.h"
#include "Lexeme.h"

class Identifier {
	std::string name; // имя идентификатора
	std::string value; // значение идентификатора
	IdentifierType type; // тип идентификатора

	bool isDeclared; // был ли он ранее объявлен

public:
	Identifier(IdentifierType type, const std::string& name, const std::string& value = "", bool isDeclared = false); // создание идентификатора

	std::string getName() const; // получение имени
	void setName(const std::string& name); // установка имени

	std::string getValue() const; // получение значения
	void setValue(const std::string& value); // установка значения
	void setDefaultValue(); // установка значения по-умолчанию

	IdentifierType getType() const; // получение типа
	void setType(IdentifierType type); // установка типа

	bool getDeclared() const; // получение флага объявленности
	void setDeclared(bool declared); // установка флага объявленности

	LexemeType toConstantLexemeType() const; // получение типа константы как лексемы
	const std::string printType() const; // печать типа идентификатора
};