
all:

include .config

JX_BUILD=build/$(JX_PREFIX1)-$(JX_ARCH)

depbuild: 
	cd src/jxon; make -f ../../rules/codegen.mk jxon
	cd src/shell; make -f ../../rules/codegen.mk shell
	cd src/py; make -f ../../rules/codegen.mk py
	cd src/net; make -f ../../rules/codegen.mk net
	cd $(JX_BUILD); make -f ../../rules/depbuild.mk

lib: depbuild
	cd $(JX_BUILD)/src;make -f ../../../rules/link.mk libjx

mains: 
	cd $(JX_BUILD)/src;make -j8 -f ../../../rules/link.mk mains
	exit 0 # "Executables built! ./jx, ./jxs , etc."

tests: 
	cd $(JX_BUILD)/src;make -j8 -f ../../../rules/link.mk tests

runtests: tests
	cd $(JX_BUILD);make -f $(JX_BASE)/rules/test.mk

product: lib
	cd $(JX_BUILD); make -f ../../rules/product.mk	

clean:
	cd $(JX_BUILD); make -f ../../rules/clean.mk	

cleaner: clean
	cd $(JX_BUILD); make -f ../../rules/cleaner.mk

restart: 
	make distclean $(JX_PREFIX1)$(JX_CONFIG) depbuild
