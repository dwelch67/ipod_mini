
CROSS_COMPILE = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

all : rockbox.ipod

rockbox.ipod : rockloader wheel0.bin
	./rockloader wheel0.bin rockbox.ipod

wheel0.bin : startup.o wheel0.o hexlcd.o memmap
	$(CROSS_COMPILE)-ld startup.o wheel0.o hexlcd.o -T memmap -o wheel0.elf  
	$(CROSS_COMPILE)-objdump -D wheel0.elf > wheel0.list
	$(CROSS_COMPILE)-objcopy wheel0.elf -O binary wheel0.bin

startup.o : startup.s
	$(CROSS_COMPILE)-as $(AOPS) startup.s -o startup.o

wheel0.o : wheel0.c 
	$(CROSS_COMPILE)-gcc -c $(COPS) wheel0.c -o wheel0.o

hexlcd.o : hexlcd.c hexfont.h
	$(CROSS_COMPILE)-gcc -c $(COPS) hexlcd.c -o hexlcd.o

rockloader : rockloader.c
	gcc rockloader.c -o rockloader


clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.list
	rm -f rockloader
	rm -f rockbox.ipod


