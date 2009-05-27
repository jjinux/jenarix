
all: depbuild mains

# we can build the jx library with alternate prefixes: xx, ov, etc. so
# as to have "private" versions co-existing in a global symbol namespace

jx:
	echo "JX_ARCH="`uname`-`uname -m` | sed 's/ /_/g' > .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct

xx:
	echo "JX_ARCH="`uname`-`uname -m` | sed 's/ /_/g' > .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=xx" >> .config
	echo "JX_PREFIX2=XX" >> .config
	echo "JX_PREFIX3=xx" >> .config
	make -f rules/srclink.mk remap 

ov:
	echo "JX_ARCH="`uname`-`uname -m` | sed 's/ /_/g' > .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=ov" >> .config
	echo "JX_PREFIX2=OV" >> .config
	echo "JX_PREFIX3=Ov" >> .config
	make -f rules/srclink.mk remap 

.config: 
	sleep 0 # "To configure the build, issue:    make jx"
	sleep 0 # "Thereafter, you can simply issue: make"
	sleep 0 # "and after file / svn updates:     make restart"
	exit 1

# dependency-aware build 

depbuild: .config
	make -f rules/main.mk $@

lib: .config
	make -f rules/main.mk $@

mains: .config
	make -f rules/main.mk $@

tests: .config
	make -f rules/main.mk $@

runtests: .config
	make -f rules/main.mk $@

clean: .config
	make -f rules/main.mk $@

cleaner: .config
	make -f rules/main.mk $@

distclean: 
	/bin/rm -rf build
	/bin/rm -f .config jxl jxs jxp jxj libjx.a bin

restart: .config
	make -f rules/main.mk restart mains

