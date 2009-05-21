all: main_jxx main_jxv

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

main_jxx.o: main_jxx.c $(JX_INC_FILES)
main_jxv.o: main_jxv.c $(JX_INC_FILES)

main_jxx: main_jxx.o $(JX_ROOT)/libjx.a
main_jxv: main_jxv.o $(JX_ROOT)/libjx.a

clean:
	/bin/rm -f main_*


