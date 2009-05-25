
all: jx_private.o 

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/os $(CFLAGS_ALL)

jx_private.o: jx_private.c  $(JX_INC_FILES)

