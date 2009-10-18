

hello_world: main.c
	cc -I../../libjx/include main.c -DJX_HAVE_CONFIG -ljx -L../../libjx -o $@

clean:
	touch .nofail
	/bin/rm -f .nofail hello_world

