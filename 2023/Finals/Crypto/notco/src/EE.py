#!/usr/bin/env python3

from Crypto.Util.number import getPrime, bytes_to_long, long_to_bytes, inverse


def get_flag():
    try:
        with open("/flag") as f:
            FLAG = f.read().strip()
        return FLAG
    except:
        print("[ERROR] - Please contact an Administrator.")



def encrypt():
    p = getPrime(1024)
    q = getPrime(1024)
    e = 1336
    n = p*q
    FLAG = get_flag().encode()
    enc = pow(bytes_to_long(FLAG),e,n)

    print(f"e = {e}")
    print(f"p = {p}")
    print(f"q = {q}")
    print(f"c = {enc}")

if __name__ == "__main__":
    print("Welcome to the 2048 bit secure RSA encryption service.")
    print("here's the information:")
    encrypt()

