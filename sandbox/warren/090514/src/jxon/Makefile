
all:  jx_jxon_out.o jx_jxon_scan.o jx_jxon_parse.o

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core -I$(JX_ROOT)/src/os $(CFLAGS_ALL)

jx_jxon_out.o: jx_jxon_out.c $(JX_INC_FILES)

jx_jxon_parse.o: jx_jxon_parse.c $(JX_INC_FILES)

jx_jxon_scan.o: jx_jxon_scan.c $(JX_INC_FILES)

gen:
	lemon jx_jxon_parse.lem
	re2c jx_jxon_scan.re > jx_jxon_scan.c


