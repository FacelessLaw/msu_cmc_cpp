#include "SyntaxAnalyzer.h"

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(vector<Lexeme> &lexemes, IdentifiersTable& identifiersTable) {
	this->lexemes = lexemes;
	this->identifiersTable = identifiersTable;

	this->posLexeme = 0;
}

Lexeme &SyntaxAnalyzer::currLexeme() {
	return lexemes[posLexeme];
}

Lexeme &SyntaxAnalyzer::nextLexeme() {
	return lexemes[++posLexeme];
}

bool SyntaxAnalyzer::haveLexemes() {
	return currLexeme().getType() != LexemeType::null;
}

void SyntaxAnalyzer::checkLexemes() {
	bool haveUnknownLexemes = false;
	Lexeme unknownLex;

	for(auto i = lexemes.begin(); i < lexemes.end(); i++) {
		if (i->getType() == LexemeType::unknown) {
			cout << endl << "Lexical error: " << i->getValue();
			haveUnknownLexemes = true;
			unknownLex = *i;
		}
		else if (i->getType() == LexemeType::constant_real) {
			string& value = i->getValue();

			int points = 0;

			for (size_t index = 0; index < value.length(); index++) {
				if (value[index] == '.')
					points++;

				if (points > 1)
					throw Lexeme(LexemeType::constant_real, "real constant can't contain more than one point");
			}
		}
	}

	if (haveUnknownLexemes) {
		unknownLex.setValue("There are unknown lexemes in file.");
		throw unknownLex;
	}
}

void SyntaxAnalyzer::checkProgram() {
	size_t lexemesSize = lexemes.size();

	if (lexemesSize < 4)
		throw string("Fatal error: program space not found");
	
	if (!lexemes.at(0).check(LexemeType::keyword, LEX_PROGRAM) || 
		!lexemes.at(1).check(LexemeType::delimeter, LEX_LFIG_BRACKET) ||
		!lexemes.at(lexemesSize - 2).check(LexemeType::delimeter, LEX_RFIG_BRACKET))
		throw string("Fatal error: program template (") + string(LEX_PROGRAM) + string(" { ... }) is incorrect");

	// start after 'program' without {}
	lexemes.erase(lexemes.begin());
	lexemes.erase(lexemes.begin());
	lexemes.erase(lexemes.end() - 2);
}

string SyntaxAnalyzer::printIdentifierType(IdentifierType identT) {
	switch (identT) {
		case IdentifierType::identifier_int:
			return LEX_INT;

		case IdentifierType::identifier_real:
			return LEX_REAL;
		
		case IdentifierType::identifier_string:
			return LEX_STRING;
		
		case IdentifierType::identifier_boolean:
			return LEX_BOOLEAN;

		default:
			return "unknown";
	}
}

void SyntaxAnalyzer::declaration(IdentifierType identT) {
	Lexeme lexeme = nextLexeme();

	if (lexeme.getType() == LexemeType::identifier) {
		Identifier* identifier = identifiersTable.getByName(lexeme.getValue());

		if (identifier == nullptr)
			throw "Unexpected identifier. '" + lexeme.getValue() + "' does not exist in identifier table";
		
		if (identifier->getDeclared())
			throw "'" + identifier->getName() + "' is already declared";
		
		identifier->setDeclared(true);
		identifier->setType(identT);
		identifier->setDefaultValue();

		lexeme = nextLexeme();

		if (lexeme.check(LexemeType::delimeter, LEX_ASSIGN)) {
			lexeme = nextLexeme();

			string sign;

			// if initialize int constant
			if ((identT == IdentifierType::identifier_int || identT == IdentifierType::identifier_real) && 
				(lexeme.check(LexemeType::delimeter, LEX_PLUS) || lexeme.check(LexemeType::delimeter, LEX_MINUS))) {

				if (lexeme.getValue() != LEX_PLUS)
					sign = lexeme.getValue();

				lexemes.erase(lexemes.begin() + posLexeme); // delete sign lexeme
				lexeme = currLexeme();
			}

			if (!lexeme.isConstant())
				throw "Constant value expected. " + lexeme.getValue() + " is not a constant.";
			else if (lexeme.equalTypes(identT)) {
				checkEnv(lexeme);

				lexeme.setValue(sign + lexeme.getValue());
				(&(*identifier))->setValue(lexeme.getValue());
				nextLexeme();
			} else 
				throw "Type of constant value (" + lexeme.getValue() + ") doesn't match with identifier type (" + identifier->printType() + ")";
		}
	} else
		throw "Identifier name expected. " + lexeme.getValue() + " is not a name.";
}

