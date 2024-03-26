CC = g++
CFLAGS = -Wall -O3 --std=c++11 




main.exe : main.o random.o LSN3.h
	$(CC) random.o main.o -larmadillo -o main.exe
main.o : main.cpp 
	$(CC) -c main.cpp -o main.o $(CFLAGS)



random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o main.exe main2.exe seed.out
