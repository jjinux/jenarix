INCLUDES=-I../../../trunk/src/libjx/include \
 -I../../../trunk/src/libjx/src/core \
 -I../../../trunk/src/libjx/src/main \
 -I../../../trunk/src/libjx/src/os
LIBS=../../../trunk/src/libjx/libjx.a  \
 -L/usr/lib \
 -lpthread -lm

CFLAGS=-g

qtest: qtest.o qtBuiltins.o
	$(CC) -o $@ $^ $(LIBS) $(QTLIBS)
qtest.o: qtest.c
	$(CC) $(CFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
qtBuiltins.o: qtBuiltins.c
	$(CC) $(CFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<

clean:
	rm -f qtest *.o
