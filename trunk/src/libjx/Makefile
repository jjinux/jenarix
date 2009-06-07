
all: depbuild mains

# we can build the jx library with alternate prefixes: xx, ov, etc. so
# as to have "private" versions co-existing in a global symbol namespace

# normal mode -- optimized builds without heap tracking

jx:
	echo "JX_ARCH="`uname`-`uname -m` | sed 's/ /_/g' > .config
	echo "JX_CONFIG=" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

# quick-build devel -- unoptimized builds with heap trackng

jx-dev:
	echo "JX_ARCH="`uname`-`uname -m`-dev | sed 's/ /_/g' > .config
	echo "JX_CONFIG=-dev" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

# quick-build 64-bit devel --  unoptimized 64 bit builds with heap trackng

jx-dev64:
	echo "JX_ARCH="`uname`-`uname -m`-dev64 | sed 's/ /_/g' > .config
	echo "JX_CONFIG=-dev64" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

# run mode -- max optimized builds with no heap tracking

jx-run:
	echo "JX_ARCH="`uname`-`uname -m`-run | sed 's/ /_/g' > .config
	echo "JX_CONFIG=-run" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

# run mode -- max optimized builds with no heap tracking

jx-run64:
	echo "JX_ARCH="`uname`-`uname -m`-run64 | sed 's/ /_/g' > .config
	echo "JX_CONFIG=-run64" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

# mixed run mode (32 bit data, 64-bit arch)
# -- max optimized builds with no heap tracking

jx-mixed:
	echo "JX_ARCH="`uname`-`uname -m`-mixed | sed 's/ /_/g' > .config
	echo "JX_CONFIG=-mixed" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

# max mode -- max optimized builds with no heap tracking and no symbols

jx-max:
	echo "JX_ARCH="`uname`-`uname -m`-max | sed 's/ /_/g' > .config
	echo "JX_CONFIG=-max" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

# max mode -- max optimized builds with no heap tracking and no symbols

jx-max64:
	echo "JX_ARCH="`uname`-`uname -m`-max64 | sed 's/ /_/g' > .config
	echo "JX_CONFIG=-max64" >> .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=jx" >> .config
	echo "JX_PREFIX2=JX" >> .config
	echo "JX_PREFIX3=Jx" >> .config
	make -f rules/srclink.mk direct
	exit 0 # Jenarix configured -- you can now run 'make'

xx:
	echo "JX_ARCH="`uname`-`uname -m` | sed 's/ /_/g' > .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=xx" >> .config
	echo "JX_PREFIX2=XX" >> .config
	echo "JX_PREFIX3=xx" >> .config
	make -f rules/srclink.mk remap 
	exit 0 # Jenarix configured -- you can now run 'make'

ov:
	echo "JX_ARCH="`uname`-`uname -m` | sed 's/ /_/g' > .config
	echo "JX_BASE="`pwd` >> .config
	echo "JX_PREFIX1=ov" >> .config
	echo "JX_PREFIX2=OV" >> .config
	echo "JX_PREFIX3=Ov" >> .config
	make -f rules/srclink.mk remap 
	exit 0 # Jenarix configured -- you can now run 'make'

.config: 
	sleep 0 # "To configure the build, issue:     make jx"
	sleep 0 # "Thereafter, you can simply issue:  make"
	sleep 0 # "and after SVN updates always run:  make restart"
	sleep 0 # " to recreate the build directory"
	exit 1

# defaults

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

utests:
	make -f rules/main.mk $@

utupdate:
	make -f rules/main.mk $@

clean: .config
	make -f rules/main.mk $@

cleaner: .config
	make -f rules/main.mk $@

distclean: 
	/bin/rm -rf build
	/bin/rm -f .config jx jxl jxs jxp jxj libjx.a bin

restart: .config
	make -f rules/main.mk restart mains

# shortcuts / aliases for lazy programmmers

re: restart

dc: distclean

dev: jx-dev 

dev64: jx-dev64

run: jx-run

run64: jx-run64

mixed: jx-mixed

max: jx-max

max64: jx-max64

ut: utests

utu: utupdate

