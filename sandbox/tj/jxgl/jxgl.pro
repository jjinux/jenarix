HEADERS       = glwidget.h \
                window.h \
                jenarix.h
INCLUDEPATH   = ../jsoncpp/include/
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
		jxprimitives.cpp
OBJECTS       += ../jsoncpp/src/json_reader.o \
                 ../jsoncpp/src/json_value.o \
                 ../jsoncpp/src/json_writer.o
QT           += opengl

# install
target.path = jxgl
sources.files = jxgl.pro
sources.path = jxgl
INSTALLS += target sources