void SyntaxAnalyzer::declarations(IdentifierType identT) {	
	declaration(identT);

	if (!currLexeme().check(LexemeType::delimeter, LEX_SEMICOLON)) {
		if (currLexeme().check(LexemeType::delimeter, LEX_COMMA))
			declarations(identT);
		else
			throw "Incorrect lexeme for declaration '" + currLexeme().getValue() + "'.";
	} else 
		nextLexeme();
}

void SyntaxAnalyzer::checkDelimeter(const string& delimeter) {
	if (!currLexeme().check(LexemeType::delimeter, delimeter))
		throw "'" + delimeter + "' expected before " + currLexeme().getValue();
}

void SyntaxAnalyzer::checkKeyword(const string& keyword) {
	if (!currLexeme().check(LexemeType::keyword, keyword))
		throw "'" + keyword + "' expected before " + currLexeme().getValue();
}

IdentifierType SyntaxAnalyzer::identifier() {
	if (currLexeme().getType() != LexemeType::identifier)
		throw "'" + currLexeme().getValue() + "' is not an identifier";
	
	Identifier* identifier = identifiersTable.getByName(currLexeme().getValue());

	if (identifier == nullptr)
		throw "Unexpected identifier. '" + currLexeme().getValue() + "' does not exist in identifier table";
	
	if (!identifier->getDeclared())
		throw "'" + currLexeme().getValue() + "' is not declared.";
	
	nextLexeme();

	return identifier->getType();
}

void SyntaxAnalyzer::assignment() {
	Lexeme leftLexeme = currLexeme();
	IdentifierType leftType = identifier();

	rpn.push_back(Lexeme(LexemeType::rpn_address, leftLexeme.getValue()));

	checkDelimeter(LEX_ASSIGN);
	nextLexeme();

	Lexeme& rightLexeme = currLexeme();
	IdentifierType rightType = expression();

	bool canAssign = false;

	if ((leftType == IdentifierType::identifier_real || leftType == IdentifierType::identifier_int) && 
		(rightType == IdentifierType::identifier_real || rightType == IdentifierType::identifier_int))
		canAssign = true;
	else 
		canAssign = leftType == rightType;

	if (!canAssign) {
		throw "'" + leftLexeme.getValue() + "' has type " + printIdentifierType(leftType) + " which is incompatible with '"
			  + rightLexeme.getValue() + "' that has type " + printIdentifierType(rightType);
	}

	rpn.push_back(Lexeme(LexemeType::delimeter, LEX_ASSIGN));

}

IdentifierType SyntaxAnalyzer::invertEntity(IdentifierType identT) {
	if (identT == IdentifierType::identifier_boolean)
		return IdentifierType::identifier_boolean;
	
	throw string("'not' can't invert non boolean expression");
}

IdentifierType SyntaxAnalyzer::overallType(IdentifierType leftType, Lexeme& sign, IdentifierType rightType) {
	LexemeType lexT = sign.getType();
	string& lexV = sign.getValue();
	
	// ariphmetic operations can be applied only on integer types (and '+' on string for concatination)
	if (sign.isAriphmeticOp()) {
		if (leftType == IdentifierType::identifier_string && lexV == LEX_PLUS) {
			if (rightType == IdentifierType::identifier_string)
				return IdentifierType::identifier_string;
			
			if (rightType == IdentifierType::identifier_boolean)
				throw string("Can't concatenate string with boolean type");
			
			return IdentifierType::identifier_string;
		}

		if (leftType == IdentifierType::identifier_real && (rightType == IdentifierType::identifier_real || rightType == IdentifierType::identifier_int)) {
			return IdentifierType::identifier_real;
		}

		if (leftType == rightType && (leftType == IdentifierType::identifier_int))
			return IdentifierType::identifier_int;
		
		if ((leftType == IdentifierType::identifier_real || leftType == IdentifierType::identifier_int) &&
			(rightType == IdentifierType::identifier_real || rightType == IdentifierType::identifier_int))
			return IdentifierType::identifier_real;
		
		throw "'" + sign.getValue() + "' incompatible with given types ('" + printIdentifierType(leftType) + "' and '" + printIdentifierType(rightType) + "')";
	}

	// boolean operations OR and AND can be applyed only on boolean types
	if (lexT == LexemeType::keyword && (lexV == LEX_OR || lexV == LEX_AND)) {
		if (leftType == rightType && leftType == IdentifierType::identifier_boolean)
			return IdentifierType::identifier_boolean;
		
		throw string("logic operation can't be applied to non-boolean variables");
	} 
	
	// ==, !=, <, >, <=, >= operations can be applied on int, real and string types (string without <=, >=)
	if (sign.isComparison()) {
		if (leftType == IdentifierType::identifier_real && (rightType == IdentifierType::identifier_real || rightType == IdentifierType::identifier_int))
			return IdentifierType::identifier_boolean;

		if (leftType == rightType && leftType == IdentifierType::identifier_int)
			return IdentifierType::identifier_boolean;

		if ((leftType == IdentifierType::identifier_real || leftType == IdentifierType::identifier_int) &&
		   (rightType == IdentifierType::identifier_real || rightType == IdentifierType::identifier_int))
			return IdentifierType::identifier_boolean;

		if (lexV != LEX_LESS_EQUAL && lexV != LEX_GREATER_EQUAL) {
			if (leftType == rightType && leftType == IdentifierType::identifier_string)
				return IdentifierType::identifier_boolean;	
		}

		throw "'" + sign.getValue() + "' incompatible with given types ('" + printIdentifierType(leftType) + "' and '" + printIdentifierType(rightType) + "')";
	}
	
	throw "'" + sign.getValue() + "' is not a proper sign";
}

