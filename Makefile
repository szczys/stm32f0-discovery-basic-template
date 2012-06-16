# put your *.o targets here, make should handle the rest!

SRCS = main.c system_stm32f0xx.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=/usr/share/openocd/scripts/board

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -TDevice/stm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m

###################################################

vpath %.c src
vpath %.a Libraries

ROOT=$(shell pwd)

CFLAGS += -Iinc -ILibraries -ILibraries/CMSIS/Device/ST/STM32F0xx/Include
CFLAGS += -ILibraries/CMSIS/Include -ILibraries/STM32F0xx_StdPeriph_Driver/inc

SRCS += Device/startup_stm32f0xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

lib:
	$(MAKE) -C Libraries

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -LLibraries -lstm32f0
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	
program: $(PROJ_NAME).bin
	openocd -f $(OPENOCD_BOARD_DIR)/stm32f0discovery.cfg -f extra/stm32f0-openocd.cfg -c "stm_flash `pwd`/$(PROJ_NAME).bin" -c shutdown

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
