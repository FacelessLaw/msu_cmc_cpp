#pragma once

#include <vector>
#include <string>

// Опции для управления интерпретатором
const std::string optPrintText = "-t"; // печатать ли текст программы
const std::string optPrintLexemes = "-l"; // печатать ли лексемы
const std::string optPrintIdentifiers = "-i"; // печатать ли таблицу идентификаторов
const std::string optPrintRpn = "-r"; // печатать ли ПОЛИЗ
const std::string optPrintStack = "-s"; // печатать ли состояния стека
const std::string optPrintStackStepByStep = "-ss"; // печатать ли стек с пошаговым выполнением
const std::string optPrintAll = "-a"; // выбор всех вышеперечисленных флагов
const std::string optExecute = "-e"; // выполнять ли программу в случае успеха

// ключевые слова языка
const std::string LEX_PROGRAM = "program";

const std::string LEX_READ = "read";
const std::string LEX_WRITE = "write";

const std::string LEX_NOT = "not";
const std::string LEX_AND = "and";
const std::string LEX_OR = "or";

const std::string LEX_IF = "if";
const std::string LEX_ELSE = "else";

const std::string LEX_FOR = "for";
const std::string LEX_WHILE = "while";
const std::string LEX_DO = "do";

const std::string LEX_BREAK = "break";

const std::string LEX_FALSE = "false";
const std::string LEX_TRUE = "true";

const std::string LEX_INT = "int";
const std::string LEX_BOOLEAN = "boolean";
const std::string LEX_STRING = "string";
const std::string LEX_REAL = "real";

// разделители
const std::string LEX_ML_COMMENT_BEGIN = "/*";
const std::string LEX_ML_COMMENT_END = "*/";
const std::string LEX_QUOTE = "\"";

const std::string LEX_SEMICOLON = ";";
const std::string LEX_POINT = ".";
const std::string LEX_COMMA = ",";

const std::string LEX_LFIG_BRACKET = "{";
const std::string LEX_RFIG_BRACKET = "}";

const std::string LEX_LPARENTHESIS = "(";
const std::string LEX_RPARENTHESIS = ")";

const std::string LEX_PLUS = "+";
const std::string LEX_MINUS = "-";
const std::string LEX_MULT = "*";
const std::string LEX_DIV = "/";

const std::string LEX_ASSIGN = "=";
const std::string LEX_EQUAL = "==";
const std::string LEX_NEQUAL = "!=";
const std::string LEX_LESS = "<";
const std::string LEX_GREATER = ">";
const std::string LEX_LESS_EQUAL = "<=";
const std::string LEX_GREATER_EQUAL = ">=";

// вектор ключевых слов
const std::vector <std::string> keywords = {
	LEX_PROGRAM,
	LEX_READ, LEX_WRITE,
	LEX_NOT, LEX_AND, LEX_OR,
	LEX_IF, LEX_ELSE,
	LEX_FOR, LEX_WHILE, LEX_DO,
	LEX_BREAK,
	LEX_FALSE, LEX_TRUE
};

// вектор разделителей
const std::vector <std::string> delimeters = {
	LEX_ML_COMMENT_BEGIN, LEX_ML_COMMENT_END, LEX_QUOTE, 
	
	LEX_ASSIGN,
	
	LEX_SEMICOLON, LEX_COMMA,
	
	LEX_LFIG_BRACKET, LEX_RFIG_BRACKET, LEX_LPARENTHESIS, LEX_RPARENTHESIS,
	
	LEX_PLUS, LEX_MINUS, LEX_DIV, LEX_MULT, 
	
	LEX_EQUAL, LEX_NEQUAL, LEX_LESS, LEX_GREATER, LEX_LESS_EQUAL, LEX_GREATER_EQUAL,
};