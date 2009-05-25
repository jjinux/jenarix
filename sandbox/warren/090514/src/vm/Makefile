
all:  jx_safe.o jx_code.o

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core -I$(JX_ROOT)/src/os $(CFLAGS_ALL)

jx_safe.o: jx_safe.c $(JX_INC_FILES)

jx_code.o: jx_code.c $(JX_INC_FILES)