IdentifierType SyntaxAnalyzer::unaryMinus() {
	LexemeType lexT = currLexeme().getType();

	if (currLexeme().isConstant()) {
		if (lexT == LexemeType::constant_real || lexT == LexemeType::constant_int) {
			rpn.push_back(Lexeme(currLexeme().getType(), currLexeme().getValue()));
			rpn.push_back(Lexeme(LexemeType::rpn_minus, LEX_MINUS));

			Lexeme& curLex = currLexeme();
			nextLexeme();

			return curLex.constantToIdentifier();
		}

		throw "unary minus is incompatible with type " + printIdentifierType(currLexeme().constantToIdentifier());
	} else if (currLexeme().getType() == LexemeType::identifier) {
		rpn.push_back(Lexeme(LexemeType::identifier, currLexeme().getValue()));

		Identifier* identifier = identifiersTable.getByName(currLexeme().getValue());

		if (identifier == nullptr)
			throw "Unexpected identifier. '" + currLexeme().getValue() + "' does not exist in identifier table";
		
		if (!identifier->getDeclared())
			throw "'" + currLexeme().getValue() + "' is not declared.";

		IdentifierType identT = identifier->getType();

		if (identT == IdentifierType::identifier_real || identT == IdentifierType::identifier_int)
			rpn.push_back(Lexeme(LexemeType::rpn_minus, LEX_MINUS));
		else 
			throw "unary minus is incompatible with type " + printIdentifierType(identT);
		
		nextLexeme();

		return identT;
	}
	else if (currLexeme().check(LexemeType::delimeter, LEX_LPARENTHESIS)) {
		nextLexeme();
		IdentifierType entT = expression();

		checkDelimeter(LEX_RPARENTHESIS);
		nextLexeme();

		rpn.push_back(Lexeme(LexemeType::rpn_minus, LEX_MINUS));

		return entT;
	}

	throw "'" + currLexeme().getValue() + "' is not an entity";	
}

IdentifierType SyntaxAnalyzer::expression() {
	IdentifierType leftType = term();

	if (currLexeme().check(LexemeType::delimeter, LEX_EQUAL) ||
		currLexeme().check(LexemeType::delimeter, LEX_GREATER) ||
		currLexeme().check(LexemeType::delimeter, LEX_LESS) ||
		currLexeme().check(LexemeType::delimeter, LEX_GREATER_EQUAL) ||
		currLexeme().check(LexemeType::delimeter, LEX_LESS_EQUAL) ||
		currLexeme().check(LexemeType::delimeter, LEX_NEQUAL)) {

		Lexeme& sign = currLexeme();
		nextLexeme();

		IdentifierType rightType = term();

		rpn.push_back(sign);

		return overallType(leftType, sign, rightType);
	}

	return leftType;
}

IdentifierType SyntaxAnalyzer::term() {
	IdentifierType leftType = term1();

	while (currLexeme().check(LexemeType::delimeter, LEX_PLUS) ||
		currLexeme().check(LexemeType::delimeter, LEX_MINUS) || 
		currLexeme().check(LexemeType::keyword, LEX_OR)) {

		Lexeme& sign = currLexeme();
		nextLexeme();
		IdentifierType rightType = term1();

		rpn.push_back(sign);

		leftType = overallType(leftType, sign, rightType);
	}

	return leftType;
}

