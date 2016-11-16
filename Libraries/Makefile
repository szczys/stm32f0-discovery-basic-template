CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c STM32F0xx_StdPeriph_Driver/src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -includestm32f0xx_conf.h -ICMSIS/Include -ICMSIS/Device/ST/STM32F0xx/Include -ISTM32F0xx_StdPeriph_Driver/inc
CFLAGS += -I../inc

SRCS = stm32f0xx_adc.c stm32f0xx_cec.c stm32f0xx_comp.c stm32f0xx_crc.c \
	stm32f0xx_dac.c stm32f0xx_dbgmcu.c stm32f0xx_dma.c stm32f0xx_exti.c \
	stm32f0xx_flash.c stm32f0xx_gpio.c stm32f0xx_i2c.c stm32f0xx_iwdg.c \
	stm32f0xx_misc.c stm32f0xx_pwr.c stm32f0xx_rcc.c stm32f0xx_rtc.c \
	stm32f0xx_spi.c stm32f0xx_syscfg.c stm32f0xx_tim.c \
	stm32f0xx_usart.c stm32f0xx_wwdg.c

OBJS = $(addprefix objs/,$(SRCS:.c=.o))
DEPS = $(addprefix deps/,$(SRCS:.c=.d))

.PHONY: all clean

all: libstm32f0.a

-include $(DEPS)

dirs:
	mkdir -p deps objs
	touch dirs

objs/%.o : %.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

libstm32f0.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f dirs $(OBJS) $(DEPS) libstm32f0.a
