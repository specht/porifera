bits 16
org 0x7c00 ; "Tell NASM that this code will be loaded at 0x7C00
             ; to ensure any absolute jumps are calculated correctly"
             ; I don't really understand what this means.

start:
    xor ax, ax
    mov ds, ax ; ds is used in lodsb, so needs to be cleared. I guess?
    mov es, ax ; xv6 bootloader does this. does it need to be cleared here?

    mov si, VerySpecialWords ; why si? it's used by lodsb
    mov bx, 0x00
    mov ah, 0x0e ; this sets "teletype mode". need to figure out what the HECK

print:
    lodsb ; whatever ESI points to into AL, increments ESI by 1
    cmp al, 0
    je end

    int 0x10
; "Lastly, we need to know how to print to the screen. You can either write directly into video memory at 0xB8000 - 0xBFFFF, or you can use BIOS Interrupt 0×10. Interrupts take their paramters in processor registers (small areas of 32-bit memory used by programmers to hold variables). INT 0×10 takes the mode in AH, the page in BH, the colour in BL and the character itself (in ASCII) in AL."

    jmp print

end:
    cli ; disables interrupts
    hlt


VerySpecialWords:
    db 'This is my kernel. '
    db 'There are many like it, but this one is mine.'
    db 0   ; the zero here is a trailing null character?


times 510 - ($ - $$) db 0 ; i saw this in multiple bootloaders.
dw 0xAA55 ; the hardware checks the last 2 bytes of the boot sector to confirm that its an MBR. like a secret handshake

