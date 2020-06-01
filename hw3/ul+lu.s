mov ah, ch
and ah, 0x20
cmp ah, 0x20
je LOWER
jmp UPPER

LOWER:
    and ch, 0xef
    jmp END

UPPER:
    or ch, 0x20
    jmp END

END:

done:
