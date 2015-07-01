DEVICE     = atmega328p
OBJECTS    = main.o


COMPILE = avr-gcc -Wall -Os -mmcu=$(DEVICE)
 
all: main.hex
 
.c.o:
	$(COMPILE) -c $< -o $@
 
.S.o:
	$(COMPILE) -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:  all
	avrdude -V -F -c arduino -p ATMEGA328P -b 115200 -P /dev/ttyUSB0 -U flash:w:main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS) *~
 
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)
 
main.hex: main.elf
	rm -f main.hex
	avr-objcopy -O ihex -R .eeprom main.elf main.hex
 
