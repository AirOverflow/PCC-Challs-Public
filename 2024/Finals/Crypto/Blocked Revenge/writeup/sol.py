from pwn import *

if args.REMOTE:
    p = remote(sys.argv[1], int(sys.argv[2]))
else:
    p = process(['python3', '../src/challenge.py'])


p.sendlineafter(b'> ', b'111A1113000B')

p.sendlineafter(b'< ', b"-1-2")
flag = p.recvline()
print(flag)
p.close()

