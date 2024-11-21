#!/usr/local/bin/python

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from hashlib import sha256

import os


def get_flag():
    try:
        with open("/flag.txt", "rb") as f:
            FLAG = f.read().strip()
        return FLAG
    except:
        print("[ERROR] - Please contact an Administrator.")
flag = get_flag()


class AESCipher:
    def __init__(self, key):
        self.key = key

    def encrypt(self, pt):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return cipher.encrypt(pad(pt, AES.block_size))

    def decrypt(self, enc):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return unpad(cipher.decrypt(enc), AES.block_size)


key1 = sha256(os.urandom(3)).digest()
key2 = sha256(os.urandom(3)).digest()


aes1 = AESCipher(key1)
aes2 = AESCipher(key2)
pt = b'PCCCCCCCCCCCCCCC'

ct = aes2.encrypt(aes1.encrypt(pt))

pt = aes1.decrypt(aes2.decrypt(ct))

print(ct.hex())


print(f'pt = {pt.hex()}\nct = {ct.hex()}')

key = sha256(key1 + key2).digest()

aes = AESCipher(key)
enc_flag = aes.encrypt(flag)

print(f'enc_flag = {enc_flag.hex()}')
