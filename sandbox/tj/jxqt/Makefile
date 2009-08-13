INCLUDES=-I../../../trunk/src/libjx/include \
 -I../../../trunk/src/libjx/src/core \
 -I../../../trunk/src/libjx/src/main \
 -I../../../trunk/src/libjx/src/os
LIBS=../../../trunk/src/libjx/libjx.a  \
 -L/usr/lib \
 -lpthread -lm

CXXFLAGS=-g

qtest: qtest.o qtBuiltins.o
	$(CXX) -o $@ $^ $(LIBS) $(QTLIBS)
qtest.o: qtest.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
qtBuiltins.o: qtBuiltins.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<

clean:
	rm -f qtest *.o
