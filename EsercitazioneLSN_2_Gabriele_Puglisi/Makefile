CC = g++
CFLAGS = -Wall -O3 --std=c++11 


main2.exe : main2.o random.o
	$(CC)  random.o main2.o -larmadillo -o main2.exe 
main2.o : main2.cpp 
	$(CC) -c main2.cpp -o main2.o $(CFLAGS)

main.exe : main.o random.o IntegralMC.o FunzioneBase.h VectorOperations.h
	$(CC) IntegralMC.o random.o main.o -o main.exe
main.o : main.cpp 
	$(CC) -c main.cpp -o main.o $(CFLAGS)

IntegralMC.o : IntegralMC.cpp IntegralMC.h FunzioneBase.h VectorOperations.h
	$(CC) -c IntegralMC.cpp -o IntegralMC.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o main.exe main2.exe seed.out
