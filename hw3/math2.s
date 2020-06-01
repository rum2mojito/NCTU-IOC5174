mov eax, [0x600000]
neg eax
mov ebx, [0x600004]
mul ebx
add eax, [0x600008]
done: