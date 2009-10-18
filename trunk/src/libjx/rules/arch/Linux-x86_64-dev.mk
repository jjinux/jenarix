
# NOTE: default build should be 32-bit Jenarix even if platform is 64 bit

ARCH_CFLAGS = -Wall -ansi -pedantic -std=gnu99 -g

ARCH_CXXFLAGS = -ansi 

ARCH_LDFLAGS = -m64 -lpthread -lm


