mov ecx, 15
mov ebx, 0x600000
mov edx, 0x600010
loop_start:
    mov al, [ebx]
    or al, 0x20
    mov [edx], al
    add ebx, 0x1
    add edx, 0x1
    loop loop_start
done:
