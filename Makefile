CC = g++
CFLAGS=-c -Wall
LDFLAGS=-lwinmm

all: mouseclicker

mouseclicker: mouseclicker.o
	$(CC) mouseclicker.o $(LDFLAGS) -o mouseclicker.exe

mouseclicker.o:
	$(CC) $(CFLAGS) mouseclicker.cpp

clean:
	rm *.o mouseclicker.exe

run:
	mouseclicker.exe
