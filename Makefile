CC=g++
CPPFLAGS=-Wall

build: src/planner.cpp src/plan.h bin
	$(CC) $(CPPFLAGS) -std=c++11 src/planner.cpp -o bin/plan

bin:
	mkdir(bin)

clean:
	rm bin/*