IdentifierType SyntaxAnalyzer::term1() {
	IdentifierType leftType = entity();

	while (currLexeme().check(LexemeType::delimeter, LEX_MULT) ||
		   currLexeme().check(LexemeType::delimeter, LEX_DIV) ||
		   currLexeme().check(LexemeType::keyword, LEX_AND)) {

		Lexeme& sign = currLexeme();
		nextLexeme();
		IdentifierType rightType = entity();

		rpn.push_back(sign);

		leftType = overallType(leftType, sign, rightType);
	}

	return leftType;
}

IdentifierType SyntaxAnalyzer::entity() {
	if (currLexeme().getType() == LexemeType::identifier) {
		rpn.push_back(currLexeme());

		return identifier();
	} else if (currLexeme().isConstant()) {
		rpn.push_back(currLexeme());

		Lexeme& curLex = currLexeme();
		nextLexeme();

		return curLex.constantToIdentifier();
	}
	else if (currLexeme().check(LexemeType::keyword, LEX_NOT)) {
		nextLexeme();
		IdentifierType entT = entity();

		rpn.push_back(Lexeme(LexemeType::keyword, LEX_NOT));

		return invertEntity(entT);
	} else if (currLexeme().check(LexemeType::delimeter, LEX_LPARENTHESIS)) {
		nextLexeme();
		IdentifierType eType = expression();

		checkDelimeter(LEX_RPARENTHESIS);
		nextLexeme();

		return eType;
	} else if (currLexeme().check(LexemeType::delimeter, LEX_MINUS)) {
		nextLexeme();

		return unaryMinus();
	}
	else
		throw "'" + currLexeme().getValue() + "' is not an entity";
}

/************************************* OPERATORS PARSERS *****************************************/
void SyntaxAnalyzer::parseIf() {
	nextLexeme();

	checkDelimeter(LEX_LPARENTHESIS);	   
	nextLexeme();

	IdentifierType expT = expression();

	if (expT != IdentifierType::identifier_boolean)
		throw "'" + currLexeme().getValue() + "' expected boolean expression";

	unsigned long ifIndex = rpn.size();
	rpn.push_back(Lexeme(LexemeType::rpn_label, "0"));
	rpn.push_back(Lexeme(LexemeType::rpn_fgo, "0"));

	checkDelimeter(LEX_RPARENTHESIS);

	nextLexeme();
	parse();

	if (currLexeme().check(LexemeType::keyword, LEX_ELSE)) {
		unsigned long elseIndex = rpn.size();
		rpn.push_back(Lexeme(LexemeType::rpn_label, "0"));
		rpn.push_back(Lexeme(LexemeType::rpn_go, "0"));
		rpn[ifIndex].setValue(to_string(rpn.size()));

		nextLexeme();
		parse();

		rpn[elseIndex].setValue(to_string(rpn.size()));

		return;
	}

	rpn[ifIndex].setValue(to_string(rpn.size()));
}

