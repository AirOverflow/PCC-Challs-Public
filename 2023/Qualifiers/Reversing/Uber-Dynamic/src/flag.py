#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# -*- author: @TheFlash2k -*-

## Generates a flag.h file with the flag

with open("/flag.txt") as f:
    flag = f.read().strip()

# Dividing the flag into two portions:
# BASIC AF -> char to int...
flag_1 = [ord(i) for i in flag[:len(flag)//2]]
flag_2 = [ord(i) for i in flag[len(flag)//2:]]

# Converting to string, then replaceing [ with {, if you know a better way, lmk...
flag_1 = str(flag_1).replace("[", "{").replace("]", "}")
flag_2 = str(flag_2).replace("[", "{").replace("]", "}")

var_1 = "flag_p1"
var_2 = "flag_p2"

c_src = """
int {var_1}[] = {flag_1};
int {var_2}[] = {flag_2};
"""

with open("flag.h", "w") as f:
    f.write(c_src.format(var_1=var_1, var_2=var_2, flag_1=flag_1, flag_2=flag_2))

print("[*] Flag generated!")