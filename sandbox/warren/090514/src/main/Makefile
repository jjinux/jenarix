all: main_jx

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

main_jx.o: main_jx.c $(JX_INC_FILES)

main_jx: main_jx.o $(JX_ROOT)/libjx.a

clean:
	/bin/rm -f main_*


