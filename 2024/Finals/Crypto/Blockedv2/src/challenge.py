#!/usr/local/bin/python3
from pwn import xor


def get_flag():
    try:
        with open("/flag.txt", "rb") as f:
            FLAG = f.read().strip()
        print(FLAG)
    except:
        print("[ERROR] - Please contact an Administrator.")

y= input("> ")

blocks = [y[i:i+4] for i in range(0, len(y), 4)]
assert len(blocks) == len(set(blocks))

xor_block = 0
for block in blocks:
    xor_block = xor(block, xor_block)
assert xor_block == b'0000'

x = input("> ")
blocks = [int(x[i:i+2]) for i in range(0, len(x), 2)]
assert len(blocks) == len(set(blocks))
hash_block = 0
for block in blocks:
    hash_block^=hash(block)
assert hash_block==0

        
get_flag()

