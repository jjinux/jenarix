INCLUDES=-I../../../trunk/src/libjx/include \
 -I../../../trunk/src/libjx/src/core \
 -I../../../trunk/src/libjx/src/main \
 -I../../../trunk/src/libjx/src/os
LIBS=../../../trunk/src/libjx/libjx.a -lpthread

gui: gui.o guiCreator.o
	$(CXX) -o $@ $^ $(LIBS)
gui.o: gui.cpp
	$(CXX) -c -o $@ $(INCLUDES) $<
guiCreator.o: guiCreator.cpp
	$(CXX) -c -o $@ $(INCLUDES) $<

guiCreator.o: guiCreator.h
gui.o: guiCreator.h

clean:
	rm gui *.o
