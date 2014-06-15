; TODO: what is SEG_NULLASM and the macro for? how do macros work?
%define SEG_NULLASM dq 0

%macro SEG_ASM 3 ; Type, Base, Lim
        dw (((%3) >> 12) & 0xffff)
        dw ((%2) & 0xffff)
        db (((%2) >> 16) & 0xff)
        db (0x90 | (%1))
        db (0xC0 | (((%3) >> 28) & 0xf))
        db (((%2) >> 24) & 0xff)
%endmacro


; TODO: what are these for? they are used under gdt label
%define STA_X     0x8  ; Executable segment
%define STA_E     0x4  ; Expand down (non-executable segments)
%define STA_C     0x4  ; Conforming code segment (executable only)
%define STA_W     0x2  ; Writeable (non-executable segments)
%define STA_R     0x2  ; Readable (executable segments)
%define STA_A     0x1  ; Accessed


; TODO: figure out what these are for
%define SEG_KCODE 1  ; kernel Code
%define SEG_KDATA 2  ; kernel Data+Stack

use16

global start ; TODO: what does global do?

extern bootmain ; TODO: what does extern do? i believe bootmain is the function in the C code

start:
    cli ; disable interrupts

    ; zero out the segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; TODO: what exactly does this do
    lgdt   [gdtdesc]

    ; switch to protected mode (see Intel system programming manual
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp    (SEG_KCODE<<3):start32 ; what does this do?

start32:
use32
  ; wat
  mov    eax, SEG_KDATA<<3
  mov    ds, eax
  mov    es, eax
  mov    ss, eax
  mov    ax, 0
  mov    fs, eax
  mov    gs, eax


; Something about clearing the screen
  mov edi, 0xb8000
  mov ecx, 80*25*2
  mov al, 0
  rep stosb


; TODO: figure out what call does
  mov    esp, start
  call   bootmain


; TODO: figure out what this whole section does
gdt:
    ; null entry
    SEG_NULLASM
    ; code entry
    SEG_ASM STA_X | STA_R, 0x0, 0xffffffff
    SEG_ASM STA_W,         0x0, 0xffffffff

gdtdesc:
    dw (gdtdesc - gdt - 1) ; Size
    dd gdt ; Offset
