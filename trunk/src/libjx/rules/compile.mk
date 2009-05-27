
all: current

include ../.config
include ../../../rules/arch/$(JX_ARCH).mk
include ../../../rules/objects.mk

CXXFLAGS=-g $(ARCH_CXXFLAGS) -Wall
CFLAGS=-g $(ARCH_CFLAGS) -Wall
#CFLAGS=-O3 -g $(ARCH_CFLAGS) -Wall


include .includes

current: $(OOO) 

tests: $(OBJ)

.p.o:
	cc $*.m $(CFLAGS) -c -o $@
