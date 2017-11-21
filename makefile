# tested and worked at 2017/09/09 22:24


## makefile in dev state .2
## basic config with multiple source files
## things to do:
				## define variables for target, objects, sources, etc.
				## optimize structure of makefile

# Project
TARGET = handsOn
OBJECTS = main.o class_adc_pin.o class_TWI.o class_lcd_1602.o

# chip and project specific global definitons
MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200UL

CC=avr-g++
OBJCOPY=avr-objcopy
#CONFIG=home/stban/Projekt/irrigation_bot/code/pratices/microcontroller_practices/avrdude_atmega328p.conf
#CONFIG_DEFAULT=/etc/avrdude.conf

CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=atmega328p -std=c++11
PORT=/dev/ttyACM0

handsOn.hex: handsOn.elf
	$(OBJCOPY) -j .text -j .data -O ihex handsOn.elf handsOn.hex

handsOnbin.bin: handsOn.elf
	$(OBJCOPY) -O binary -R handsOn.elf handsOnbin.bin

handsOn.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -g -Wall -o handsOn.elf $(OBJECTS)

main.o: main.cpp
	$(CC) $(CFLAGS) -g -Wall -c main.cpp

class_adc_pin.o: MCU/class_adc_pin.cpp MCU/class_adc_pin.hpp
	$(CC) $(CFLAGS) -g -Wall -c MCU/class_adc_pin.cpp

class_TWI.o: TWI/class_TWI.cpp TWI/class_TWI.hpp
	$(CC) $(CFLAGS) -g -Wall -c TWI/class_TWI.cpp

class_lcd_1602.o: TWI/class_lcd_1602.cpp TWI/class_lcd_1602.hpp
	$(CC) $(CFLAGS) -g -Wall -c TWI/class_lcd_1602.cpp

install: handsOn.hex
	avrdude -F -V -v -v -c arduino -p atmega328p -P $(PORT) -b 115200 -e -U
	flash:w:handsOn.hex

clean:
	rm $(OBJECTS) handsOn.elf handsOn.hex
