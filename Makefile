PROJ_NAME=hello
# Location of the Libraries folder from the STM32F0xx Standard Peripheral Library
STD_PERIPH_LIB=Libraries
INC := inc
INC += $(STD_PERIPH_LIB)
INC += $(STD_PERIPH_LIB)/CMSIS/Include
INC += $(STD_PERIPH_LIB)/CMSIS/Device/ST/STM32F0xx/Include
INC += $(STD_PERIPH_LIB)/STM32F0xx_StdPeriph_Driver/inc
SRC := src Device
BUILD_DIR := build

CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size
LD=arm-none-eabi-gcc

# Location of the linker scripts
LDSCRIPT_INC=Device/ldscripts

CFLAGS = $(addprefix -I,$(INC))
CFLAGS += -Wall -g -Os
CFLAGS += -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS = -L$(STD_PERIPH_LIB) -lstm32f0 -L$(LDSCRIPT_INC) -Tstm32f0.ld
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(PROJ_NAME).map


SOURCES += $(foreach sdir,$(SRC),$(wildcard $(sdir)/*.s))
SOURCES += $(foreach sdir,$(SRC),$(wildcard $(sdir)/*.cpp))
SOURCES := $(foreach sdir,$(SRC),$(wildcard $(sdir)/*.c))
OBJECTS := $(patsubst %, $(BUILD_DIR)/%.o, $(SOURCES))

all: lib $(BUILD_DIR)/$(PROJ_NAME).elf

$(BUILD_DIR)/$(PROJ_NAME).elf: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(PROJ_NAME).elf $(BUILD_DIR)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(PROJ_NAME).elf $(BUILD_DIR)/$(PROJ_NAME).bin
	$(OBJDUMP) -St $(BUILD_DIR)/$(PROJ_NAME).elf >$(BUILD_DIR)/$(PROJ_NAME).lst
	$(SIZE) $(BUILD_DIR)/$(PROJ_NAME).elf

$(BUILD_DIR):
	mkdir -p $(addprefix $@/, $(SRC))

$(BUILD_DIR)/%.s.o: %.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.cpp.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.c.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

lib:
	make -C $(STD_PERIPH_LIB)

clean:
	rm -fR $(BUILD_DIR)

fullclean: clean
	make -C $(STD_PERIPH_LIB)

