
all: current

include ../.config
include ../../../rules/arch/$(JX_ARCH).mk
include ../../../rules/objects.mk

CXXFLAGS=$(ARCH_CXXFLAGS)
CFLAGS=$(ARCH_CFLAGS) 
#CFLAGS=-O3 -g $(ARCH_CFLAGS) -Wall


include .includes

current: $(OOO) 

tests: $(OBJ)

.p.o:
	cc $*.m $(CFLAGS) -c -o $@
