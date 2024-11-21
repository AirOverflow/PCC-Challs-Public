#pragma once
#include "base.h"

// All your utility functions go here.

#define IOCTL_PCC CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2049, METHOD_BUFFERED, FILE_ANY_ACCESS)

UNICODE_STRING DEVICE_NAME = RTL_CONSTANT_STRING(L"\\Device\\AshfaqDriver");
UNICODE_STRING DEVICE_SYMBOLIC_LINK = RTL_CONSTANT_STRING(L"\\??\\AshfaqDriverLink");


NTSTATUS Handler(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	PIO_STACK_LOCATION IoStackLocation = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS Status = STATUS_SUCCESS;
	switch (IoStackLocation->MajorFunction) {
	case IRP_MJ_CREATE:
		INFO("Opened handle to %wZ", DEVICE_SYMBOLIC_LINK);
		break;
	case IRP_MJ_CLOSE:
		INFO("Closed handle to %wZ", DEVICE_SYMBOLIC_LINK);
		IoDeleteDevice(DeviceObject);
		IoDeleteSymbolicLink(&DEVICE_SYMBOLIC_LINK);
		break;
	case IRP_MJ_DEVICE_CONTROL:
		DEBUG("IRP_MJ_DEVICE_CONTROL");
		break;
	default:
		DEBUG("IRP_MJ_XXX");
		break;
	}

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}