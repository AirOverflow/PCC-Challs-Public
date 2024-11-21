#!/usr/bin/env python3
# Author: @TheFlash2k

from string import ascii_lowercase as printables
import random


# ACTUAL_FLAG = "PCC{th3se_b1ts_g0t_m3_cr4zyy}"
with open("/app/flag.txt") as f:
	ACTUAL_FLAG = f.read().strip()
	
FLAG = "Nope. Not the flag, or is it?..."

## Setting up the flag so it doesn't show up in strings:
for i in range(len(ACTUAL_FLAG)):
	FLAG += ACTUAL_FLAG[i]
	if i % 3 == 0:
		FLAG += '\n' ## Some very weird thing which honestly, idk either...

INC = 5

flags = []
iter = 0
for ii in FLAG:
	rd = random.choices(printables, k=INC)
	__b = ord(ii) + INC
	ii = chr(__b)
	if ii == '\x7f' or ii == '\x82' or ii == '\x80':
		ii = hex(__b)
	else:
		ii = f"'{ii}'"
	__str = ""
	for i in range(len(rd)):
		__str += f"'{rd[i]}' - '{rd[i]}' + "
	__str += f"{ii} - {INC}"
	flags.append(f"\tflag[{iter}] = {__str};")
	iter += 1

with open("flag.h", "w") as f:
	data = "#include <stdio.h>\nchar flag[31];\nvoid build_flag(){\n"
	f.write(data)
	for it in flags:
		f.write(it + "\n")
	f.write("}")
