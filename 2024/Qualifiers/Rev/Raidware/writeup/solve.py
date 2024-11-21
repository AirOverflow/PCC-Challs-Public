#!/usr/bin/env python3
from pwn import *

class RC4:
    """
    This class implements the RC4 streaming cipher.
    
    Derived from http://cypherpunks.venona.com/archive/1994/09/msg00304.html
    """

    def __init__(self, key, streaming=True):
        assert(isinstance(key, (bytes, bytearray)))

        # key scheduling
        S = list(range(0x100))
        j = 0
        for i in range(0x100):
            j = (S[i] + key[i % len(key)] + j) & 0xff
            S[i], S[j] = S[j], S[i]
        self.S = S

        # in streaming mode, we retain the keystream state between crypt()
        # invocations
        if streaming:
            self.keystream = self._keystream_generator()
        else:
            self.keystream = None

    def crypt(self, data):
        """
        Encrypts/decrypts data (It's the same thing!)
        """
        assert(isinstance(data, (bytes, bytearray)))
        keystream = self.keystream or self._keystream_generator()
        return bytes([a ^ b for a, b in zip(data, keystream)])

    def _keystream_generator(self):
        """
        Generator that returns the bytes of keystream
        """
        S = self.S.copy()
        x = y = 0
        while True:
            x = (x + 1) & 0xff
            y = (S[x] + y) & 0xff
            S[x], S[y] = S[y], S[x]
            i = (S[x] + S[y]) & 0xff
            yield S[i]

io = remote("localhost", 32769)

PREPEND  = "||RAIDWARE-SoM||"
POSTPEND = "||RAIDWARE-EoM||"
KEY = b"r41dw4re_k3y_f0r_c0mm5"

def do_enc(s):
	return RC4(KEY).crypt(f"{PREPEND}{s}{POSTPEND}".encode())

def do_dec(b):
	s = RC4(KEY).crypt(b)
	s = s.replace(PREPEND.encode(), b"")
	s = s[:s.index(POSTPEND.encode())]
	return s.decode()

def send(msg):
	io.sendline(do_enc(msg))

def recv():
	return do_dec(io.recv())

send("init_raidware")
print(f"Response: {recv()}")

send("Hash|b7259e73a31fdb3fb50bd752c423eb86")
print(f"Response: {recv()}")

send("flag")
print(f"Response: {recv()}")
