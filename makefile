# Project
TARGET = autspir
OBJECTS = build/main.o build/adc_pin.o build/twi_module.o build/lcd_module.o
OBJECTS += build/pin_io.o build/rtc_3w.o build/spi_module.o
# chip and project specific global definitons
MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200UL

CC=avr-g++
OBJCOPY=avr-objcopy
IDIR=include/

CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=atmega328p -std=c++11 -I $(IDIR) -o $@
PORT=/dev/ttyACM0

build/autspir_hex.hex: build/autspir.elf
	$(OBJCOPY) -j .text -j .data -O ihex build/autspir.elf build/autspir_hex.hex

bin/autspir_bin.bin: build/autspir.elf
	$(OBJCOPY) -O binary -R build/autspir.elf bin/handsOnbin.bin

build/autspir.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -g -Wall -o $@ $(OBJECTS)

build/main.o: src/main.cpp
	$(CC) $(CFLAGS) -g -Wall -c src/main.cpp -o $@

build/adc_pin.o: src/adc_pin.cpp include/adc_pin.h
	$(CC) $(CFLAGS) -g -Wall -c src/adc_pin.cpp

build/twi_module.o: src/twi_module.cpp include/twi_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/twi_module.cpp

build/rtc_3w.o: src/rtc_3w.cpp include/rtc_3w.h
	$(CC) $(CFLAGS) -g -Wall -c src/rtc_3w.cpp

build/lcd_module.o: src/lcd_module.cpp include/lcd_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/lcd_module.cpp

build/pin_io.o: src/pin_io.cpp include/pin_io.h
	$(CC) $(CFLAGS) -g -Wall -c src/pin_io.cpp

build/spi_module.o: src/spi_module.cpp include/spi_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/spi_module.cpp

install: build/autspir_hex.hex
	avrdude -F -V -v -v -c arduino -p atmega328p -P $(PORT) -b 115200 -e -U flash:w:build/autspir_hex.hex

clean:
	rm $(OBJECTS) build/autspir.elf build/autspir_hex.hex
