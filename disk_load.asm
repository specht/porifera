disk_load:
    push dx ; dx contains number of sectors

    mov ah, 0x02 ; BIOS read sector function??
    mov al, dh ; load number of sectors into al
    mov ch, 0x00 ; cylinder 0?
    mov dh, 0x00 ; head 0?
    mov cl, 0x02 ; from second sector?
    int 0x13

    jc disk_error ; jump if error (carry flag)

    pop dx
    cmp dh, al ; I guess al now contains number of sectors read? we are checking here if we read as many as expected

    jne disk_error ; jump if they arent the same

    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string

    jmp $ ; just bail, man

DISK_ERROR_MSG db "There was a disk error. Sorry", 0
