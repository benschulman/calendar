src = src/cal.cpp src/plan.cpp src/events.cpp
obj = bin/cal.o bin/plan.o bin/events.o
CC = g++
CXXFLAGS = -c -Wall -std=c++11

bin/plan: $(obj)
	$(CC) $^ -o bin/plan

bin/cal.o: src/cal.cpp src/cal.h
	mkdir -p bin
	$(CC) $(CXXFLAGS) src/cal.cpp -o bin/cal.o

bin/plan.o: src/plan.cpp src/plan.h src/cal.h
	mkdir -p bin
	$(CC) $(CXXFLAGS) src/plan.cpp -o bin/plan.o

bin/events.o: src/events.cpp src/events.h
	mkdir -p bin
	$(CC) $(CXXFLAGS) src/events.cpp -o bin/events.o

.PHONY : clean
clean:
	rm -f bin/*
