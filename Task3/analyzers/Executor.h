#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

#include "../config.h"
#include "../models/Lexeme.h"
#include "../models/Identifier.h"
#include "../models/IdentifiersTable.h"

class Executor {
	IdentifiersTable identifiersTable; // таблица идентификаторов
	std::vector<Lexeme> rpn; // ПОЛИЗ
	std::stack<Lexeme> stack; // стек для выполнения

	std::string realToString(long double v) const; // перевод вещественного числа в строку
	std::string intToString(long v) const; // перевод целого числа в строку
	long double stringToReal(const std::string& v) const; // перевод строки в вещественное число
	long stringToInt(const std::string& v) const; // перевод строки в целое число

	long lexemeToInt(Lexeme& lexeme); // перевод лексемы в целое число
	long double lexemeToReal(Lexeme& lexeme); // перевод лексемы в вещественное число
	bool lexemeToBool(Lexeme& lexeme); // перевод лексемы в логический тип
	std::string lexemeToString(Lexeme& lexeme); // перевод лексемы в строку

	IdentifierType getLexemeType(Lexeme& lexeme); // получение типа лексемы

	// функции исполнения
	void executeIdentifiers(std::string& name); // обработка идентификаторов
	void executeUnaryMinus(); // выполнение унарного минуса

	void executeNot(); // выполнение логического НЕ
	void executeOr(); //  выполнение логического ИЛИ
	void executeAnd(); // выполнение логического И

	void executeAriphmetics(std::string& lexV); // выполнение арифметиических действий (+, -, *, /)
	void executeEqualities(std::string& lexV); // выполнение проверок на равенство-неравенство (==, !=)
	void executeComparsions(std::string& lexV); // выполнение сравнений (<, >, <=, >=)

	void executeGo(unsigned long& rpnIndex); // выполнение безусловоного перехода
	void executeFgo(unsigned long& rpnIndex); // выполнение перехода по лжи

	void executeRead(); // выполнение считывания
	void executeWrite(); // выполнение вывода
	void executeAssign(); // выполнение присваивания

	void printStackDump(unsigned long rpnIndex, const std::string& lexV); // печать стека

public:
	Executor(IdentifiersTable& identifiersTable, std::vector<Lexeme>& rpn); // создание исполнителя из таблицы идентификаторов и ПОЛИЗа
	void execute(int printStack); // исполнение программы
};