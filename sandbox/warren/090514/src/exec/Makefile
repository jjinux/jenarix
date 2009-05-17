
all:  jx_exec.o 

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

jx_exec.o: jx_exec.c $(JX_INC_FILES)

