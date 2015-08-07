all: build

build:
	g++ mouseclicker.cpp -o mouseclicker.exe -g -Wall

run:
	mouseclicker.exe
