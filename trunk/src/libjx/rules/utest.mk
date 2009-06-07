

all: 
	../jx -u unit01.jx > out/unit01.out || (cat out/unit01.out; exit -1;)
	diff -q out/unit01.out out/ref || tkdiff out/unit01.out out/ref

	../jxp -u unit02.jxp > out/unit02.out || (cat out/unit02.out; exit -1;)
	diff -q out/unit02.out out/ref || tkdiff out/unit02.out out/ref


