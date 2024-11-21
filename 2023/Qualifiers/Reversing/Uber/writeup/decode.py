#!/usr/bin/env python3

flag_p1 = [0x5d,0x4e,0x4e,0x76,0x69,0x7f,0x3c,0x7b,0x3e,0x7f,0x52,0x61,0x3d,0x6c,0x69,0x3c]
flag_p2 = [0x63,0x6a,0x52,0x3c,0x7e,0x52,0x65,0x39,0x7f,0x69,0x52,0x6b,0x7f,0x6b,0x7f,0x70]
key = 0x0d

flag = ''.join([chr(i ^ key) for i in flag_p1 + flag_p2])

print(flag)