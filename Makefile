src = src/cal.cpp src/plan.cpp
obj = bin/cal.o bin/plan.o
CC = g++
CPPFLAGS = -c -Wall -std=c++11

bin/plan: $(obj)
	$(CC) $^ -o bin/plan

bin/cal.o: src/cal.cpp src/cal.h
	$(CC) $(CPPFLAGS) src/cal.cpp -o bin/cal.o

bin/plan.o: src/plan.cpp src/plan.h src/cal.h
	$(CC) $(CPPFLAGS) src/plan.cpp -o bin/plan.o

.PHONY : clean
clean:
	rm -f bin/*
