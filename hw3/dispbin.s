mov ecx, 16
mov edx, 0x600000
loop_start:
    mov bx, ax
    and bx, 0x8000
    shr bx, 15
    or bx, 0x30
    mov [edx], bx
    add edx, 0x1
    shl ax, 0x1
    loop loop_start
done: