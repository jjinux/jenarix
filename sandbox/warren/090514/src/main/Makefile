all: main

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

main.o: main.c $(JX_INC_FILES)

main: main.o $(JX_ROOT)/libjx.a

clean:
	/bin/rm -f main


