CROSS_COMPILE ?= mipsel-openipc-linux-musl-

CC = $(CROSS_COMPILE)gcc
CPLUSPLUS = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar cr
STRIP = $(CROSS_COMPILE)strip

CFLAGS = $(INCLUDES) -O2 -Wall

LDFLAG += -Wl,-gc-sections

SYSROOT := $(shell $(CC)  --print-sysroot)
TARGET = /home/javier/camera_t31n/firmware/output/target

EXTRA_LIBS_DIR= $(TARGET)/usr/lib
EXTRA_INC_DIR= $(SYSROOT)/usr/include

LIBS= -L$(EXTRA_LIBS_DIR)
INCLUDES = -I$(EXTRA_INC_DIR) -I./include

all: capJPEG

capJPEG: stub.o capJPEG.o
	$(CC) $(LDFLAG) $(INCLUDES) -o $@ $^  $(LIBS) -limp  -lalog -lpthread -lm -lrt
	$(STRIP) $@

%.o:%.c sample-common.h
	$(CC) -c $(CFLAGS) $< -o $@

test:
	@echo $(SYSROOT)

clean:
	rm -f *.o *~ capJPEG

distclean: clean
	rm -f $(SAMPLES)
