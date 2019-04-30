CC=g++
CPPFLAGS=-Wall

build: src/planner.cpp
	$(CC) $(CPPFLAGS) src/planner.cpp -o plan.out
