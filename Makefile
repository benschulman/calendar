CC=g++
CPPFLAGS=-Wall

build: clean src/planner.cpp src/plan.h
	$(CC) $(CPPFLAGS) -std=c++11 src/planner.cpp -o bin/plan

clean:
	rm -f bin/plan
