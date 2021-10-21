p0: p0.o 
	g++ p0.o -lpthread -o p0

p1: p1.o
	g++ p1.o -lpthread -o p1

p0.o: p0.cpp defs.h my_const.h
	g++ -c p0.cpp

p1.o: p1.cpp defs.h my_const.h
	g++ -c p1.cpp


clean:
	rm *.o p0 p1 *.txt