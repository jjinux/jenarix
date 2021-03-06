
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
	echo
	echo # "Executables built! ./jx, ./jxs , etc."

ctests: 
	cd $(JX_BUILD)/src;make -j8 -f ../../../rules/link.mk ctests
	cd $(JX_BUILD);make -f $(JX_BASE)/rules/test.mk

utests: jx
	cd $(JX_BASE)/test;make -f $(JX_BASE)/rules/utest.mk

x1tests: 
	cd $(JX_BUILD)/src;make -j8 -f ../../../rules/link.mk x1tests

x2tests: 
	cd $(JX_BUILD)/src;make -j8 -f ../../../rules/link.mk x2tests

utupdate: 
	cd $(JX_BASE)/test;make -f $(JX_BASE)/rules/utupdate.mk

product: lib
	cd $(JX_BUILD); make -f ../../rules/product.mk	

clean:
	cd $(JX_BUILD); make -f ../../rules/clean.mk	

cleaner: clean
	cd $(JX_BUILD); make -f ../../rules/cleaner.mk

restart: 
	make distclean $(JX_PREFIX1)$(JX_CONFIG) depbuild
