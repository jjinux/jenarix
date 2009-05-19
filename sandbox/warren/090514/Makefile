
JX_ROOT=.

include Rules.make

all: tests

libjx.a: core json exec
	ar -cr libjx.a src/core/*.o src/json/*.o src/exec/*.o

core:
	cd src/core;$(MAKE)

gen:
	cd src/json;$(MAKE) gen;

json: 
	cd src/json;$(MAKE)

exec:
	cd src/exec;$(MAKE)

jx:
	cd src/jx; $(MAKE)

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
	src/test/test008 | perl
	src/test/test009 | perl
	src/test/test010 | perl
