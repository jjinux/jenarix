
all: jx_private.o jx_mem_wrap.o

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include $(CFLAGS_ALL)

jx_private.o: jx_private.c  $(JX_INC_FILES)

jx_mem_wrap.o: jx_mem_wrap.c $(JX_INC_FILES)
