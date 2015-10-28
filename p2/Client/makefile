.Phony:: all

all: prog run

prog: UDPClient.c
	gcc -o out.out UDPClient.c

run: out.out
	./out.out tux183 10019 1 www.google.com www.facebook.com www.amazon.com yahoo.com .... www...

