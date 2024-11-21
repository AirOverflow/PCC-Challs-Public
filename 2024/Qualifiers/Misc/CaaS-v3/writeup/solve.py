#!/usr/bin/env python3

from pwn import *
context.log_level = 'error'

exe = "./chal.py"
io = remote(sys.argv[1], int(sys.argv[2])
	) if args.REMOTE else process([exe], aslr=True)

payload = read("solve.c") + b"\n<<EOF>>"
io.sendlineafter(b"): ", payload)

flag = io.recv(72).decode()
print(f"Flag: PCC{{{flag}}}")