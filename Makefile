hw4: hw4.o tablecheck.o crypt.o
	g++ -g -W hw4.o tablecheck.o crypt.o -o hw4

tablecheck.o: tablecheck.h tablecheck.cpp
	g++ -g -W -c tablecheck.cpp

crypt.o: crypt.h crypt.cpp
	g++ -g -W -c crypt.cpp

hw4.o: hw4.cpp
	g++ -g -W -c hw4.cpp

clean:
	rm -f *.o *.gch hw4
