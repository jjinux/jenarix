
all:  jx_code.o 

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

jx_code.o: jx_code.c $(JX_INC_FILES)

