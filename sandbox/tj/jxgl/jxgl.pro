HEADERS       = glwidget.h \
                window.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp
QT           += opengl

# install
target.path = jxgl
sources.files = jxgl.pro
sources.path = jxgl
INSTALLS += target sources
