HEADERS       = gui.h \
                ../../../trunk/src/libjx/include/jx_public.h

INCLUDEPATH   = ../../../trunk/src/libjx/include/ \
                ../../../trunk/src/libjx/src/core \
                ../../../trunk/src/libjx/src/os \
                ../../../trunk/src/libjx/src/main

SOURCES       = gui.cpp \
                main.cpp

LIBS         += ../../../trunk/src/libjx/libjx.a

# install
target.path = gui
sources.files = gui.pro
sources.path = gui
INSTALLS += target sources
