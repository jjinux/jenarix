

# genericize the output to enable  comparisons

TXFM=sed 's/xrange/range/;s/0x[a-f0-9]+/0xX/'

all: 
	../jx -u unit01.jx | $(TXFM) > out/unit01.out || (cat out/unit01.out; exit 0;)
	diff -q out/unit01.out out/ref || tkdiff out/unit01.out out/ref

	../jxp -u unit02.jxp | $(TXFM) > out/unit02.out || (cat out/unit02.out; exit 0;)
	diff -q out/unit02.out out/ref || tkdiff out/unit02.out out/ref


