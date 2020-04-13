#pragma once

#include <iostream>
#include <string>

#include "../config.h"
#include "types.h"
#include "Identifier.h"

class Lexeme {
	LexemeType type; // тип лексемы
	std::string value; // значение лексемы

public:
	Lexeme(LexemeType type = LexemeType::unknown, std::string value = ""); // создание лексемы
	Lexeme(class Identifier* ident); // создание лексемы из идентификатора

	void setType(LexemeType type = LexemeType::unknown); // установка типа
	LexemeType getType() const; // получение типа

	void setValue(const std::string& value); // установка значения
	std::string& getValue(); // получение значения

	bool check(LexemeType type, const std::string& value) const; // проверка на соответствие типа и значения

	bool isConstant() const; // является ли лексема константой
	bool isDatatype() const; // является ли лексема объявлением типа

	bool isComparison() const; // является ли лексема символом сравнения
	bool isAriphmeticOp() const; // является ли лексема арифметической операцией

	IdentifierType constantToIdentifier() const; // получение типа идентификатора из константной лексемы
	IdentifierType datatypeToIdentifier() const; // получение типа идентификатора из объявления типа

	bool equalTypes(IdentifierType identT) const; // проверка на одинаковость типов

	std::string printForRPN() const; // получение строки для RPN

	friend std::ostream &operator<<(std::ostream &stream, const Lexeme &lexeme); // вывод лексемы в стандратный поток
};