mov eax, [0x600000]
mov ebx, 5
mul ebx
mov ebx, [0x600004]
mov ecx, 3
sub ebx, ecx
div ebx
mov [0x600008], eax
done: