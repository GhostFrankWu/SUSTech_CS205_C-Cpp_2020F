Final: A4.o Mat.o 
	g++ -o Final A4.o Mat.o  -lpthread

Mat.o: Mat.cpp Mat.h
	g++ -c Mat.cpp

A4.o: A4.cpp Mat.o
	g++ -c A4.cpp 

clean:
	rm Final A4.o Mat.o