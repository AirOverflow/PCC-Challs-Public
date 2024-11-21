#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# -*- author: @TheFlash2k -*-

import qrcode
from PIL import Image

IMAGE_NAME = "challenge.png"
CHAL_FILE = "dist/challenge.txt"

def get_flag() -> str:
    try:
        with open("/flag.txt", "r") as f:
            return f.read()
    except:
        return "Call @TheFlash2k now. Good Job. The flag didn't exist on the remote instance."

def generate_qr(flag: str) -> Image:
    qr = qrcode.QRCode(version=3, box_size=5, border=1, error_correction=qrcode.constants.ERROR_CORRECT_H)
    qr.add_data(flag)
    qr.make(fit=True)
    img = qr.make_image(fill_color="black", back_color="white")
    img.save(IMAGE_NAME)
    return img

def generate_challenge():

    img = Image.open(IMAGE_NAME)
    pixels = img.load()
    _p = []

    for i in range(img.size[0]):
        for j in range(img.size[1]):
            _p.append(pixels[i, j].__str__().replace("255", "1"))

    pixels = "".join(_p)

    with open(CHAL_FILE, "w") as f:
        f.write("I love Cheese and Quesadillas. But I'm scared about the bill. The receipt and the amount might be too much for me to handle. Can you help me?\n")
        f.write(pixels)
        f.write("\nI knew it would be that much.. :(((")

if __name__  == "__main__":
    flag = get_flag()
    generate_qr(flag)
    generate_challenge()