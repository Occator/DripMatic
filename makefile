# Project
TARGET = autspir
OBJECTS = build/main.o build/adc_pin.o build/twi_module.o build/lcd_module.o
OBJECTS += build/pin_io.o build/rtc_3w.o build/spi_module.o build/uart_module.o
OBJECTS += build/sdcard_module.o build/diskio.o build/ff.o build/atexit.o
# chip and project specific global definitons
MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200UL

CC=avr-g++
OBJCOPY=avr-objcopy
IDIR=include/

CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=atmega328p -std=c++11 -I $(IDIR) -o $@
CFLAGS += -fno-threadsafe-statics
PORT=/dev/ttyACM0

build/autspir_hex.hex: build/dripmatic.elf
	$(OBJCOPY) -j .text -j .data -O ihex build/dripmatic.elf build/dripmatic_hex.hex

bin/autspir_bin.bin: build/dripmatic.elf
	$(OBJCOPY) -O binary -R build/dripmatic.elf bin/handsOnbin.bin

build/dripmatic.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -g -Wall -o $@ $(OBJECTS)

build/main.o: src/main.cpp
	$(CC) $(CFLAGS) -g -Wall -c src/main.cpp -o $@

build/adc_pin.o: src/adc_pin.cpp include/adc_pin.h
	$(CC) $(CFLAGS) -g -Wall -c src/adc_pin.cpp

build/twi_module.o: src/twi_module.cpp include/twi_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/twi_module.cpp

build/atexit.o: src/atexit.cpp
	$(CC) $(CFLAGS) -g -Wall -c src/atexit.cpp

build/rtc_3w.o: src/rtc_3w.cpp include/rtc_3w.h
	$(CC) $(CFLAGS) -g -Wall -c src/rtc_3w.cpp

build/lcd_module.o: src/lcd_module.cpp include/lcd_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/lcd_module.cpp

build/pin_io.o: src/pin_io.cpp include/pin_io.h
	$(CC) $(CFLAGS) -g -Wall -c src/pin_io.cpp

build/spi_module.o: src/spi_module.cpp include/spi_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/spi_module.cpp

build/sdcard_module.o: src/sdcard_module.cpp include/sdcard_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/sdcard_module.cpp

build/uart_module.o: src/uart_module.cpp include/uart_module.h
	$(CC) $(CFLAGS) -g -Wall -c src/uart_module.cpp

build/diskio.o: src/diskio.cpp include/diskio.h
	$(CC) $(CFLAGS) -g -Wall -c src/diskio.cpp

build/ff.o: src/ff.c include/ff.h
	$(CC) $(CFLAGS) -g -Wall -c src/ff.c

install: build/dripmatic_hex.hex
	avrdude -F -V -v -v -c arduino -p atmega328p -P $(PORT) -b 115200 -e -U flash:w:build/dripmatic_hex.hex

clean:
	rm $(OBJECTS) build/dripmatic.elf build/dripmatic_hex.hex
