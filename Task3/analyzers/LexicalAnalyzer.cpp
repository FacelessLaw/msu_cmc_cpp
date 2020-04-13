#include "LexicalAnalyzer.h"

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(ifstream& sourceFile) {
	if (!sourceFile.good())
		throw string("Error: incorrect file");

	string sourceText {
		istreambuf_iterator<char>(sourceFile), istreambuf_iterator<char>()
	};

	checkSource(sourceText);

	this->progText = sourceText;
	this->charPosition = 0;
}

LexicalAnalyzer::LexicalAnalyzer(string &sourceText) {
	checkSource(sourceText);

	this->progText = sourceText;
	this->charPosition = 0;
}

void LexicalAnalyzer::checkSource(string& source) {
	unsigned long line = 1;
	string s;

	size_t i = 0;

	while (i < source.length()) {
		if (source[i] == '\n') {
			lines.push_back(s);
			s.clear();

			line++;
		}
		else if (source[i] == '\t') {
			s += "    ";
		}
		else {
			s += source[i];

			if (!source[i]) {
				source.erase(i--, 1);
				cout << "Warning: surpressed zero byte in text at line " << line << " (was deleted for interpreting)" << endl;
			}
		}

		i++;
	}

	lines.push_back(s);
}

string LexicalAnalyzer::currChar() {
	char c = progText[charPosition];

	string s;
	s += c;

	return s;
}

string LexicalAnalyzer::nextChar(bool shift) {
	charPosition++;

	string s = currChar();

	if (!shift)
		charPosition--;

	return s;
}

bool LexicalAnalyzer::isLetter(char symbol) const {
	return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z') || symbol == '_';
}

bool LexicalAnalyzer::isDigit(char symbol) const {
	return symbol >= '0' && symbol <= '9';
}

bool LexicalAnalyzer::isLetterOrDigit(char symbol) const {
	return isLetter(symbol) || isDigit(symbol);
}

bool LexicalAnalyzer::isAsDelimeter(char symbol) const {
	for (size_t i = 0; i < delimeters.size(); i++)
		if (delimeters[i][0] == symbol)
			return true;

	return false;
}

bool LexicalAnalyzer::isKeyword(string &word) const{
	return find(keywords.begin(), keywords.end(), word) < keywords.end();
}

bool LexicalAnalyzer::isDelimeter(string &word) const {
	return find(delimeters.begin(), delimeters.end(), word) < delimeters.end();
}

Lexeme LexicalAnalyzer::readFromLetter() {
	string c = currChar();
	string word;

	while (c[0] && isLetterOrDigit(c[0])) {
		word += c;
		c = nextChar();
	}

	transform(word.begin(), word.end(), word.begin(), ::tolower);

	if (word == LEX_TRUE || word == LEX_FALSE)
		return Lexeme(LexemeType::constant_boolean, word);

	if (word == LEX_INT)
		return Lexeme(LexemeType::datatype_int, word);

	if (word == LEX_REAL)
		return Lexeme(LexemeType::datatype_real, word);

	if(word == LEX_STRING)
		return Lexeme(LexemeType::datatype_string, word);

	if (word == LEX_BOOLEAN)
		return Lexeme(LexemeType::datatype_boolean, word);

	if (isKeyword(word))
		return Lexeme(LexemeType::keyword, word);
	else {
		identifiersTable.addByName(word);

		return Lexeme(LexemeType::identifier, word);
	}
}

Lexeme LexicalAnalyzer::readFromDigit() {
	string c = currChar();
	string number;

	while (c[0] && (isDigit(c[0]) || c == LEX_POINT)) {
		number += c;
		c = nextChar();
	}

	if (number.find(LEX_POINT) == string::npos)
		return Lexeme(LexemeType::constant_int, number);
	else
		return Lexeme(LexemeType::constant_real, number);
}

