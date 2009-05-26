
JX_ROOT=.

include Rules.make

all: main tests 

libjx.a: core jxon vm os
	ar -cr libjx.a src/core/*.o src/jxon/*.o src/vm/*.o src/os/*.o

core:
	cd src/core;$(MAKE)

gen:
	cd src/jxon;$(MAKE) gen;

jxon: 
	cd src/jxon;$(MAKE)

os:
	cd src/os;$(MAKE);
vm:
	cd src/vm;$(MAKE)

main: libjx.a
	cd src/main; $(MAKE)
	mv src/main/main_jx jx

test100: libjx.a
	cd src/test;$(MAKE) test100

test200: libjx.a
	cd src/test;$(MAKE) test200

test201: libjx.a
	cd src/test;$(MAKE) test201

tests: libjx.a
	cd src/test;$(MAKE)

clean: 
	/bin/rm -f src/*/*.o libjx.a jxeval jxx
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
