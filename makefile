# tested and worked at 2017/09/09 22:24


## makefile in dev state .2
## basic config with multiple source files
## things to do:
				## define variables for target, objects, sources, etc.
				## optimize structure of makefile

# Project
TARGET = handsOn
OBJECTS = build/main.o build/class_adc_pin.o build/class_TWI.o build/class_lcd_1602.o build/class_pin_io.o build/class_rtc_3w.o

# chip and project specific global definitons
MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200UL

CC=avr-g++
OBJCOPY=avr-objcopy
IDIR=include/

CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=atmega328p -std=c++11 -I $(IDIR)
PORT=/dev/ttyACM0

build/autspir_hex.hex: build/autspir.elf
	$(OBJCOPY) -j .text -j .data -O ihex build/autspir.elf build/autspir_hex.hex

bin/autspir_bin.bin: /build/autspir.elf
	$(OBJCOPY) -O binary -R build/autspir.elf bin/handsOnbin.bin

build/autspir.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -g -Wall -o handsOn.elf $(OBJECTS)

build/main.o: src/main.cpp
	$(CC) $(CFLAGS) -g -Wall -c src/main.cpp

build/class_adc_pin.o: src/class_adc_pin.cpp include/class_adc_pin.hpp
	$(CC) $(CFLAGS) -g -Wall -c src/class_adc_pin.cpp

build/class_TWI.o: src/class_TWI.cpp include/class_TWI.hpp
	$(CC) $(CFLAGS) -g -Wall -c src/class_TWI.cpp

build/class_rtc_3w.o: src/class_rtc_3w.cpp include/class_rtc_3w.hpp
	$(CC) $(CFLAGS) -g -Wall -c src/class_rtc_3w.cpp

install: build/autspir_hex.hex
	avrdude -F -V -v -v -c arduino -p atmega328p -P $(PORT) -b 115200 -e -U flash:w:build/autspir_hex.hex

clean:
	rm $(OBJECTS) build/autspir.elf build/autspir_hex.hex