Lexeme LexicalAnalyzer::readFromQuote() {
	string c = nextChar();
	string s;

	while (c[0] && c != LEX_QUOTE) {
		s += c;

		if (c[0] == '\\') {
			c = nextChar();

			if (c[0])
				s += c;
		}

		c = nextChar();
	}

	if (!c[0])
		return Lexeme(LexemeType::unknown, "surprised end of file");

	nextChar();

	s.erase(remove(s.begin(), s.end(), '\n'), s.end());

	string s2;
	unsigned long i = 0, len = s.length();
	bool isOk = true;
	char c1;

	while (i < len && isOk) {
		c1 = s[i];

		if (c1 == '\\') {
			c1 = s[++i];

			switch (c1) {
				case 'n':
					s2 += '\n';
					break;

				case 't':
					s2 += '\t';
					break;

				case '\\':
				case '"':
					s2 += c1;
					break;

				default:
					isOk = false;
					break;
			}
		} else
			s2 += c1;

		i++;
	}

	if(isOk)
		return Lexeme(LexemeType::constant_string, s2);
	else
		return Lexeme(LexemeType::unknown, string("unknown screened character '\\") + c1 + "'");
}

Lexeme LexicalAnalyzer::readFromSlash() {
	string c = nextChar();

	if (c == LEX_DIV || c == LEX_MULT) {
		string comment;

		if (c == LEX_DIV) {
			c = nextChar();

			while (c[0] && c[0] != '\n') {
				comment += c; 
				c = nextChar();
			}
		}
		else {
			c = nextChar();

			while (c[0] && c + nextChar(false) != LEX_ML_COMMENT_END) {
				comment += c;
				c = nextChar();
			}

			if (!c[0])
				return Lexeme(LexemeType::unknown, "surprised end of file");

			nextChar();
			nextChar();
		}

		return Lexeme(LexemeType::comment, comment);
	}
	else
		return Lexeme(LexemeType::delimeter, LEX_DIV);
}

Lexeme LexicalAnalyzer::readDelimeter() {
	string curr = currChar();
	string next = nextChar();

	string s = curr + next;
	string c = curr;

	if (isDelimeter(s)) {
		nextChar();
		
		return Lexeme(LexemeType::delimeter, s);	
	}
	
	if (isDelimeter(c))
		return Lexeme(LexemeType::delimeter, c);
	
	return Lexeme(LexemeType::unknown, string("unknown character '") + c + string("'"));
}

Lexeme LexicalAnalyzer::getLexem() {
	string c = currChar();

	while (c[0] && (c[0] == ' ' || c[0] == '\n' || c[0] == '\t'))
		c = nextChar();

	if (!c[0])
		return Lexeme(LexemeType::null, "null");

	if (isLetter(c[0]))
		return readFromLetter();

	if (isDigit(c[0]))
		return readFromDigit();

	if (c == LEX_QUOTE)
		return readFromQuote();

	if (c == LEX_DIV)
		return readFromSlash();

	if (isAsDelimeter(c[0]))
		return readDelimeter();

	nextChar();

	return Lexeme(LexemeType::unknown, string("unknown character '") + c + '\'');	
}

vector<Lexeme>& LexicalAnalyzer::analyze() {
	Lexeme lexeme = Lexeme(LexemeType::null, "null");

	do {
		lexeme = getLexem();

		if (lexeme.getType() != LexemeType::comment)
			lexemes.push_back(lexeme);	

	} while (lexeme.getType() != LexemeType::null);

	return lexemes;
}

IdentifiersTable& LexicalAnalyzer::getIdentifiersTable() {
	return identifiersTable;
}

void LexicalAnalyzer::printText() const {
	cout << "Program text: " << endl;

	for (size_t i = 0; i < lines.size(); i++)
		cout << lines[i] << "\n";
}

void LexicalAnalyzer::printLexemes() const {
	cout << endl;
	cout << "Founded lexemes (" << (lexemes.size() - 1) << "):" << endl;

	for (size_t i = 0; i < lexemes.size(); i++)
		cout << lexemes[i] << endl;
}

void LexicalAnalyzer::printIdentifiers() const {
	if (identifiersTable.size()) {
		cout << endl << "Founded identifiers after lexical analyze (" << identifiersTable.size() << "):" << endl << identifiersTable;
	}
	else
		cout << endl << "Identifiers table after lexical analyze is empty" << endl;
}