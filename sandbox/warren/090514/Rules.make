
# right now we're just targeting GCC, so hopefully these settings will
# work on Mac and Linux as well as perhaps Cygwin / MinGW

CFLAGS_ALL = -g -Wall -ansi -pedantic -std=gnu99
#-O3 -m64
# -DJX_64_BIT

LDFLAGS = -lm 
# -m64
# -pthread



# include file list for manual Makefile dependencies
# (replace this with something automatic in the future...)

JX_INC_FILES = \
	$(JX_ROOT)/include/jx_public.h \
	$(JX_ROOT)/include/jx_inline.h \
	$(JX_ROOT)/src/vm/jx_safe.h \
	$(JX_ROOT)/src/core/jx_private.h \
	$(JX_ROOT)/src/os/jx_mem_wrap.h \
	$(JX_ROOT)/src/os/jx_heap.h \
	$(JX_ROOT)/src/os/jx_os_process.h \
	$(JX_ROOT)/src/os/jx_os_process_private.h \
	$(JX_ROOT)/src/os/jx_os_nothreads.h \
	$(JX_ROOT)/src/os/jx_os_pthreads.h 

