HEADERS       = glwidget.h \
                mainwindow.h \
                glweb.h \
                jenarix.h

INCLUDEPATH   = ../jsoncpp/include/

SOURCES       = glwidget.cpp \
                mainwindow.cpp \
                main.cpp \
                glweb.cpp \
		jxprimitives.cpp

OBJECTS       += ../jsoncpp/src/json_reader.o \
                 ../jsoncpp/src/json_value.o \
                 ../jsoncpp/src/json_writer.o

QT           += opengl \
                webkit

# install
target.path = glweb
sources.files = glweb.pro
sources.path = glweb
INSTALLS += target sources
