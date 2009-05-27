
all: .includes

include ../.config

include .files

.cpp.p:
	g++ -M $(CFLAGS) $< | sed 's/$*\\.o[ :]*/& $@/g;s/[^ ]*\/usr\/[^ ]*//g;s/[^ ]*\/ext\/[^ ]*//g' > $@

.c.p:
	cc -M $(CFLAGS) $< | sed 's/$*\\.o[ :]*/& $@/g;s/[^ ]*\/usr\/[^ ]*//g;s/[^ ]*\/ext\/[^ ]*//g' > $@

.m.p:
	cc -M $(CFLAGS) $< | sed 's/$*\\.o[ :]*/& $@/g;s/[^ ]*\/usr\/[^ ]*//g;s/[^ ]*\/System\/[^ ]*//g;s/[^ ]*\/ext\/[^ ]*//g' > $@

.depends: $(DEPS)
	cat *.p > .depends

.includes: .depends
	cat .files .depends > .includes

