CXXFLAGS=-DJX_QT
INCLUDES=-I../../include \
 -I../../src/core \
 -I../../src/main \
 -I../../src/os
LIBS=../../libjx.a  \
 -L/usr/lib \
 -lpthread

#QT stuff, if desired
MOC=/usr/bin/moc
QTINCS=-I/usr/include/qt4/QtCore \
 -I/usr/include/qt4/QtGui \
 -I/usr/include/qt4/QtOpenGL \
 -I/usr/include/qt4 \
 -I/usr/X11R6/include
QTLIBS=-L/usr/X11R6/lib \
 -lQtOpenGL \
 -lQtGui \
 -lQtCore \
 -lGLU \
 -lGL
QTOBS=glwidget.o moc_glwidget.o jxmenu.o moc_jxmenu.o
QTFLAGS=-DJX_QT
#uncomment the following lines to disable QT
#MOC=
#QTOBS=
#QTFLAGS=
#QTLIBS=
#QTINCS=
#uncomment the above lines to disable QT
CXXFLAGS=-g $(QTFLAGS)

gui: gui.o guiCreator.o $(QTOBS)
	$(CXX) -o $@ $^ $(LIBS) $(QTLIBS)
gui.o: gui.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
guiCreator.o: guiCreator.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
glwidget.o: glwidget.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
jxmenu.o: jxmenu.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
moc_glwidget.o: moc_glwidget.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
moc_jxmenu.o: moc_jxmenu.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<

moc_glwidget.cpp: glwidget.h
	$(MOC) -o $@ $<
moc_jxmenu.cpp: jxmenu.h
	$(MOC) -o $@ $<

guiCreator.o: guiCreator.h
gui.o: guiCreator.h

clean:
	rm -f gui *.o moc_*.cpp
