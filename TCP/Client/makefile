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
	ClientTCP.java

MAIN = ClientTCP

default: classes
classes: $(CLASSES:.java=.class)

run: classes
	$(JVM) $(MAIN) tux183 10019 5 "Hello There!"

clean:
	$(RM) *.class



