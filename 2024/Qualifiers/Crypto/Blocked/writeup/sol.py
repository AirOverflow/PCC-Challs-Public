from pwn import *

if args.REMOTE:
    p = remote(sys.argv[1], int(sys.argv[2]))
else:
    p = process(['python3', '../src/challenge.py'])

p.sendlineafter(b': ', b'1')
p.sendlineafter(b': ', b'AAAAAAAAAAAAAAPC')
p.recvuntil(b': ')
part1 = p.recvline().decode()[:32]
print(part1)

p.sendlineafter(b': ', b'1')
p.sendlineafter(b': ', b'C')
p.recvuntil(b': ')
part2 = p.recvline().decode()

p.sendlineafter(b': ', b'2')
p.sendlineafter(b': ', part1 + part2)
flag = p.recvline()
print(flag)
p.close()