void SyntaxAnalyzer::parseFor() {
	unsigned long startIndex = rpn.size();

	nextLexeme();

	checkDelimeter(LEX_LPARENTHESIS);		
	nextLexeme();

	if (!currLexeme().check(LexemeType::delimeter, LEX_SEMICOLON)) {
		assignment();
		checkDelimeter(LEX_SEMICOLON);
	}

	unsigned long expressionIndex = rpn.size();

	nextLexeme();

	if (!currLexeme().check(LexemeType::delimeter, LEX_SEMICOLON)) {
		IdentifierType expT = expression();

		unsigned long expressionLabel1 = rpn.size();
		rpn.push_back(Lexeme(LexemeType::rpn_label, "0"));
		rpn.push_back(Lexeme(LexemeType::rpn_fgo, "0"));

		if (expT != IdentifierType::identifier_boolean)
			throw "'" + currLexeme().getValue() + "' expected boolean expression";
		
		checkDelimeter(LEX_SEMICOLON);

		nextLexeme();

		vector <Lexeme> tmp;

		if (!currLexeme().check(LexemeType::delimeter, LEX_RPARENTHESIS)) {
			unsigned long assignmentIndex = rpn.size();
			assignment();
			unsigned long assignmentCount = rpn.size() - assignmentIndex;

			for (unsigned long i = 0; i < assignmentCount; i++) {
				tmp.push_back(rpn[assignmentIndex]);
				rpn.erase(rpn.begin() + assignmentIndex);
			}
		}

		checkDelimeter(LEX_RPARENTHESIS);

		nextLexeme();
		parse();
		
		rpn.insert(rpn.end(), tmp.begin(), tmp.end());
		rpn.push_back(Lexeme(LexemeType::rpn_label, to_string(expressionIndex)));
		rpn.push_back(Lexeme(LexemeType::rpn_go, "0"));
		rpn[expressionLabel1].setValue(to_string(rpn.size()));
	} else {
		nextLexeme();

		vector <Lexeme> tmp;

		if (!currLexeme().check(LexemeType::delimeter, LEX_RPARENTHESIS)) {
			unsigned long assignmentIndex = rpn.size();
			assignment();
			unsigned long assignmentCount = rpn.size() - assignmentIndex;

			for (unsigned long i = 0; i < assignmentCount; i++) {
				tmp.push_back(rpn[assignmentIndex]);
				rpn.erase(rpn.begin() + assignmentIndex);
			}
		}

		checkDelimeter(LEX_RPARENTHESIS);

		nextLexeme();
		parse();

		rpn.insert(rpn.end(), tmp.begin(), tmp.end());
		rpn.push_back(Lexeme(LexemeType::rpn_label, to_string(expressionIndex)));
		rpn.push_back(Lexeme(LexemeType::rpn_go, "0"));		
	}

	insertBreakLabels(startIndex, rpn.size(), rpn.size());
}

void SyntaxAnalyzer::parseWhile() {
	unsigned long startIndex = rpn.size();

	nextLexeme();

	checkDelimeter(LEX_LPARENTHESIS);		
	nextLexeme();

	unsigned long expressionIndex = rpn.size();

	IdentifierType expT = expression();

	unsigned long expressionLabel = rpn.size();
	rpn.push_back(Lexeme(LexemeType::rpn_label, "0"));
	rpn.push_back(Lexeme(LexemeType::rpn_fgo, "0"));

	if (expT != IdentifierType::identifier_boolean)
	  	throw "'" + currLexeme().getValue() + "' expected boolean expression";

	checkDelimeter(LEX_RPARENTHESIS);

	nextLexeme();
	parse();

	rpn.push_back(Lexeme(LexemeType::rpn_label, to_string(expressionIndex)));
	rpn.push_back(Lexeme(LexemeType::rpn_go, "0"));
	rpn[expressionLabel].setValue(to_string(rpn.size()));
	insertBreakLabels(startIndex, rpn.size(), rpn.size());
}

void SyntaxAnalyzer::parseDoWhile() {
	unsigned long startIndex = rpn.size();
	nextLexeme();
	unsigned long parseIndex = rpn.size();
	parse();

	checkKeyword(LEX_WHILE);
	nextLexeme();
	checkDelimeter(LEX_LPARENTHESIS);			
	nextLexeme();

	IdentifierType expT = expression();

	unsigned long expressionLabel = rpn.size();
	rpn.push_back(Lexeme(LexemeType::rpn_label, "0"));
	rpn.push_back(Lexeme(LexemeType::rpn_fgo, "0"));

	if (expT != IdentifierType::identifier_boolean)
		throw "'" + currLexeme().getValue() + "' expected boolean expression";

	checkDelimeter(LEX_RPARENTHESIS);
	nextLexeme();

	rpn.push_back(Lexeme(LexemeType::rpn_label, to_string(parseIndex)));
	rpn.push_back(Lexeme(LexemeType::rpn_go, "0"));
	rpn[expressionLabel].setValue(to_string(rpn.size()));
	insertBreakLabels(startIndex, rpn.size(), rpn.size());
}

void SyntaxAnalyzer::parseRead() {
	nextLexeme();

	checkDelimeter(LEX_LPARENTHESIS);
	nextLexeme();

	string address = currLexeme().getValue();
	identifier();

	rpn.push_back(Lexeme(LexemeType::rpn_address, address));
	rpn.push_back(Lexeme(LexemeType::keyword, LEX_READ));

	while (currLexeme().check(LexemeType::delimeter, LEX_COMMA)) {
		nextLexeme();
		string address1 = currLexeme().getValue();
		identifier();

		rpn.push_back(Lexeme(LexemeType::rpn_address, address1));
		rpn.push_back(Lexeme(LexemeType::keyword, LEX_READ));
	}

	checkDelimeter(LEX_RPARENTHESIS);
	nextLexeme();
}

