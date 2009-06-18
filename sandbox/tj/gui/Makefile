INCLUDES=-I../../../trunk/src/libjx/include \
 -I../../../trunk/src/libjx/src/core \
 -I../../../trunk/src/libjx/src/main \
 -I../../../trunk/src/libjx/src/os \
 -I/usr/include/qt4/QtCore \
 -I/usr/include/qt4/QtGui \
 -I/usr/include/qt4/QtOpenGL \
 -I/usr/include/qt4 \
 -I/usr/X11R6/include
LIBS=../../../trunk/src/libjx/libjx.a  \
 -L/usr/lib \
 -L/usr/X11R6/lib \
 -lQtOpenGL \
 -lQtGui \
 -lQtCore \
 -lGLU \
 -lGL \
 -lpthread

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
