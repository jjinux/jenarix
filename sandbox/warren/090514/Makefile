
JX_ROOT=.

include Rules.make

all: tests

libjx.a: core json
	ar -cr libjx.a src/core/*.o src/json/*.o

core:
	cd src/core;$(MAKE)

gen:
	cd src/json;$(MAKE) gen;

json: 
	cd src/json;$(MAKE)

test100: gen libjx.a
	cd src/test;$(MAKE) test100

tests: libjx.a
	cd src/test;$(MAKE)

clean: 
	/bin/rm -f src/*/*.o
	/bin/rm libjx.a
	cd src/test;$(MAKE) clean

test: tests
	src/test/test001 | perl
	src/test/test002 | perl
	src/test/test003 | perl
	src/test/test004 | perl
	src/test/test005 | perl
	src/test/test006 | perl
	src/test/test007 | perl
