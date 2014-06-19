disk_load:
    push dx ; dx contains number of sectors

    mov ah, 0x02 ; BIOS read sector function??
    mov al, dh ; load number of sectors into al
    mov ch, 0x00 ; cylinder 0?
    mov dh, 0x00 ; head 0?
    mov cl, 0x02 ; from second sector?
    int 0x13

    jc disk_error1 ; jump if error (carry flag)

    pop dx
    cmp dh, al ; I guess al now contains number of sectors read? we are checking here if we read as many as expected

    jne disk_error2 ; jump if they arent the same

    ret

disk_error1:
    mov bx, DISK_ERROR_MSG1
    call print_string

    jmp $ ; just bail, man

disk_error2:
    mov bx, DISK_ERROR_MSG2
    call print_string
    jmp $ ; just bail, man


DISK_ERROR_MSG1 db "the interrupt didnt work or whatev. ", 0
DISK_ERROR_MSG2 db "they aint the same size.", 0
