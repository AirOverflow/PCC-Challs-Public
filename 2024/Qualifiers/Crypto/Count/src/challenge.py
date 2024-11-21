#!/usr/local/bin/python3

import random
from Crypto.Util.number import long_to_bytes
from Crypto.Cipher import AES
from Crypto.Util import Counter
from Crypto.Util.Padding import pad

def get_flag():
    try:
        with open("/flag.txt", "rb") as f:
            FLAG = f.read().strip()
        return FLAG
    except:
        print("[ERROR] - Please contact an Administrator.")

key = random.randbytes(16)

def encrypt(data: bytes) -> bytes:
    cipher = AES.new(key, AES.MODE_CTR, counter=Counter.new(128))
    return cipher.encrypt(data)

# you'll have to find this, i guess.
f = open("a_quote_from_a_leader.txt", "rb")
data = f.read()
f.close()

encrypted = encrypt(data)
print(f"Encrypted Data: {encrypted.hex()}")

flag = get_flag()

encrypted_flag = encrypt(flag)
print(f"Encrypted Flag: {encrypted_flag.hex()}")
