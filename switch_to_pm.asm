[bits 16]

; switches to protected mode
switch_to_pm:
    cli ; disable interrupts

    lgdt [gdt_descriptor] ; load GDT

    ; switch to protected mode
    mov eax, cr0
    or al, 0x1
    mov cr0, eax
    
   jmp CODE_SEG:init_prot  ; far jump, puts the right data in code segment
                           ; this is important since all addresses go through GDT
                           ; in protected mode


[bits 32]

init_prot:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM

