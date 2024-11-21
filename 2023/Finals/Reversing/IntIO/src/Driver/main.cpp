#include "utils.h"
#include "SBOX.h"

NTSTATUS ConnectionHandler(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	PIO_STACK_LOCATION stackLocation = NULL;
	stackLocation = IoGetCurrentIrpStackLocation(Irp);

	CHAR* msg = "Nah man, it's not that easy. ;)";
	int i = 0;
	if (stackLocation->Parameters.DeviceIoControl.IoControlCode == IOCTL_PCC) {
		INFO("[0x%x] Connection with Userland successful.", stackLocation->Parameters.DeviceIoControl.IoControlCode);
		CHAR* ulData = (char*)Irp->AssociatedIrp.SystemBuffer;
		INFO("Data from userland: %s", ulData);
		if (strlen(ulData) > 4) {
			DEBUG("Length Check Passed.");
			if (ulData[0] == 'F') {
				DEBUG("Check-1 - Passed");
				if (ulData[1] == 'l') {
					DEBUG("Check-2 - Passed");
					if (ulData[2] == '@') {
						DEBUG("Check-3 - Passed");
						if (ulData[3] == 'G') {
							DEBUG("Check-4 - Passed");
							DEBUG("Sending %s", decrypt(flag, 48));
							Irp->IoStatus.Information = 48;
							RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, decrypt(flag, 48), 48);
						}
					}
				}
			}
		}
		i++;
	}
	if(i == 0) {
		Irp->IoStatus.Information = strlen(msg);
		RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, msg, strlen(msg));
	}
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

VOID Unload(IN PDRIVER_OBJECT DriverObject) {
	LOG("Driver Unloaded.!");
	// Close IoHandles:
}

NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {


	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS Status;

	DriverObject->DriverUnload = Unload;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ConnectionHandler;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = Handler;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = Handler;

	Status = STATUS_SUCCESS;
	DEBUG("Initialized with status 0x%X.", Status);
	INFO("Trust me man, you're on the right path...");
	INFO("Some parts for you: PCC{");

	Status = IoCreateDevice(DriverObject, 0, &DEVICE_NAME, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DriverObject->DeviceObject);
	if (!NT_SUCCESS(Status)) {
		ERROR("Unable to create IoDevice!");
	}
	else {
		INFO("Device %wZ created successfully!", DEVICE_NAME);
	}
	Status = IoCreateSymbolicLink(&DEVICE_SYMBOLIC_LINK, &DEVICE_NAME);
	if (!NT_SUCCESS(Status)) {
		ERROR("Unable to create IoDeviceSymbolicLink!");
	}
	else {
		INFO("Symbolic Link %wZ created successfully!", DEVICE_NAME);
	}


	return Status;
}