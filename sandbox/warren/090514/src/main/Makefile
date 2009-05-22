all: main_jxx main_jxeval

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

main_jxeval.o: main_jxx.c $(JX_INC_FILES)
main_jxeval.o: main_jxeval.c $(JX_INC_FILES)

main_jxx: main_jxx.o $(JX_ROOT)/libjx.a
main_jxeval: main_jxeval.o $(JX_ROOT)/libjx.a

clean:
	/bin/rm -f main_*


