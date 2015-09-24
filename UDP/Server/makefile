.PHONY: all

all: run

JFLAGS = -g
JC = javac
JVM = java
FILE=
.SUFFIXES: .java .class
.java.class:
	$(JC) $(FLAGS) $*.java
CLASSES = \
	ServerUDP.java

MAIN = ServerUDP

default: classes
classes: $(CLASSES:.java=.class)

run: classes
	$(JVM) $(MAIN) 10019

clean:
	$(RM) *.class



