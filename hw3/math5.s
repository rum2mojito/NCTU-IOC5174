mov eax, [0x600004]
mov ecx, [0x600000]
neg eax
cdq
imul ecx
mov ecx, eax

mov eax, [0x600008]
sub eax, ebx
mov ebx, eax
mov eax, ecx
cdq
idiv ebx
mov [0x600008], eax
done:
