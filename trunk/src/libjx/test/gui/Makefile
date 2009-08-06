CXXFLAGS=-DJX_QT -nowarn
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
QTOBS=glwidget.o moc_glwidget.o jxaction.o moc_jxaction.o
QTFLAGS=-DJX_QT
#uncomment the following lines to disable QT
#MOC=
#QTOBS=
#QTFLAGS=
#QTLIBS=
#QTINCS=
#uncomment the above lines to disable QT
CXXFLAGS=-g $(QTFLAGS)

gui: gui.o guiCreator.o jxobject.o qtBuiltins.o $(QTOBS)
	$(CXX) -o $@ -g $^ $(LIBS) $(QTLIBS)
gui.o: gui.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
qtBuiltins.o: qtBuiltins.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
guiCreator.o: guiCreator.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
glwidget.o: glwidget.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
jxobject.o: jxobject.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $<
jxaction.o: jxaction.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
moc_glwidget.o: moc_glwidget.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<
moc_jxaction.o: moc_jxaction.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(INCLUDES) $(QTINCS) $<

moc_glwidget.cpp: glwidget.h
	$(MOC) -o $@ $<
moc_jxaction.cpp: jxaction.h
	$(MOC) -o $@ $<

guiCreator.o: guiCreator.h glwidget.h jxobject.h
gui.o: guiCreator.h qtBuiltins.h jxobject.h
jxobject.o: jxobject.h
qtBuiltins.o: qtBuiltins.h jxobject.h

clean:
	rm -f gui *.o moc_*.cpp
