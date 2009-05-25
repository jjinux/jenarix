

CFLAGS_ALL = -g -Wall -ansi -pedantic -std=gnu99
# -O3 -m64
# -DJX_64_BIT

LDFLAGS = 
#-m64

# include file list for manual Makefile dependencies

JX_INC_FILES = $(JX_ROOT)/include/jx_inline.h \
	$(JX_ROOT)/include/jx_public.h \
	$(JX_ROOT)/src/vm/jx_safe.h \
	$(JX_ROOT)/src/core/jx_private.h \
	$(JX_ROOT)/src/core/jx_mem_wrap.h 

