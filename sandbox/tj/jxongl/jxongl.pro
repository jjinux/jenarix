HEADERS       = ../../../trunk/src/libjx/include/jx_public.h \
                glwidget.h \
                window.h \
                jenarix.h
INCLUDEPATH   = ../../../trunk/src/libjx/include/
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
		jxprimitives.cpp
LIBS         += ../../../trunk/src/libjx/libjx.a
QT           += opengl

# install
target.path = jxongl
sources.files = jxongl.pro
sources.path = jxongl
INSTALLS += target sources
