# AVR Toolchain Configuration
MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200

# Compiler and Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# AVRDUDE Configurations
PORT = /dev/ttyUSB0
PROGRAMMER = arduino

# File Configurations
TARGET = led
SOURCE = $(TARGET).c
HEX = $(TARGET).hex
ELF = $(TARGET).elf

# Compiler Flags
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

# Linker Flags
LDFLAGS = -mmcu=$(MCU)

# Build Targets
all: $(HEX)

$(ELF): $(SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload: $(HEX)
	$(AVRDUDE) -v -patmega328p -c$(PROGRAMMER) -P$(PORT) -b$(BAUD) -D -Uflash:w:$(HEX):i

clean:
	rm -f $(ELF) $(HEX)

# Phony targets to avoid conflicts with files
.PHONY: all upload clean
