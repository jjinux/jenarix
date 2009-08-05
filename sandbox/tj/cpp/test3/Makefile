CXXFLAGS = -g
INCLUDES=-I../../../../trunk/src/libjx/include \
 -I../../../../trunk/src/libjx/src/core \
 -I../../../../trunk/src/libjx/src/main \
 -I../../../../trunk/src/libjx/src/os
LIBS=../../../../trunk/src/libjx/libjx.a  \
 -L/usr/lib \
 -lpthread

test3: test3.o jxobject.o
	$(CXX) -o $@ $^ $(LIBS)
test3.o: test3.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $<
jxobject.o: jxobject.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $<

test3.o: jxobject.h

clean:
	rm -f test3 *.o
