
JX_ROOT=.

include Rules.make

all: tests main

libjx.a: core jxon code safe
	ar -cr libjx.a src/core/*.o src/jxon/*.o src/code/*.o src/safe/*.o

core:
	cd src/core;$(MAKE)

gen:
	cd src/jxon;$(MAKE) gen;

jxon: 
	cd src/jxon;$(MAKE)

code:
	cd src/code;$(MAKE)

safe:
	cd src/safe;$(MAKE)

main: libjx.a
	cd src/main; $(MAKE)
	mv src/main/main_jxx jxx
	mv src/main/main_jxv jxv

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
