all: os-image floppy
	qemu-system-i386 os-image
	rm -f floppy.img

floppy: os-image
	rm floppy.img; mkdosfs -C floppy.img 1440
	dd status=noxfer conv=notrunc if=os-image of=floppy.img

os-image: kernel boot
	cat boot_sect.bin kernel.bin > os-image

kernel: kernel.c entry.asm
	nasm entry.asm -f elf64 -o entry.o
	gcc -ffreestanding -mfpmath=387 -mno-sse -c kernel.c -o kernel.o
	ld -o kernel.bin -Ttext 0x1000 entry.o kernel.o --oformat binary

boot: boot.asm
	nasm boot.asm -o boot_sect.bin
