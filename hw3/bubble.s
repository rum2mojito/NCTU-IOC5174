mov ecx, 9
outer_loop:
    mov edx, ecx
    mov ecx, 9
    mov esi, 0x600000
    inner_loop:
        mov eax, [esi]
        mov ebx, [esi+4]
        cmp ebx, eax
        jae NEXT
        mov [esi], ebx
        mov [esi+4], eax
        NEXT:
            add esi, 0x4
            loop inner_loop
    mov ecx, edx
    loop outer_loop
            
done: