.Phony:: all

all: prog run

prog: ServerTCP.c
	gcc -o ServerTCP.out ServerTCP.c

run: ServerTCP.out
	./ServerTCP.out 10019

