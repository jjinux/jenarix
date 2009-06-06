
ARCH_CFLAGS = -Wall -ansi -pedantic -std=gnu99 -O3 -fomit-frame-pointer -ffast-math -finline-limit=10000 -DJX_64_BIT -m64 
# NOTE -fast causes segfault with GCC 4.0.1


ARCH_CXXFLAGS = -ansi 

ARCH_LDFLAGS = -m64



