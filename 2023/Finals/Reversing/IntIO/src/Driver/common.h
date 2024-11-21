#pragma once
#include <ntddk.h>
#include <wdf.h>
#include <stdlib.h>
#include <ntstrsafe.h>
#include <string.h>
#include <stdio.h>

// ************************************
// ******** ALL LOG METHODS ***********
// ************************************
#define BANNER() { DbgPrintEx(0, 0, "============================================\n"); }
#define DEBUG(x, ...) { DbgPrintEx(0, 0, "[DEBUG] ");DbgPrintEx(0, 0, x, __VA_ARGS__);DbgPrintEx(0, 0, "\n"); }
#define LOG(x, ...) { DbgPrintEx(0, 0, "[LOG] ");DbgPrintEx(0, 0, x, __VA_ARGS__);DbgPrintEx(0, 0, "\n"); }
#define ERROR(x, ...) { DbgPrintEx(0, 0, "[ERROR] ");DbgPrintEx(0, 0, x, __VA_ARGS__);DbgPrintEx(0, 0, "\n"); }
#define INFO(x, ...) { DbgPrintEx(0, 0, "[INFO] ");DbgPrintEx(0, 0, x, __VA_ARGS__);DbgPrintEx(0, 0, "\n"); }
#define WARNING(x, ...) { DbgPrintEx(0, 0, "[WARNING] ");DbgPrintEx(0, 0, x, __VA_ARGS__);DbgPrintEx(0, 0, "\n"); }
#define MESSAGE(x, ...) { DbgPrintEx(0, 0, x, __VA_ARGS__);DbgPrintEx(0, 0, "\n"); }
#define MESSAGELN(x, ...) { DbgPrintEx(0, 0, "[*] ");DbgPrintEx(0, 0, x, __VA_ARGS__); }
#define DBGPRINT DEBUG
// ************************************

#define RCAST reinterpret_cast
#define SCAST static_cast

void* __cdecl operator new(size_t size, POOL_TYPE pool, ULONG tag = 0) {
	PVOID newAddress;

	newAddress = ExAllocatePoolWithTag(pool, size, tag);
	//
	// Remove remenants from previous use.
	//
	if (newAddress)
	{
		memset(newAddress, 0, size);
	}
	return newAddress;
}
void __cdecl operator delete(void* p, unsigned __int64) {
	ExFreePool(p);
}