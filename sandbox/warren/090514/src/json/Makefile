
all:  jx_json_out.o jx_json_scan.o jx_json_parse.o

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

jx_json_out.o: jx_json_out.c $(JX_INC_FILES)

jx_json_parse.o: jx_json_parse.c $(JX_INC_FILES)

jx_json_scan.o: jx_json_scan.c $(JX_INC_FILES)

gen:
	lemon jx_json_parse.lem
	re2c jx_json_scan.re > jx_json_scan.c


