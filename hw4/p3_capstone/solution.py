from binascii import unhexlify
from capstone import *
from pwn import *

proc = remote('aup.zoolab.org', 2530)

for m in range(10):
    proc.recvuntil('>>> ')

    line = proc.recvline()[:-1]
    input = unhexlify(line)
    print(line)
    print(input)

    inss = []
    md = Cs(CS_ARCH_X86, CS_MODE_64)
    for i in md.disasm(input, 0x0):
        inss.append(i.mnemonic + ' ' + i.op_str)
    print(inss)

    ret = ''
    for ins in inss:
        ret += ''.join([hex(ord(i))[2:] for i in ins])
        # new line
        ret += '0a'
    print(ret)
    proc.sendlineafter('Your answer: ', ret)

proc.interactive()
