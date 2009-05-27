

all: libjx

include ../.config
include ../../../rules/arch/$(JX_ARCH).mk
include ../../../rules/objects.mk

JX_BUILD=build/$(JX_PREFIX1)-$(JX_ARCH)

mains: $(JX_PREFIX1)_lisp.a $(JX_PREFIX1)_shell.a 
	/bin/mv -f ../$(JX_PREFIX1)_shell $(JX_BASE)/$(JX_PREFIX1)s
	/bin/mv -f ../$(JX_PREFIX1)_lisp $(JX_BASE)/$(JX_PREFIX1)l

# jx_python.a jx_java.a jx_network.a

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
	test200.a \
	test201.a \
	test202.a \
	test203.a \
	test204.a 

libjx: libjx.a

libjx.a: $(OOO_LIB)
	ar -cr ../$@ $(OOO_LIB) $(LDFLAGS) $(LDFLAGS_ARCH)
	/bin/mv -f ../$@  $(JX_BASE)/$@

.o.a:
	cc $< $(OOO_LIB) -o $@ $(LDFLAGS) $(LDFLAGS_ARCH)
	/bin/mv -f $@ ../$*