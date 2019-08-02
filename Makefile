# Project
MCU = atmega328p
FORMAT = ihex
TARGET = odb

# Optimization
OPT = s

# Source files
SRC = \
			main.c \
			driver_spi/spi.c \

# Include directories
INC = \
			driver_spi \

# FLAGS
CFLAGS = \
				 -std=gnu99 \
				 -Wall \
				 -O$(OPT) \
				 -DF_CPU=16000000 \
				 -mmcu=$(MCU) \
				 -fsigned-char \
				 -funsigned-bitfields \
				 -fpack-struct \
				 -fshort-enums \
				 $(patsubst %,-I%,$(INC))

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = /dev/ttyUSB0
AVRDUDE_BAUDRATE = 57600
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUDRATE) \
				-c $(AVRDUDE_PROGRAMMER)
SIZE = avr-size $(TARGET).elf

# Define all object files.
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET).elf $(TARGET).hex filesize

# Display size of file
filesize:
	@if [ -f $(TARGET).elf ]; then echo;$(SIZE); echo; fi

# Program the device
program: $(TARGET).hex
	@echo
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

# Create final output file
$(TARGET).hex: $(TARGET).elf
	@echo
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

# Create ELF output file
$(TARGET).elf: $(OBJ)
	@echo
	$(CC) $(CFLAGS) $^ -o $@

# Create object files
%.o : %.c
	@echo
	$(CC) -c $(CFLAGS) $< -o $@

# Clean
clean:
	@echo
	rm -f $(TARGET).hex
	rm -f $(TARGET).elf
	rm -f $(OBJ)

.PHONY : all clean program
.PRECIOUS: %.o %.elf

