FILENAME   = at05
PORT       = /dev/ttyACM0
DEVICE     = atmega328p
PROGRAMMER = arduino

#Se utlizar o Atmega328p na placa Arduino UNO o baud rate deve ser 115200
#Se utilizar o Atmega328p nano o baud rate deve ser 57600
#Se utilizar o Atmega168 no nano o baud rate deve ser 19200
BAUD       = 115200
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)


default: compile upload clean

compile:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -c lcd.c -o lcd.o
	$(COMPILE) -c matriz.c -o matriz.o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o lcd.o matriz.o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex 
	avr-size -C --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i

clean:
	rm *.o
	rm *.elf
	rm *.hex
