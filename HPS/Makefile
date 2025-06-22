TARGET = fifo_write

ALT_DEVICE_FAMILY ?= soc_cv_av
SOCEDS_ROOT ?= $(SOCEDS_DEST_ROOT)
HWLIBS_ROOT = $(SOCEDS_ROOT)/ip/altera/hps/altera_hps/hwlib
CROSS_COMPILE = arm-linux-gnueabihf-
CFLAGS = -g -Wall -D$(ALT_DEVICE_FAMILY) -I$(HWLIBS_ROOT)/include/$(ALT_DEVICE_FAMILY) -I$(HWLIBS_ROOT)/include/ -std=c99
LDFLAGS =  -g -Wall 
CC = $(CROSS_COMPILE)gcc
ARCH= arm
#OBJFILES = read_test.c
#OBJFILES = main.o
OBJFILES = lib/HPS_FIFO.h hps_fifo.o

build: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o build/$(TARGET) $(OBJFILES) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o *~