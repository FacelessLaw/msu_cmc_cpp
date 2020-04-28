#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../config.h"
#include "../models/Lexeme.h"
#include "../models/Identifier.h"
#include "../models/IdentifiersTable.h"

class LexicalAnalyzer {
	std::string progText; // строка с текстом программы
	unsigned long charPosition; // текущая позиция символа

	IdentifiersTable identifiersTable; // таблица идентификаторов
	std::vector<Lexeme> lexemes; // вектор лексем
	std::vector<std::string> lines; // вектор строк текста

	Lexeme getLexem(); // получение очередной лексемы

	Lexeme readFromLetter(); // получение лексемы, начиная с буквы
	Lexeme readFromDigit(); // получение лексемы, начиная с цифры
	Lexeme readFromQuote(); // получение лексемы, начиная с кавычки
	Lexeme readFromSlash(); // получение лексемы, начиная со слеша
	Lexeme readFromHash(); // получение лексемы, начиная с хеша
	Lexeme readDelimeter(); // получение лексемы-разделителя

	void checkSource(std::string& source); // проверка исходного текста на некорректные символы

	std::string currChar(); // текущий символ в виде строки
	std::string nextChar(bool shift = true); // следующий символ (флаг указывает, нужно ли сдвигаться дальше)

	bool isLetter(char symbol) const; // проверка символа на букву
	bool isDigit(char symbol) const; // проверка символа на цифру
	bool isLetterOrDigit(char symbol) const; // проврека символа на букву или цифру
	bool isAsDelimeter(char symbol) const; // проверка символа на схожесть с разделителями

	bool isKeyword(std::string& word) const; // является ли слово ключевым
	bool isDelimeter(std::string& word) const; // является ли слово разделителем

public:	
	LexicalAnalyzer(std::ifstream& sourceFile); // конструктор из файлового потока
	LexicalAnalyzer(std::string& sourceText); // конструктор из строки

	std::vector<Lexeme>& analyze(); // выполнение лексического анализа
	IdentifiersTable& getIdentifiersTable(); // получение таблицы идентификаторов

	void printText() const; // печать текста
	void printLexemes() const; // печать лексем
	void printIdentifiers() const; // печать таблицы идентификаторов
};