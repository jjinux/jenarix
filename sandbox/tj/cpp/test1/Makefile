CXXFLAGS = -g
INCLUDES=-I../../../trunk/src/libjx/include \
 -I../../../trunk/src/libjx/src/core \
 -I../../../trunk/src/libjx/src/main \
 -I../../../trunk/src/libjx/src/os
LIBS=../../../trunk/src/libjx/libjx.a  \
 -L/usr/lib \
 -lpthread

test1: test1.o jx.o
	$(CXX) -o $@ $^ $(LIBS)
test1.o: test1.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $<
jx.o: jx.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $<

test1.o: jx.h

clean:
	rm -f test1 *.o
