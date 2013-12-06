all: flex bison build

flex:
	flex -o "lex.yy.cpp" java.l

bison:
	bison -o java.tab.cpp -d java.y

build: 
	g++ lex.yy.cpp java.tab.cpp syntax_engine.cpp -std=c++0x
