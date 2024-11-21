#pragma once
#include "common.h"

#define DEFINE_TAG(tag) tag
#define SYSTEM_MODULE_INFO_TAG DEFINE_TAG('mSpS')

EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;
NTSTATUS
DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
);

DRIVER_UNLOAD DriverUnload;
VOID
DriverUnload(
	_In_ PDRIVER_OBJECT DriverObject
);
EXTERN_C_END