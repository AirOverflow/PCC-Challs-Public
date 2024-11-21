#!/usr/local/bin/python3

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import os

class AESCipher:
    def __init__(self, key):
        self.key = key

    def encrypt(self, pt):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return cipher.encrypt(pad(pt, AES.block_size))

    def decrypt(self, enc):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return unpad(cipher.decrypt(enc), AES.block_size)


def menu():
    print("1. Encrypt")
    print("2. Get Flag")
    print("3. Exit")
    return int(input("Enter your choice: "))


key = os.urandom(16)

while True:
    choice = menu()
    if choice == 1:
        pt = input("Enter the plaintext: ")
        if 'PCC' in pt:
            print("You shall not pass!")
            continue
        cipher = AESCipher(key)
        print("Ciphertext:", cipher.encrypt(pt.encode()).hex())
    elif choice == 2:
        enc = bytes.fromhex(input("Enter the ciphertext: "))
        cipher = AESCipher(key)
        try:
            pt = cipher.decrypt(enc).decode()
            if 'PCC' in pt:
                print(open('/flag.txt').read())
        except:
            print("Invalid ciphertext")
    elif choice == 3:
        break
        

