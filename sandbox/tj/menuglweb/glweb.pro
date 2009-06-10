HEADERS       = glwidget.h \
                mainwindow.h \
                glweb.h \
                ../../../trunk/src/libjx/include/jx_public.h  \
                jenarix.h

INCLUDEPATH   = ../../../trunk/src/libjx/include/

SOURCES       = glwidget.cpp \
                mainwindow.cpp \
                main.cpp \
                glweb.cpp \
		jxprimitives.cpp

LIBS         += ../../../trunk/src/libjx/libjx.a

QT           += opengl \
                webkit

# install
target.path = glweb
sources.files = glweb.pro
sources.path = glweb
INSTALLS += target sources
