
# NOTE: default build should be 32-bit Jenarix even if platform is 64 bit

ARCH_CFLAGS = -Wall -ansi -pedantic -std=gnu99 -g -O3
# -m64 -DJX_64_BIT

ARCH_CXXFLAGS = -ansi 
# -m64 -DJX_64_BIT

ARCH_LDFLAGS = -m64 -lpthread -lm


