CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c STM32F0xx_StdPeriph_Driver/src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -includestm32f0xx_conf.h -ICMSIS/Include -ICMSIS/Device/ST/STM32F0xx/Include -ISTM32F0xx_StdPeriph_Driver/inc


SRC = STM32F0xx_StdPeriph_Driver/src
SRCS := $(foreach sdir,$(SRC),$(wildcard $(sdir)/*.c))
SRCS += $(foreach sdir,$(SRC),$(wildcard $(sdir)/*.s))
SRCS += $(foreach sdir,$(SRC),$(wildcard $(sdir)/*.cpp))
OBJS := $(patsubst %, $(BUILD_DIR)/%.o, $(SOURCES))

.PHONY: libstm32f0.a

all: libstm32f0.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f0.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f *.o $(OBJS) libstm32f0.a
