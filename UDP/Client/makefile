.Phony:: all

all: prog run

prog: ClientUDP.c
	gcc -o ClientUDP.out ClientUDP.c

run: ClientUDP.out
	./ClientUDP.out tux183 10019 5 Hello

