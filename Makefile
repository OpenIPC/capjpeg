CROSS_COMPILE ?= mipsel-openipc-linux-musl-
#CROSS_COMPILE ?= mips-linux-gnu-
#CROSS_COMPILE ?= mips-buildroot-linux-musl-


CC = $(CROSS_COMPILE)gcc
CPLUSPLUS = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar cr
STRIP = $(CROSS_COMPILE)strip

SYSROOT := $(shell $(CC)  --print-sysroot)
TARGET = /home/javier/camera_t31n/firmware/output/target

EXTRA_LIBS_DIR = $(TARGET)/usr/lib
EXTRA_INC_DIR = $(SYSROOT)/usr/include

STATIC_LIBS_PATH = -L lib/uclibc

INCLUDES = -I$(EXTRA_INC_DIR) -I./include

CFLAGS = $(INCLUDES) -O2 -Wall -g
CFLAGS +=  -march=mips32r2

LIBS = -L $(EXTRA_LIBS_DIR)

LDFLAG += -Wl,-z,now,-gc-sections

all: capJPEG

capJPEG: stub.o capJPEG.o
	$(CC) $(LDFLAG) $(LIBS) -o $@ $^ -limp -lalog -lpthread -lm -lrt
	$(STRIP) $@

%.o:%.c stub.h
	$(CC) -c $(CFLAGS) $< -o $@

test:
	@echo $(SYSROOT)

clean:
	rm -f *.o *~ capJPEG capJPEG-static

distclean: clean
	rm -f $(SAMPLES)
