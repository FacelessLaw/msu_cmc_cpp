FILES=modellang.cpp models/Lexeme.cpp models/Identifier.cpp models/IdentifiersTable.cpp analyzers/LexicalAnalyzer.cpp analyzers/SyntaxAnalyzer.cpp analyzers/Executor.cpp
STD=14
OPTIMIZE_LEVEL=3
FLAGS=-Wall -Wunreachable-code -pedantic

all:
	g++ $(FLAGS) -O$(OPTIMIZE_LEVEL) -std=c++$(STD) $(FILES) -o modelLang

clean:
	rm -rf modelLang