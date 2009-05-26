
all: \
	test001 \
	test002 \
	test003 \
	test004 \
	test005 \
	test006 \
	test007 \
	test008 \
	test009 \
	test010 \
	jx_echo

JX_ROOT=../..

include $(JX_ROOT)/Rules.make

CFLAGS=-I$(JX_ROOT)/include -I$(JX_ROOT)/src/core -I$(JX_ROOT)/src/os $(CFLAGS_ALL)

test001.o: test001.c $(JX_INC_FILES)
test002.o: test002.c $(JX_INC_FILES)
test003.o: test003.c $(JX_INC_FILES)
test004.o: test004.c $(JX_INC_FILES)
test005.o: test005.c $(JX_INC_FILES)
test006.o: test006.c $(JX_INC_FILES)
test007.o: test007.c $(JX_INC_FILES)
test008.o: test008.c $(JX_INC_FILES)
test009.o: test009.c $(JX_INC_FILES)
test010.o: test010.c $(JX_INC_FILES)


test100.o: test100.c $(JX_INC_FILES)

test200.o: test200.c $(JX_INC_FILES)
test201.o: test201.c $(JX_INC_FILES)
test202.o: test202.c $(JX_INC_FILES)
test203.o: test202.c $(JX_INC_FILES)

test001: test001.o $(JX_ROOT)/libjx.a
test002: test002.o $(JX_ROOT)/libjx.a
test003: test003.o $(JX_ROOT)/libjx.a
test004: test004.o $(JX_ROOT)/libjx.a
test005: test005.o $(JX_ROOT)/libjx.a
test006: test006.o $(JX_ROOT)/libjx.a
test007: test007.o $(JX_ROOT)/libjx.a
test008: test008.o $(JX_ROOT)/libjx.a
test009: test009.o $(JX_ROOT)/libjx.a
test010: test010.o $(JX_ROOT)/libjx.a

test100: test100.o $(JX_ROOT)/libjx.a

test200: test200.o $(JX_ROOT)/libjx.a
test201: test201.o $(JX_ROOT)/libjx.a
test202: test202.o $(JX_ROOT)/libjx.a
test203: test203.o $(JX_ROOT)/libjx.a

jx_echo: jx_echo.o $(JX_ROOT)/libjx.a

clean:
	/bin/rm -f \
	test001 \
	test002 \
	test003 \
	test004 \
	test005 \
	test006 \
	test007 \
	test008 \
	test009 \
	test010 \
	test200 \
	test201 \
	test202 \
	test203 \
	jx_echo

