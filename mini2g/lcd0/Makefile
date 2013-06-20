
CROSS_COMPILE = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

all : rockbox.ipod

rockbox.ipod : rockloader lcd0.bin
	./rockloader lcd0.bin rockbox.ipod

lcd0.bin : startup.o lcd0.o memmap
	$(CROSS_COMPILE)-ld startup.o lcd0.o -T memmap -o lcd0.elf  
	$(CROSS_COMPILE)-objdump -D lcd0.elf > lcd0.list
	$(CROSS_COMPILE)-objcopy lcd0.elf -O binary lcd0.bin

startup.o : startup.s
	$(CROSS_COMPILE)-as $(AOPS) startup.s -o startup.o

lcd0.o : lcd0.c ifont.h
	$(CROSS_COMPILE)-gcc -c $(COPS) lcd0.c -o lcd0.o

rockloader : rockloader.c
	gcc rockloader.c -o rockloader

showfont : showfont.c
	gcc showfont.c -o showfont

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.list
	rm -f rockloader
	rm -f rockbox.ipod
	rm -f showfont

