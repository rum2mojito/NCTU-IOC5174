L1:
    cmp eax, 0
    jae L1_t
    jmp L1_f

L1_t:
    mov eax, 1
    mov [0x600000], eax
    jmp L2

L1_f:
    mov eax, -1
    mov [0x600000], eax
    jmp L2

L2:
    cmp ebx, 0
    jae L2_t
    jmp L2_f

L2_t:
    mov eax, 1
    mov [0x600004], eax
    jmp L3

L2_f:
    mov eax, -1
    mov [0x600004], eax
    jmp L3

L3:
    cmp ecx, 0
    jae L3_t
    jmp L3_f

L3_t:
    mov eax, 1
    mov [0x600008], eax
    jmp L4

L3_f:
    mov eax, -1
    mov [0x600008], eax
    jmp L4

L4:
    cmp edx, 0
    jae L4_t
    jmp L4_f

L4_t:
    mov eax, 1
    mov [0x60000c], eax

L4_f:
    mov eax, -1
    mov [0x60000c], eax
done: