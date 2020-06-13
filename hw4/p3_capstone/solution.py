from capstone import *

CODE = "1001000100001011101111011111100010010000010100111111000010010000011000111110110010010000000100111010110".encode('utf-8')

md = Cs(CS_ARCH_X86, CS_MODE_64)

for i in md.disasm(CODE, 0x1000):
    print("0x%x:\t%s\t%s" %(i.address, i.mnemonic, i.op_str))
