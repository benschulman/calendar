CC=g++
CPPFLAGS=-Wall

build: clean src/planner.cpp src/plan.h src/cal.cpp src/cal.h
	$(CC) $(CPPFLAGS) -std=c++11 src/planner.cpp src/cal.cpp -o bin/plan

clean:
	rm -f bin/plan
