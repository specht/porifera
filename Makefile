all: os-image
	qemu-system-i386 os-image

os-image: kernel boot
	cat boot_sect.bin kernel.bin > os-image

kernel: kernel.rs entry.asm
	nasm entry.asm -f elf64 -o entry.o
	rustc -Z no-landing-pads --crate-type=lib --emit=obj -o kernel.o kernel.rs
	ld -o kernel.bin -Ttext 0x1000 entry.o kernel.o --oformat binary

boot: boot.asm
	nasm boot.asm -o boot_sect.bin
