
bootblock: boot.asm bootmain.c
	nasm -f elf32 -o boot.o boot.asm
	gcc -fno-pic -static -fno-builtin -fno-strict-aliasing -Wall -MD -ggdb -m32 -Werror -fno-omit-frame-pointer -fno-stack-protector -O -nostdinc -I. -o bootmain.o -c bootmain.c
	# -c disables the linking
	# -O is -O1, optimization
	ld -N -m elf_i386 -e start -Ttext 0x7c00 -o bootblock.o boot.o bootmain.o
	objdump -S bootblock.o > bootblock.asm # disassemble
	objcopy -S -O binary -j .text bootblock.o bootblock.bin
	python bootit.py bootblock.bin
