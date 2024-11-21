import random
import time
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from base64 import b64encode, b64decode
from secret import FLAG

KEY_LEN = 32
IV_LEN = 16
BLOCK_SIZE = 256

# generate awesome keys for encryption
def generate_key() -> (bytearray, bytearray):
    # pseudorandom time   
    random.seed(int(time.time()))
    key = random.randbytes(KEY_LEN)
    iv = random.randbytes(IV_LEN)
    return (key, iv)

# do any thing for super awesome encryption
def encrypt(message: str,key: bytearray, iv: bytearray):
	message = pad(message.encode(),block_size = BLOCK_SIZE)
	cipher = AES.new(mode=AES.MODE_CBC, key=key, iv=iv)
	encrypt_text = cipher.encrypt(message)
	return encrypt_text


# meh, do it yourself 
def decrypt(message: str,key: bytearray, iv: bytearray):



if __name__=='__main__':
	key,iv = generate_key()
	message = b64encode(encrypt(FLAG,key,iv)).decode('utf-8')
	print(message)