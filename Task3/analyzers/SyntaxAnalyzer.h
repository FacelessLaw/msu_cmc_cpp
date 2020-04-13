#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../config.h"
#include "../models/Lexeme.h"
#include "../models/Identifier.h"
#include "../models/IdentifiersTable.h"

class SyntaxAnalyzer {
	std::vector<Lexeme> lexemes; // вектор лексем
	IdentifiersTable identifiersTable; // таблица идентификаторов
	std::vector<Lexeme> rpn; // вектор ПОЛИЗа

	size_t posLexeme; // текущая позиция лексемы

	Lexeme& currLexeme(); // текущая лексема
	Lexeme& nextLexeme(); // следующая лексема
	bool haveLexemes(); // наличие лексем

	void checkLexemes(); // проверка лексем на наличие некорректных
	void checkProgram(); // проверка шаблона программы

	std::string printIdentifierType(IdentifierType identT); // печать типа идентификатора

	void declaration(IdentifierType identT); // обработка одного объявления типа
	void declarations(IdentifierType identT); // обработка объявлений типов

	void checkDelimeter(const std::string& delimeter); // проверка на разделитель
	void checkKeyword(const std::string& keyword); // проверка на ключевое слово

	IdentifierType identifier(); // обработка идентификатора
	void assignment(); // обработка присваивания

	IdentifierType invertEntity(IdentifierType identT); // обработка инвертирования
	IdentifierType overallType(IdentifierType leftType, Lexeme& sign, IdentifierType rightType); // обработка операции и используемых типов

	IdentifierType unaryMinus(); // обработка унарного минуса

	 // обработка выражения
	IdentifierType expression();
	IdentifierType term();
	IdentifierType term1();
	IdentifierType entity();

	void insertBreakLabels(unsigned long startIndex, unsigned long endIndex, unsigned long labelIndex); // вставка флагов разрыва

	void parseIf(); // парсинг условоного оператора
	void parseFor(); // парсинг цикла for
	void parseWhile(); // парсинг цикла while
	void parseDoWhile(); // парсинг цикла do-while
	void parseRead(); // парсинг функци read
	void parseWrite(); // парсинг функции write
	void parseBreak(); // парсинг оператора break

	void parse(); // парсинг лексем

public:
	SyntaxAnalyzer(std::vector<Lexeme> &lexemes, IdentifiersTable &identifiersTable); // создание анализатора ил лексем и таблицы идентификаторов

	void printIdentifiers() const; // печать таблицы идентификаторов
	void printReversePolishNotation() const; // печать ПОЛИЗа

	std::vector<Lexeme>& getReversePolishNotation(); // получение ПОЛИЗа
	IdentifiersTable& getIdentifiersTable(); // получение таблицы идентификаторов

	bool analyze(); // синтаксический анализ
};