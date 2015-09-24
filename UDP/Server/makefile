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
	UDPServer.java

MAIN = UDPServer

default: classes
classes: $(CLASSES:.java=.class)

run: classes
	$(JVM) $(MAIN) 10019

clean:
	$(RM) *.class



