all:
	nasm boot.asm
	/usr/bin/qemu-system-i386 boot
