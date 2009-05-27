all: main_jxl

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

main_jxl.o: main_jxl.c $(JX_INC_FILES)

main_jxl: main_jxl.o $(JX_ROOT)/libjx.a

clean:
	/bin/rm -f main_*


