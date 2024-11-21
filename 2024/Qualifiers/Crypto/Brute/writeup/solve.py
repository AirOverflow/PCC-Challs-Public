from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from hashlib import sha256
import os
from tqdm import trange
class AESCipher:
    def __init__(self, key):
        self.key = key

    def encrypt(self, pt):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return cipher.encrypt(pad(pt, AES.block_size))

    def decrypt(self, enc):
        cipher = AES.new(self.key, AES.MODE_ECB)
        return unpad(cipher.decrypt(enc), AES.block_size)


pt = b'PCCCCCCCCCCCCCCC'

conflict = dict()


for i in trange(256*256*256):
    key1 = sha256(i.to_bytes(3, 'big')).digest()
    aes1 = AESCipher(key1)
    enc = aes1.encrypt(pt)
    conflict[enc] = key1
    

conflict2 = dict()
    


ct = "7bc057243a259085fb9b3f2941d1ba1f650837965eeb2591e9a95aa6435d150d0d423477805581459125fcc7214fa88a"
ct = bytes.fromhex(ct)
print(ct)
conflict2 = dict()
for i in trange(256*256*256):
    key2 = sha256(i.to_bytes(3, 'big')).digest()
    aes2 = AESCipher(key2)
    try:
        dec = aes2.decrypt(ct)
    except:
        continue
    if dec in conflict:
        key1 = conflict[dec]
        key2 = key2
        key = sha256(key1 + key2).digest()
        break
        

print(key)

enc_flag = "2deeb7dc14afcc3ac9145b33a351bd558860f3a86f068f7d1920812b92b0db04094c700b0bef0a0d53a134ec04d52396"
enc_flag = bytes.fromhex(enc_flag)
aes = AESCipher(key)
flag = aes.decrypt(enc_flag)

print(f'flag = {flag}')