void SyntaxAnalyzer::parseWrite() {
	nextLexeme();

	checkDelimeter(LEX_LPARENTHESIS);			
	nextLexeme();
	expression();

	rpn.push_back(Lexeme(LexemeType::keyword, LEX_WRITE));

	while (currLexeme().check(LexemeType::delimeter, LEX_COMMA)) {
		nextLexeme();		
		expression();

		rpn.push_back(Lexeme(LexemeType::keyword, LEX_WRITE));
	}

	checkDelimeter(LEX_RPARENTHESIS);
	nextLexeme();
}

void SyntaxAnalyzer::parseBreak() {
	rpn.push_back(Lexeme(LexemeType::rpn_label, "-1"));
    rpn.push_back(Lexeme(LexemeType::rpn_go, "0"));
    nextLexeme();
}

void SyntaxAnalyzer::parse() {
	if (currLexeme().check(LexemeType::delimeter, LEX_LFIG_BRACKET)) {
		nextLexeme();

		while (!currLexeme().check(LexemeType::delimeter, LEX_RFIG_BRACKET)) {
			parse();
		}

		nextLexeme();
	} else if (currLexeme().check(LexemeType::keyword, LEX_IF)) {
		parseIf();
	}
	else if (currLexeme().check(LexemeType::keyword, LEX_WHILE)) {
		parseWhile();
	}
	else if (currLexeme().check(LexemeType::keyword, LEX_FOR)) {
		parseFor();
	}
	else if (currLexeme().check(LexemeType::delimeter, LEX_SEMICOLON)) {
		nextLexeme();
	}
	else {
		if (currLexeme().check(LexemeType::keyword, LEX_DO)) {
			parseDoWhile();
		}
		else if (currLexeme().check(LexemeType::keyword, LEX_READ)) {
			parseRead();
		}
		else if (currLexeme().check(LexemeType::keyword, LEX_WRITE)) {
			parseWrite();
		}
		else if (currLexeme().check(LexemeType::keyword, LEX_BREAK)) {
			parseBreak();
		}
		else if (currLexeme().isDatatype()) { // declaration
			throw string("All declarations must be in first section");
		}
		else { // assignment
			assignment();
		}

		checkDelimeter(LEX_SEMICOLON);		
		nextLexeme();
	}
}

void SyntaxAnalyzer::insertBreakLabels(unsigned long startIndex, unsigned long endIndex, unsigned long labelIndex) {
	for (unsigned long i = startIndex; i < endIndex; i++)
		if (rpn[i].check(LexemeType::rpn_label, "-1"))
			rpn[i].setValue(to_string(labelIndex));
}

void SyntaxAnalyzer::checkEnv(Lexeme &lexeme) {
	if (lexeme.isString() && (lexeme.getValue()[0] == '$')) {
		string raw_key = lexeme.getValue();
		string key;

		for (unsigned int i = 1; i < raw_key.size(); ++i) {
			key += raw_key[i];
		}

		char *raw_value = getenv(key.c_str());
		if (raw_value) {
			string value(raw_value);
			lexeme.setValue(value);
		}
	}
}

void SyntaxAnalyzer::printIdentifiers() const {
	if (identifiersTable.size()) {
		cout << endl << "Processed identifiers after syntax analyze (" << identifiersTable.size() << "):" << endl;
		cout << identifiersTable << endl;
	}
	else 
		cout << endl << "Identifiers table is empty" << endl;
}

void SyntaxAnalyzer::printReversePolishNotation() const {
	cout << endl << "Reverse polish notation:" << endl;

	for (auto i = rpn.begin(); i < rpn.end(); i++)
		cout << i->printForRPN() << " ";

	cout << endl << endl;
}

vector<Lexeme>& SyntaxAnalyzer::getReversePolishNotation() {
	return rpn;
}

IdentifiersTable& SyntaxAnalyzer::getIdentifiersTable() {
	return identifiersTable;
}

bool SyntaxAnalyzer::analyze() {
	try {
		checkLexemes();
		checkProgram();

		while (haveLexemes() && currLexeme().isDatatype())
			declarations(currLexeme().datatypeToIdentifier());		

		while (haveLexemes())
			parse();

		for (size_t i = 0; i < rpn.size(); i++)
			if (rpn[i].check(LexemeType::rpn_label, "-1"))
				throw string("Break statement not within loop");
	}
	catch (string &e) {
		cout << endl << e << endl;

		return false;
	}
	catch (Lexeme &lex) {
		cout << endl << lex.getValue() << endl;

		return false;	
	}

	return true;
}