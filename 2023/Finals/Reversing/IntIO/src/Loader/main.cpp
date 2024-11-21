#include <iostream>
#include <Windows.h>

#define IOCTL_PCC CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2049, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main(char argc, char** argv)
{
    HANDLE device = INVALID_HANDLE_VALUE;
    BOOL status = FALSE;
    DWORD bytesReturned = 0;
    CHAR inBuffer[128] = { 0 };
    CHAR outBuffer[128] = { 0 };

    RtlCopyMemory(inBuffer, argv[1], strlen(argv[1]));

    printf("Ashfaq Nadeem Special Loader\n");
    device = CreateFileW(L"\\\\.\\AshfaqDriverLink", GENERIC_WRITE | GENERIC_READ | GENERIC_EXECUTE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);

    if (device == INVALID_HANDLE_VALUE)
    {
        printf_s("> Could not open device: 0x%x\n", GetLastError());
        return FALSE;
    }

    printf_s("> Issuing Driver: 0x%x\n", IOCTL_PCC);
    status = DeviceIoControl(device, IOCTL_PCC, inBuffer, sizeof(inBuffer), outBuffer, sizeof(outBuffer), &bytesReturned, (LPOVERLAPPED)NULL);
    printf_s("> 0x%x issued\n", IOCTL_PCC);
    printf_s("> Received from the kernel land: %s. Received buffer size: %d\n", outBuffer, bytesReturned);
    CloseHandle(device);
}