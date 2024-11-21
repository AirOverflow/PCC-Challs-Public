#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# -*- author: @TheFlash2k -*-

MSG = "I might've messed up here and there, and the buf is something which I absolutely dont know.."
SHIFT_KEY = 1
ROT_KEY = 0x2A

def encrypt(flag:str) -> list:
    enc = []
    for i in flag:
        enc.append((ord(i) >> SHIFT_KEY - 1) ^ ROT_KEY)
    return enc

flag = "PCC{the_sn4k3_and_th3_byt3s_don7_g0_w3ll_togeth3r}\nNOTE: I kept the flag to make it easy ;mmm"
buf = [122, 105, 105, 81, 94, 66, 79, 117, 89, 68, 30, 65, 25, 117, 75, 68, 78, 117, 94, 66, 25, 117, 72, 83, 94, 25, 89, 117, 78, 69, 68, 29, 117, 77, 26, 117, 93, 25, 70, 70, 117, 94, 69, 77, 79, 94, 66, 25, 88, 87]
input("What is your name?")
print("Nope.")
