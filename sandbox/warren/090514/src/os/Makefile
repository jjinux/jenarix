
all:  jx_os_process.o jx_heap.o jx_os_io.o

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core $(CFLAGS_ALL)

jx_os_process.o: jx_os_process.c $(JX_INC_FILES)

jx_heap.o: jx_heap.c $(JX_INC_FILES)

jx_os_io.o: jx_os_io.c $(JX_INC_FILES)

