CXXFLAGS = -g
INCLUDES=-I../../../../trunk/src/libjx/include \
 -I../../../../trunk/src/libjx/src/core \
 -I../../../../trunk/src/libjx/src/main \
 -I../../../../trunk/src/libjx/src/os
LIBS=../../../../trunk/src/libjx/libjx.a  \
 -L/usr/lib \
 -lpthread

test2: test2.o jx.o
	$(CXX) -o $@ $^ $(LIBS)
test2.o: test2.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $<
jx.o: jx.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $<

test2.o: jx.h

clean:
	rm -f test2 *.o
