HEADERS       =  ../../../trunk/src/libjx/include/jx_public.h
INCLUDEPATH   = ../../../trunk/src/libjx/include
#SOURCES       = main_jxl.cpp \
#                show_ob.cpp
SOURCES       = main_jxl.c \
                show_ob.c
LIBS          += ../../../trunk/src/libjx/libjx.a

# install
target.path = test
sources.files = test.pro
sources.path = test
INSTALLS += target sources
