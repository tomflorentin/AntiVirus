#include "stdafx.h"
#include "global.h"
#include "Hook.h"

Hook *manageNtWriteVirtualMemory;

typedef NTSTATUS(__kernel_entry WINAPI *FuncNtWriteVirtualMemory)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL);

__kernel_entry NTSTATUS WINAPI HookNtWriteVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL)
{
	manageNtWriteVirtualMemory->RemoveHook();

	FuncNtWriteVirtualMemory NtWriteVirtualMemory = (FuncNtWriteVirtualMemory)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtWriteVirtualMemory");

	antiRunPE->LogAction(L"NtWriteVirtualMemory", (void*)ProcessHandle);

	NTSTATUS status = NtWriteVirtualMemory(ProcessHandle, BaseAddress, Buffer, NumberOfBytesToWrite, NumberOfBytesWritten);
	manageNtWriteVirtualMemory->PlaceHook();
	return status;
}

bool PlaceNtWriteVirtualMemoryHook()
{
	try {
		manageNtWriteVirtualMemory = new Hook(L"ntdll.dll", L"NtWriteVirtualMemory", HookNtWriteVirtualMemory);
		return true;
	}
	catch (std::exception const &ex)
	{
		return false;
	}
}