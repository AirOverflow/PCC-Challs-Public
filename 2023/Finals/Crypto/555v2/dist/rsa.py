import random
from Crypto.Util.number import long_to_bytes
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

bits = 128
shares = 30
def get_flag():
    try:
        with open("/flag") as f:
            FLAG = f.read().strip()
        return FLAG
    except:
        print("[ERROR] - Please contact an Administrator.")


def encrypt(flag):
    key = random.randbytes(16)
    cipher = AES.new(key, AES.MODE_ECB)
    enc_flag = cipher.encrypt(pad(flag, 16))
    return enc_flag.hex()

poly = [random.getrandbits(bits) for _ in range(shares)]
flag = get_flag().encode()
random.seed(poly[15])
print("encrypted flag: ", encrypt(flag))
x = int(input("input x: "))
print(sum(map(lambda i: poly[i] * pow(x, i), range(len(poly)))))