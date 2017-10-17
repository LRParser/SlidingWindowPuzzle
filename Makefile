CPPFLAGS = -Wall -std=c++11 -O2
OBJECTS = SlidingWindowPuzzle.o SlidingWindowSearch.o Move.o GraphSearchResult.o SlidingWindowHeuristic.o
EXEC = hw1
HEADERS = SlidingWindowPuzzle.h SlidingWindowSearch.h Move.h GraphSearchResult.h SlidingWindowHeuristic.h
SOURCES = SlidingWindowPuzzle.cpp SlidingWindowSearch.cpp Move.cpp GraphSearchResult.cpp SlidingWindowHeuristic.cpp

all: clean $(SOURCES) $(EXEC)

clean:
	rm -f $(OBJECTS) $(EXEC)

objs:
	g++  -Wall -DDEBUG -g -std=c++11 -O2  -c -o SlidingWindowPuzzle.o SlidingWindowPuzzle.cpp
	g++  -Wall -DDEBUG -g -std=c++11 -O2  -c -o SlidingWindowSearch.o SlidingWindowSearch.cpp
	g++  -Wall -DDEBUG -g -std=c++11 -O2  -c -o Move.o Move.cpp
	g++  -Wall -DDEBUG -g -std=c++11 -O2  -c -o GraphSearchResult.o GraphSearchResult.cpp
	g++  -Wall -DDEBUG -g -std=c++11 -O2  -c -o SlidingWindowHeuristic.o SlidingWindowHeuristic.cpp

$(EXEC) : objs
	g++ -DDEBUG -g $(CPPFLAGS) $(OBJECTS) -o $@
	

run: all
	./hw1
