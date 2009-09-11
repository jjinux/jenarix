HEADERS       = glwidget.h \
                mainwindow.h \
                jxdock.h \
                ../../../trunk/src/libjx/include/jx_public.h  \
                jenarix.h

INCLUDEPATH   = ../../../trunk/src/libjx/include/

SOURCES       = glwidget.cpp \
                mainwindow.cpp \
                main.cpp \
                jxdock.cpp \
		jxprimitives.cpp

LIBS         += ../../../trunk/src/libjx/libjx.a

QT           += opengl \
                webkit

# install
target.path = jxdock
sources.files = jxdock.pro
sources.path = jxdock
INSTALLS += target sources
