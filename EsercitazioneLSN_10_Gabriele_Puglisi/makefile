C = mpic++ -w 
CFLAGS = -Wall -O3 --std=c++11

es10.exe: es10.o random.o
	$(C) random.o es10.o -o es10.exe $(CFLAGS)

es10.o: es10.cpp
	$(C) -c es10.cpp -o es10.o $(CFLAGS)

random.o: random.cpp random.h
	$(C) -c random.cpp -o random.o $(CFLAGS)

clean:
	rm -f *.o es10.exe
