CC=g++
CPPFLAGS=-Wall

build: src/planner.cpp
	$(CC) $(CPPFLAGS) -std=c++11 src/planner.cpp -o bin/plan

clean:
	rm bin/*
