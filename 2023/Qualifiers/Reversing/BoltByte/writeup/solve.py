#!/usr/bin/env python3
# Author: @TheFlash2k

import subprocess

_IN = "boltbyte"
_OUT = "flag.elf"

with open(_IN, 'rb') as f:
	data = f.read()

## Reversing the bits:
data = data[::-1]

## -1 from each bit:
buf = b''

for i in range(0, len(data)):
	buf += ((data[i] - 1) % 256 if i % 2 == 0 else data[i]).to_bytes()

print(f"[*] Writing {len(buf)} bytes to {_OUT}", end='')
with open(_OUT, "wb") as f:
	f.write(buf)

print(": Done.")

''' Single solution file to print the flag as well.. '''
print("[*] Granting permissions and executing...")
subprocess.run(["chmod", "+x", _OUT])
subprocess.run([f"./{_OUT}"])
