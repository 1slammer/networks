.Phony:: all

all: prog run

prog: UDP-Client.c
	gcc -o udp.out UDP-Client.c

run: udp.out
	./udp.out tux183 10019 5 Hello

