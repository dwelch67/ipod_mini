
CROSS_COMPILE = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

all : rockbox.ipod

rockbox.ipod : rockloader blinker0.bin
	./rockloader blinker0.bin rockbox.ipod

blinker0.bin : startup.o blinker0.o memmap
	$(CROSS_COMPILE)-ld startup.o blinker0.o -T memmap -o blinker0.elf  
	$(CROSS_COMPILE)-objdump -D blinker0.elf > blinker0.list
	$(CROSS_COMPILE)-objcopy blinker0.elf -O binary blinker0.bin

startup.o : startup.s
	$(CROSS_COMPILE)-as $(AOPS) startup.s -o startup.o

blinker0.o : blinker0.c 
	$(CROSS_COMPILE)-gcc -c $(COPS) blinker0.c -o blinker0.o

rockloader : rockloader.c
	gcc rockloader.c -o rockloader

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.list
	rm -f rockloader
	rm -f rockbox.ipod

