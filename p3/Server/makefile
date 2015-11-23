.Phony:: all

all: prog run

prog: UDPServer.c
	gcc -o out.out UDPServer.c

run: out.out
	./out.out 10019
