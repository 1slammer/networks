.Phony:: all

all: prog run

prog: TCPServerDisplay.c
	gcc -o tcp.out TCPServerDisplay.c

run: tcp.out
	./tcp.out 10019

