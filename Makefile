CC = g++
CFLAGS=-c -Wall
LDFLAGS=-lwinmm

all: mouseclicker

mouseclicker: mouseclicker.o mouseclicker.res
	$(CC) mouseclicker.o mouseclicker.res $(LDFLAGS) -o mouseclicker.exe

mouseclicker.o:
	$(CC) $(CFLAGS) mouseclicker.cpp

mouseclicker.res:
	windres mouseclicker.rc -O coff -o mouseclicker.res

clean:
	rm *.o mouseclicker.exe *.res

run:
	mouseclicker.exe
