#include <iostream>

#include "config.h"
#include "analyzers/LexicalAnalyzer.h"
#include "analyzers/SyntaxAnalyzer.h"
#include "analyzers/Executor.h"

using namespace std;
using namespace std::literals;

const int STATUS_OK = 0;
const int STATUS_INCORRECT_ARGS = 1;
const int STATUS_NO_LEXEMES = 2;
const int STATUS_SYNTAX_ERRORS = 3;

// процедура для вывода сообщений об использовании и информации о языке
void help() {
	cout << "Usage:" << endl;
	cout << "  ./modelLang [options] file" << endl << endl;

	cout << "Options:" << endl;
	cout << optPrintText << "' - print interpreted text (your program)" << endl;
	cout << optPrintLexemes << "' - print processed lexemes" << endl;
	cout << optPrintIdentifiers << "' - print processed in lexical and syntax alalyze table of identifiers" << endl;
	cout << optPrintRpn << "' - print reverse polish notation of code" << endl;
	cout << optPrintStack << "' - print stack during code execution" << endl;
	cout << optPrintStackStepByStep << "' - print stack during code execution step by step" << endl;
	cout << optPrintAll << "' - print all available information" << endl;
	cout << optExecute << "' - execute interpreted program" << endl;
	cout << endl;

	cout << endl << "Additional information about language and syntax rules:" << endl << endl;

	cout << "Data types:" << endl;
	cout << "    '" << LEX_INT << "' - integer data type" << endl;
	cout << "    '" << LEX_REAL << "' - real data type" << endl;
	cout << "    '" << LEX_BOOLEAN << "' - boolean data type" << endl;
	cout << "    '" << LEX_STRING << "' - string data type" << endl;
	cout << endl;

	cout << "Keywords:" << endl;
	cout << "    '" << LEX_PROGRAM << "' - template keyword for source" << endl;
	cout << endl;
	cout << "    '" << LEX_READ << "' - read to identifier(s) values from standart input stream" << endl;
	cout << "    '" << LEX_WRITE << "' - write expression(s) to standart output stream" << endl;
	cout << endl;
	cout << "    '" << LEX_IF << "' - conditional operator (if)" << endl;
	cout << "    '" << LEX_ELSE << "' - else branch of conditional operator (else)" << endl;
	cout << endl;
	cout << "    '" << LEX_WHILE << "' - cycle operator (while)" << endl;
	cout << "    '" << LEX_DO << "' - cycle operator (do)" << endl;
	cout << "    '" << LEX_FOR << "' - cycle operator (for)" << endl;
	cout << endl;

	cout << "    '" << LEX_FALSE << "' - boolean constant (false)" << endl;
	cout << "    '" << LEX_TRUE << "' - boolean constant (true)" << endl;
	cout << endl;

	cout << "Ariphmetics operations:" << endl;
	cout << "    '" << LEX_PLUS << "' - addition operation (only for integer, real and string data types)" << endl;
	cout << "    '" << LEX_MINUS << "' - subtraction operation (only for integer and real data types)" << endl;
	cout << "    '" << LEX_MULT << "' - multiplication operation (only for integer and real data types)" << endl;
	cout << "    '" << LEX_DIV << "' - division operation (only for integer and real data types)" << endl;
	cout << endl;

	cout << "Logical operations:" << endl;
	cout << "    '" << LEX_NOT << "' - not operation (only for boolean data type)" << endl;
	cout << "    '" << LEX_AND << "' - and operation (only for boolean data type)" << endl;
	cout << "    '" << LEX_OR << "' - or operation (only for boolean data type)" << endl;
	cout << endl;

	cout << "Comparison operations:" << endl;
	cout << "    '" << LEX_EQUAL << "' - operation 'equal'" << endl;
	cout << "    '" << LEX_NEQUAL << "' - operation 'not equal'" << endl;
	cout << "    '" << LEX_LESS << "' - operation 'less' (only for integer, real and string data types)" << endl;
	cout << "    '" << LEX_GREATER << "' - operation 'greater' (only for integer, real and string data types)" << endl;
	cout << "    '" << LEX_LESS_EQUAL << "' - operation 'less or equal' (only for integer and real data types)" << endl;
	cout << "    '" << LEX_GREATER_EQUAL << "' - operation 'greater ot equal' (only for integer and real data types)" << endl;
	cout << endl;

	cout << "Template of the interpreted program (source code):" << endl;
	cout << "  " << LEX_PROGRAM << " {" << endl << "      [declaration(s)]" << endl << "      [operator(s)]" << endl << "  }" << endl;
	cout << endl;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		cout << "Incorrect arguments of program" << endl << "use --help for usage" << endl; // если аргументов нет, сообщаем об этом и предалагем воспользоваться помощью

		return STATUS_INCORRECT_ARGS;
	}

	// если аргументов ровно два и при этом это вызов помощи
	if (argc == 2 && argv[1] == "--help"s) {
		help(); // выводим информацию

		return STATUS_OK;
	}

	string path; // путь к файлу для интерпретации

	int printStack = 0; // печатать ли информацию о стеке
	bool printText = false; // печатать ли текст программы
	bool printLexemes = false; // печатать ли лексемы
	bool printIdentifiers = false; // печатать ли таблицу идентификатоорв
	bool printRPN = false; // печатать ли ПОЛИЗ
	bool execute = false; // выполнять ли программу в случае успеха

	// обрабатываем ключи, которые ввёл пользователь
	for (int i = 1; i < argc; i++) {
		if (optPrintText == argv[i]) {
			printText = true;
		}
		else if (optPrintLexemes == argv[i]) {
			printLexemes = true;
		}
		else if (optPrintIdentifiers == argv[i]) {
			printIdentifiers = true;
		}
		else if (optPrintRpn == argv[i]) {
			printRPN = true;
		}
		else if (optPrintStack == argv[i]) {
			printStack = 1;
		}
		else if (optPrintStackStepByStep == argv[i]) {
			printStack = 2;
		}
		else if (optPrintAll == argv[i]) {
			printText = printLexemes = printIdentifiers = printRPN = true;
			printStack = 1;
		}
		else if (optExecute == argv[i]) {
			execute = true;
		}
		else if (path.empty()) {
			path = argv[i];
		}
		else { // не один из аргументов не подошёл, значит он некорректен
			cout << "incorrect argument: " << argv[i] << endl;
			cout << "use --help for usage" << endl;

			return STATUS_INCORRECT_ARGS;
		}
	}

	if (path.empty()) {
		path = argv[0];
	}

	try {
		ifstream sourceFile(path); // создаём файловый поток
		LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer(sourceFile); // создаём лексический анализатор из файлового потока

		vector<Lexeme>& lexemes = lexicalAnalyzer.analyze(); // производим лексический анализ, получая лексемы

		// если лексем меньше двух, значит не нашли ничего
		if (lexemes.size() < 2) {
			cout << "No lexemes founded" << endl;
			return STATUS_NO_LEXEMES;
		}

		if (printText)
			lexicalAnalyzer.printText(); // если используется ключ печатати текста, печатаем текст программы

		if (printLexemes)
			lexicalAnalyzer.printLexemes(); // если используется ключ печати лексем, печатаем лексемы

		if (printIdentifiers)
			lexicalAnalyzer.printIdentifiers(); // если используется ключ печати таблицы идентификаторов, печатаем таблицу идентификаторов

		SyntaxAnalyzer syntaxAnalyzer(lexemes, lexicalAnalyzer.getIdentifiersTable()); // создаём синтаксический анализатор из лексем и таблицы идентификаторов

		// производим анализ
		if (!syntaxAnalyzer.analyze())
			return STATUS_SYNTAX_ERRORS; // если анализ не удался, выходим

		if (printIdentifiers)
			syntaxAnalyzer.printIdentifiers(); // если используется ключ печати таблицы идентификаторов, печатаем таблицу идентификаторов

		if (printRPN)
			syntaxAnalyzer.printReversePolishNotation(); // если используется ключ печати ПОЛИЗА, печатаем ПОЛИЗ

		//  // если используется ключ выполнения программы
		if (execute) {
			Executor executor(syntaxAnalyzer.getIdentifiersTable(), syntaxAnalyzer.getReversePolishNotation()); // создаём исполнителя из таблицы идентификаторов и ПОЛИЗа

			executor.execute(printStack); // выполняем программу
		}
	}
	catch (string s) {
		cout << s << endl; // обрабатываем исключения путём вывода сообщенияс ошибкой
	}
}