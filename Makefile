# put your *.o targets here, make should handle the rest!

SRCS = main.c system_stm32f0xx.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib -Ilib/inc 
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals 

SRCS += lib/startup_stm32f0xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lstm32f0
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
