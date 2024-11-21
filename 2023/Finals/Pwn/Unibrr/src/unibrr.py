#!/usr/bin/env python3

from unicorn import *
from unicorn.x86_const import *

ADDRESS = 0x1000000
def get_flag():
    try:
        with open("/flag") as f:
            FLAG = f.read().strip()
            print(f"Well done: {FLAG}")
    except:
        print("[ERROR] - Please contact an Administrator.")

def hook_code(uc, address, size, user_data):
    print(">>> Tracing instruction at 0x%x, instruction size = 0x%x" %(address, size))

def hook_mem_invalid(uc, access, address, size, value, user_data):
    if access == UC_MEM_WRITE_UNMAPPED:
        print(">>> Missing memory is being WRITTEN at 0x%x, data size = %u, data value = 0x%x" %(address, size, value))
        uc.mem_map(0xaaaa0000, 2 * 1024 * 1024)
        return True
    else:
        print(">>> Missing memory is being READ at 0x%x, data size = %u, data value = 0x%x" %(address, size, value))
        uc.mem_map(0xaaaa0000, 2 * 1024 * 1024)
        return True
print("Welcome to abdullahzamir's secure code emulator.")
try:
    X86_CODE32 = bytes.fromhex(input("Enter the code you want to emulate: "))
    print("x86 code: ", X86_CODE32)
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    mu.mem_map(ADDRESS, 2 * 1024 * 1024)
    mu.mem_write(ADDRESS, X86_CODE32)
    mu.reg_write(UC_X86_REG_EAX, 0x1337)
    mu.hook_add(UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED, hook_mem_invalid)
    mu.hook_add(UC_HOOK_CODE, hook_code)
    mu.emu_start(ADDRESS, ADDRESS + len(X86_CODE32))
    r_eax = mu.reg_read(UC_X86_REG_EAX)
    print(">>> EAX = 0x%x" %r_eax)

    if(r_eax == 0xdeadbeef):
        get_flag()

except UcError as e:
    print(f"UNICORN Error: {e}")

except Exception as E:
    print(f"An error occurred: {E}")