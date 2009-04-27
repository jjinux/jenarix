
CFLAGS =  -O3 -Wall -ansi -pedantic -std=gnu99
# -DJX_64_BIT
# -m64

LDFLAGS = 
# -m64

tests: test001 test002 test003 test004 test005 test006

jx_private.o: jx_private.c jx_inline.h jx_private.h jx_public.h

test001.o: test001.c jx_public.h jx_inline.h

test002.o: test002.c jx_public.h jx_inline.h

test003.o: test003.c jx_public.h jx_inline.h

test004.o: test004.c jx_public.h jx_inline.h

test005.o: test005.c jx_public.h jx_inline.h

test006.o: test006.c jx_public.h jx_inline.h

test001: test001.o jx_private.o

test002: test002.o jx_private.o 

test003: test003.o jx_private.o 

test004: test004.o jx_private.o 

test005: test005.o jx_private.o

test006: test006.o jx_private.o

clean: 
	/bin/rm -f *.o

test:
	test001 | perl
	test002 | perl
	test003 | perl
	test004 | perl
	test005 | perl
	test006 | perl
