
# unit tests

TXFM=sed 's/xrange/range/g;s/0x[a-f0-9]*/0xXXXXXX/g' < out/tmp.out 

# tkdiff is included as part of the Jenarix "ext" external dependencies


TKDIFF=../../../ext/bin/tkdiff

all: 
	../jx -u unit01.jx > out/tmp.out || (cat out/tmp.out; exit 1;)
	$(TXFM) > out/unit01.out 
	diff -q out/unit01.out out/ref || $(TKDIFF) out/unit01.out out/ref

	../jxp -u unit02.jxp > out/tmp.out || (cat out/tmp.out; exit 1;)
	$(TXFM) > out/unit02.out 
	diff -q out/unit02.out out/ref || $(TKDIFF) out/unit02.out out/ref

	../jx -u unit03.jx > out/tmp.out || (cat out/tmp.out; exit 1;)
	$(TXFM) > out/unit03.out 
	diff -q out/unit03.out out/ref || $(TKDIFF) out/unit03.out out/ref
	echo
	echo # Tests completed successfully!