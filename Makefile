all.out: test.o Centrale.o
	g++ test.o Centrale.o -o all

test.o: test.cpp
	g++ -c -Wall test.cpp

Centrale.o: Centrale.cpp
	g++ -c -Wall Centrale.cpp
