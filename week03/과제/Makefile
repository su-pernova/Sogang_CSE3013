a.out: main.o str.o
	g++ -o a.out main.o str.o

str.o: str.cpp
	g++ -c -g str.cpp

.PHONY : clean
clean:
	rm *.o a.out
