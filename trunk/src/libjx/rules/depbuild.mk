
include .config

JX_BUILD=$(JX_BASE)/build/$(JX_PREFIX1)-$(JX_ARCH)

all:
	make -f ../../rules/prime.mk
	cd $(JX_BUILD)/src;make -f ../../../rules/include.mk
	cd $(JX_BUILD)/src;make -j8 -f ../../../rules/compile.mk

