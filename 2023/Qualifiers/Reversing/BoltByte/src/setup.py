#!/usr/bin/env python3

def bit_shift(data: list, inc: int, _start:int = 0, _len: int = 20):
	print(f"=> Shifting every 2-bits by {inc} (Total: {_len})")
	data = [i for i in data]
	_len = _len if _len > 1 else len(data)
	buf = b''
	for i in range(_start, _len):
		if i % 2 == 0:
			bb = (data[i] + inc) % 256
		else:
			bb = data[i]
		buf += bb.to_bytes()
	return buf
		
if __name__ == "__main__":
	EXE = "boltbyte"
	WRITE = f"{EXE}_patched"
	with open(EXE, 'rb') as f:
		data = f.read()
	buf = bit_shift(data, inc=1, _len=-1)
	with open(WRITE, 'wb') as f:
		f.write(buf[::-1])
	print("Success..")
