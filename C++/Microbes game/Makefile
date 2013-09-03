CXXFLAGS=-Wall -O2
all: make_library main.cpp
	g++ ${CXXFLAGS} main.cpp Game.o -o game
make_library: Game.cpp
	g++ ${CXXFLAGS} Game.cpp -c -o Game.o
clean:
	rm Game.o game
