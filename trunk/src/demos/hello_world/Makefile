

hello_world: main.c ../../libjx/libjx.a
	cc -I../../libjx/include main.c -ljx -L../../libjx -o $@

clean:
	touch .nofail
	/bin/rm -f .nofail hello_world

