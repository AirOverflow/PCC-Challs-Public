#!/usr/bin/env python3

## Flag generator for INTERCAL.
# FLAG = "Here you go, the flag: PCC{1n73rcal_7h3_0g}"

from base64 import b64encode

with open("/app/flag.txt") as f:
	flag = f.read().strip()

flag = b64encode(flag.encode()).decode()

FLAG = f"Sure, you couldn't find out if this was the flag?? {flag}. Wasn't it that easy? I told everyone that all you had to was 'google' ;)"

''' https://github.com/dreamsofcode-io/cursed-hello-world/blob/main/intercal/tools/print.py '''
def minus(x, y):
    return (x - y) & 0xFF

def bitreverse(x):
    return int("{:08b}".format(x)[::-1], 2)

def figureout(xs):
    res = []
    for i in range(0, len(xs)):
        val = ord(xs[i])
        rev = bitreverse(val)
        prev = 256
        if i > 0:
            prev = bitreverse(ord(xs[i - 1]))

        res.append(minus(prev, rev))

    return res

def generate_chal(file_name : str = "UnProunounceable.i"):

    # Array iterator
    BASE = f"DO ,1 SUB #IDX <- #VAL\n" #
    src  = ""
    src += f"DO ,1 <- #{len(FLAG)}\n"
    src += "PLEASE " # Adding this to be "POLITE >,<"
    flag = figureout(FLAG)
    for i in range(0, len(flag)):
        if i % 6 == 0 and i != 0:
            src += "PLEASE "
        src += BASE.replace('IDX', str(i + 1)).replace('VAL', str(flag[i]))

    # src += "PLEASE READ OUT ,1\nPLEASE GIVE UP\n"
    
    with open(file_name, 'w') as f:
        f.write(src)

    print(f"[*] Generated {file_name} with flag {FLAG}")

generate_chal()
