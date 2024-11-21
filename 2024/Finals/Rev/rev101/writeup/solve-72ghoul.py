from z3 import *
from pwn import *

flag = [BitVec(f'flag_{i}', 64) for i in range(80)]

s= Solver()

allowed_chars = list(range(ord('a'), ord('z') + 1)) + \
                list(range(ord('A'), ord('Z') + 1)) + \
                list(range(ord('0'), ord('9') + 1)) + \
                [ord('{'), ord('}'), ord('_'), ord('@'), ord('#')]

for char in flag:
    s.add(Or([char == c for c in allowed_chars]))
with open("aaa.bin","rb") as f:
    data = f.read()
    f.close()
with open("second.bin","rb") as f:
    second = f.read()
    f.close()
for i in range(80):
    s.add(flag[i] >= 0)
    s.add(flag[i] <= 255)
def xor_func(a, b):
    return a^b & 0xFFFFFFFF
def add_func(a,b):
    return a+b & 0xFFFFFFFF
def subtract_func(a,b):
    return a-b & 0xFFFFFFFF
def access_val_func(a):
    global data
    tt = data[a*4:(a*4)+4]
    value = int.from_bytes(tt, byteorder='little', signed=False)
    if value >= 0x80000000:  
        value -= 0x100000000
    #handle neg
    return value
def right_shift_func(a,b):
    return a>>b & 0xFFFFFFFF
def left_shift_func(a,b):
    return a<<b & 0xFFFFFFFF
def access_from_arr(data2,index):
    tt = data2[index*4:(index*4)+4]
    value = int.from_bytes(tt, byteorder='little', signed=False)
    if value >= 0x80000000:  
        value -= 0x100000000
    return value
def neg_func(a):
    return -a & 0xFFFFFFFF
def xor_and_some_other_stuff(index,constant):
    val1 = (flag[index] + constant) % 255
    val2 = flag[index]
    temp = xor_func(val2 >> 2,val1)
    return right_shift_func(temp,2)
def or_func(a):
    a += 1
    b = 0xFFFFFF06
    return a|b & 0xFFFFFFFF
def xor_compare(index):
    for i in range(5):
        calc = xor_func(flag[index+i],i+63)
        s.add(calc == access_val_func(index+i))
    return 



v2 = flag[0];
s.add(v2 == (access_val_func(0) & 0xE0) - 16 )
v3 = flag[1];
v4 = add_func( v3, 16);
s.add(v4 == access_val_func(1) )
v5 = flag[2];
v6 = subtract_func( 105, v5);
s.add(v6 == access_val_func(2) )
v8 = flag[3];
v9 = right_shift_func( v8, 2);
v10 = xor_func( v9, 255);
v11 = right_shift_func( v10, 2);
s.add(v11 == access_val_func(3) )
v12 = flag[4];
v13 = access_val_func(4);
s.add(v12 == xor_func( v13, 63) )
v14 = flag[5];
s.add(v14 == access_val_func(5) )

xor_compare(7)

for i in range(0,15):
    index = access_from_arr(second,i)
    print(index)
    v14 = flag[index]
    v2 = flag[1]
    v2 = flag[2]
    v2 = flag[3]
    v2 = flag[4]
    v2 = flag[5]
    v6 = access_val_func(index)
    v7 = neg_func(v6)
    v8 = xor_func(v7, 21)
    v9 = left_shift_func(v8, 4)
    v10 = right_shift_func(v9, 8)
    v11 = xor_func(v10, 99)
    s.add(v14 == subtract_func(v11, 5))

for i in range(13,17):
    tt = xor_and_some_other_stuff(i,208)
    s.add(tt == access_val_func(i))

v16 = flag[18]
v17 = left_shift_func( v16, 2)
v18 = or_func( v17)
v19 = neg_func(v18)
s.add(v19 == access_val_func(18) )


v16 = flag[18]
v17 = left_shift_func( v16, 2)
v18 = or_func(v17)
v19 = neg_func(v18)
s.add(v19 == access_val_func(18) )

v20 = flag[19]
v21 = xor_func( v20, 160)
s.add(v21 == access_val_func(19) )
v23 = flag[21]
v24 = flag[20]
v25 = xor_func( v24, v23)
s.add(v25 == access_val_func(21) )

v26 = flag[22]
v27 = flag[21]
v28 = xor_func( v27, v26)
s.add(v28 == access_val_func(22) )

v29 = flag[23]
v30 = flag[22]
v31 = xor_func( v30, v29)
s.add(v31 == access_val_func(23))
v32 = flag[24]
v33 = flag[23]
v34 = xor_func( v33, v32)
s.add(v34 == access_val_func(24))

v36 = xor_and_some_other_stuff( 26, 230)
s.add(v36 == access_val_func(26) )

v37 = xor_and_some_other_stuff( 27, 16)
s.add(v37 == access_val_func(27) )

