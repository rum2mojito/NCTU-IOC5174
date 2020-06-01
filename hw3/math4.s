mov eax, [0x600004]
neg eax
cdq
mov ebx, [0x600008]
idiv ebx
mov ebx, edx
mov eax, 5
neg eax
mov ecx, [0x600000]
cdq
imul ecx
cdq
idiv ebx
mov [0x60000c], eax
done:
