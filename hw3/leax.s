mov eax, edi
mov edx, 9
mul edx
mov [0x600000], eax

mov eax, edi
mov ecx, 5
mul ecx
mov ecx, eax

mov eax, edi
mov ebx, 3
mul ebx
mov ebx, eax

mov eax, 2
mul edi
mov edx, [0x600000]
done: