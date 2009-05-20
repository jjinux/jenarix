
all:  jx_safe.o 

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

jx_safe.o: jx_safe.c $(JX_INC_FILES)

