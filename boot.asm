[org 0x7c00]
KERNEL_OFFSET equ 0x1000

    mov [BOOT_DRIVE], dl ; BIOS stores boot drive in DL

    ; set up a stack since we'll be doing calls
    ; TODO: why this address?
    mov bp, 0x9000
    mov sp, bp

    call load_kernel ; put kernel in memory

    call switch_to_pm

    jmp $ ; jump to this line over and over

%include "disk_load.asm"
%include "gdt.asm"
%include "switch_to_pm.asm"

[bits 16]
load_kernel:
    ; prep for disk_load
    mov bx, KERNEL_OFFSET ; where in memory we are loading the kernel
    mov dh, 1 ; number of sectors
    mov dl, [BOOT_DRIVE] ; letter of the drive? TODO
    call disk_load

    ret

[bits 32]

BEGIN_PM:
    call KERNEL_OFFSET

    jmp $ ; jump to this line over and over

BOOT_DRIVE db 0

; bootsector padding + adding the boot signature
times 510-($-$$) db 0
dw 0xaa55
