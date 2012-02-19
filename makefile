# AVR-GCC Makefile
#---Prefs---
PROJECT=led_shirt
MMCU=attiny25
F_CPU=8000000 #8 MHz
FUSEBITS=0x62
#-----------



SOURCES=$(wildcard *.c)
CC=avr-gcc
OBJCOPY=avr-objcopy

CFLAGS=-g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wstrict-prototypes -DF_CPU=$(F_CPU) -Wa,-adhlns=$(<:.c=.lst) -I./ -mmcu=$(MMCU) -Wall
AVRDUDEFLAGS=-p $(MMCU) -c usbtiny

ALL: $(PROJECT).hex filesize

$(PROJECT).hex: $(PROJECT).elf
	avr-objcopy -j .text -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).elf $(SOURCES)

program: $(PROJECT).hex
	avrdude $(AVRDUDEFLAGS) -e -U flash:w:$(PROJECT).hex

burn-fuse:
	avrdude $(AVRDUDE_FLAGS) -u -U lfuse:w:$(FUSEBITS):m

# Display size of file.
filesize:
	avr-size -C --mcu=$(MMCU) $(PROJECT).elf 

clean:
	rm -f $(PROJECT).elf $(PROJECT).hex $(PROJECT).lst
