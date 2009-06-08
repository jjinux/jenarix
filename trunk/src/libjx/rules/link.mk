

all: libjx

include ../.config
include ../../../rules/arch/$(JX_ARCH).mk
include ../../../rules/objects.mk

JX_BUILD=build/$(JX_PREFIX1)-$(JX_ARCH)

mains: $(JX_PREFIX1)_lisp.a $(JX_PREFIX1)_shell.a $(JX_PREFIX1)_py.a $(JX_PREFIX1)_net.a
	/bin/mv -f ../$(JX_PREFIX1)_lisp $(JX_BASE)/$(JX_PREFIX1)
	/bin/mv -f ../$(JX_PREFIX1)_shell $(JX_BASE)/$(JX_PREFIX1)s
	/bin/mv -f ../$(JX_PREFIX1)_py $(JX_BASE)/$(JX_PREFIX1)p
	/bin/mv -f ../$(JX_PREFIX1)_net $(JX_BASE)/$(JX_PREFIX1)n

# jx_python.a jx_java.a jx_network.a

CXXFLAGS=$(ARCH_CXXFLAGS)
CFLAGS=$(ARCH_CFLAGS) 
#CFLAGS=-O3 -g $(ARCH_CFLAGS)

.p.o:
	cc $*.m $(CFLAGS) -c -o $@

tests: \
	test001.a \
	test002.a \
	test003.a \
	test004.a \
	test005.a \
	test006.a \
	test007.a \
	test008.a \
    test009.a \
	test010.a \
	test100.a \
	test200.a \
	test201.a \
	test202.a \
	test203.a \
	test204.a 

libjx: libjx.a

libjx.a: $(OOO_LIB)
	ar -cr ../$@ $(OOO_LIB) 
	/bin/mv -f ../$@  $(JX_BASE)/$@

.o.a:
	cc $< $(OOO_LIB) -o $@ $(LDFLAGS) $(ARCH_LDFLAGS)
	/bin/mv -f $@ ../$*