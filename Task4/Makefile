FILES=main.cpp
STD=14
OPTIMIZE_LEVEL=3
FLAGS=-Wall -Wunreachable-code -pedantic -Wno-unused-result -Wno-vla -Wno-write-strings

all:
	g++ $(FLAGS) -O$(OPTIMIZE_LEVEL) -std=c++$(STD) $(FILES) -o run_server

clean:
	rm -rf run_server