v38 = xor_and_some_other_stuff( 28, 84)
s.add(v38 == access_val_func(28) )

v39 = xor_and_some_other_stuff( 29, 240)
s.add(v39 == access_val_func(29) )

v40 = xor_and_some_other_stuff( 30, 38)
s.add(v40 == access_val_func(30) )

v41 = xor_and_some_other_stuff( 31, 98)
s.add(v41 == access_val_func(31) )

v43 = flag[0]
v44 = flag[33]
v45 = xor_func( v44, v43)
s.add(v45 == access_val_func(33) )

v46 = flag[1]
v47 = flag[34]
v48 = xor_func( v47, v46)
s.add(v48 == access_val_func(34) )

v49 = flag[2]
v50 = flag[35]
v51 = xor_func( v50, v49)
s.add(v51 == access_val_func(35) )

v53 = flag[13]
v54 = flag[37]
v55 = xor_func( v54, v53)
s.add(v55 == access_val_func(37) )

v56 = flag[14]
v57 = flag[38]
v58 = xor_func( v57, v56)
s.add(v58 == access_val_func(38) )

v59 = flag[15]
v60 = flag[39]
v61 = xor_func( v60, v59)

s.add(v61 == access_val_func(39))
v62 = flag[16]
v63 = flag[40]
v64 = xor_func( v63, v62)

s.add(v64 == access_val_func(40))

v66 = flag[42]
v67 = right_shift_func( v66, 2)
v68 = xor_func( v67, 105)
s.add(v68 == access_val_func(42) )

v69 = flag[43]
v70 = right_shift_func( v69, 2)
v71 = xor_func( v70, 24)
s.add(v71 == access_val_func(43) )

v72 = flag[44]
v73 = right_shift_func( v72, 2)
v74 = xor_func( v73, 239)
s.add(v74 == access_val_func(44))
v75 = flag[45]
v76 = right_shift_func( v75, 2)
v77 = xor_func( v76, 154)
s.add(v77 == access_val_func(45))

v79 = flag[0]
v80 = flag[47]
v81 = xor_func( v80, v79)
s.add(v81 == access_val_func(47) )
v82 = flag[1]
v83 = flag[48]
v84 = xor_func( v83, v82)
s.add(v84 == access_val_func(48) )
v86 = flag[12]
v87 = flag[50]
v88 = xor_func( v87, v86)
s.add(v88 == access_val_func(50) )
v89 = flag[13]
v90 = flag[51]
v91 = xor_func( v90, v89)
s.add(v91 == access_val_func(51) )
v92 = flag[14]
v93 = flag[52]
v94 = xor_func( v93, v92)
s.add(v94 == access_val_func(52))
v95 = flag[15]
v96 = flag[53]
v97 = xor_func( v96, v95)
s.add(v97 == access_val_func(53))


v99 = xor_and_some_other_stuff(55, 154)
s.add(v99 == access_val_func(55) )

v100 = xor_and_some_other_stuff(56, 133)
s.add(v100 == access_val_func(56) )

v101 = xor_and_some_other_stuff(57, 30)
s.add(v101 == access_val_func(57) )

v102 = xor_and_some_other_stuff(58, 188)
s.add(v102 == access_val_func(58) )

xor_compare(60)

xor_compare(66)

v104 = flag[0]
v105 = flag[72]
v106 = xor_func( v105, v104)
s.add(v106 == access_val_func(72) )

v107 = flag[1]
v108 = flag[73]
v109 = xor_func( v108, v107)
s.add(v109 == access_val_func(73) )

v111 = flag[75]
v112 = right_shift_func( v111, 2)
v113 = xor_func( v112, 33)Due to shortage of time, I did not write an official writeup. However, I knew 72ghoul would solve this so I'm uploading his solve as "official" writeup :D
s.add(v113 == access_val_func(75) )

v114 = flag[76]
v115 = right_shift_func( v114, 2)
v116 = xor_func( v115, 159)
s.add(v116 == access_val_func(76) )

v117 = flag[77]
v118 = right_shift_func( v117, 2)
v119 = xor_func( v118, 46)
s.add(v119 == access_val_func(77))
v120 = flag[78]
v121 = right_shift_func( v120, 2)
v122 = xor_func( v121, 255)
s.add(v122 == access_val_func(78))

v124 = xor_and_some_other_stuff( 79, 239);
v125 = add_func(v124, 30)
v126 = subtract_func(v125, 52)
v127 = left_shift_func(v126, 5)
s.add(v127 == access_val_func(79))
s.add(flag[3] == ord("{"))



while s.check() == sat:
    m = s.model()
    solution = [m[flag[i]].as_long() for i in range(80)]
    print(bytes(solution))

    block = []
    for i in range(80):
        block.append(flag[i] != m[flag[i]])
    s.add(Or(block))

    break
else:
    print("dead")
