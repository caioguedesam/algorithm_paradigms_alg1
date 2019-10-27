tp2: main.o
	g++ main.o -o tp2
	make clean

main.o: main.cpp ilha.h
	g++ -c main.cpp

clean:
	rm *.o
