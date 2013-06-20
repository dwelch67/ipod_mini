
CROSS_COMPILE = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

all : rockbox.ipod

rockbox.ipod : rockloader lcd1.bin
	./rockloader lcd1.bin rockbox.ipod

lcd1.bin : startup.o lcd1.o hexlcd.o memmap
	$(CROSS_COMPILE)-ld startup.o lcd1.o hexlcd.o -T memmap -o lcd1.elf  
	$(CROSS_COMPILE)-objdump -D lcd1.elf > lcd1.list
	$(CROSS_COMPILE)-objcopy lcd1.elf -O binary lcd1.bin

startup.o : startup.s
	$(CROSS_COMPILE)-as $(AOPS) startup.s -o startup.o

lcd1.o : lcd1.c 
	$(CROSS_COMPILE)-gcc -c $(COPS) lcd1.c -o lcd1.o

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


