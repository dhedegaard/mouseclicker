all: build

build:
	g++ mouseclicker.cpp -lwinmm -o mouseclicker.exe -g -Wall

run:
	mouseclicker.exe
