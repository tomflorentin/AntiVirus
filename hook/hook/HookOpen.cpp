#include "stdafx.h"
#include "HookOpen.h"
#include "Hook.h"

Hook *hook;

__kernel_entry NTSTATUS __stdcall HookNtCreateFile(
	OUT PHANDLE           FileHandle,
	IN ACCESS_MASK        DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN PLARGE_INTEGER     AllocationSize,
	IN ULONG              FileAttributes,
	IN ULONG              ShareAccess,
	IN ULONG              CreateDisposition,
	IN ULONG              CreateOptions,
	IN PVOID              EaBuffer,
	IN ULONG              EaLength
)
{
	ntCreate NtCreateFile = (ntCreate)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateFile");
	std::wcout << L"You are trying to open " << ObjectAttributes->ObjectName->Buffer << std::endl;
	if (wcsstr(ObjectAttributes->ObjectName->Buffer, L"blbl")) {
		MessageBoxA(NULL, "Unable to open this file", "", 0);
		hook->PlaceHook();
		return false;
	}
	hook->RemoveHook();
	NTSTATUS status = NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
	hook->PlaceHook();
	return status;
}

void HookOpen()
{
	try {
		hook = new Hook(L"ntdll.dll", L"NtCreateFile", HookNtCreateFile);
	}
	catch (std::exception const &ex)
	{
		MessageBox(NULL, L"Error", L"Error", 0);
	}
}