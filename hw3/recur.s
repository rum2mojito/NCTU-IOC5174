mov rcx, 20
mov rax, 0
mov [0x600000], rax
mov rbx, 1
mov [0x600008], rbx
loop_start:
    ; 2*r(n-1)
    mov rax, [0x600008]
    mov rdx, 2
    mul rdx
    mov rbx, rax

    ; 3*r(n-2)
    mov rax, [0x600000]
    mov rdx, 3
    mul rdx
    add rax, rbx
    mov rbx, [0x600008]
    mov [0x600000], rbx
    mov [0x600008], rax
    loop loop_start
done:
