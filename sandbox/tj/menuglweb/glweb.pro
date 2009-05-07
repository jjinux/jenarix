HEADERS       = glwidget.h \
                mainwindow.h \
                glweb.h
SOURCES       = glwidget.cpp \
                mainwindow.cpp \
                main.cpp \
                glweb.cpp
QT           += opengl \
                webkit

# install
target.path = glweb
sources.files = glweb.pro
sources.path = glweb
INSTALLS += target sources
