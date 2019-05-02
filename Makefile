CC=g++
CPPFLAGS=-Wall

build: src/planner.cpp
	$(CC) $(CPPFLAGS) src/planner.cpp -o bin/plan

clean:
	rm bin/*
