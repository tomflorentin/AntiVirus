#include "stdafx.h"
#include "HookOpen.h"
#include "Hook.h"
#include "global.h"

Hook *hook;

__kernel_entry NTSTATUS __cdecl HookNtCreateFile(
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
	hook->RemoveHook();
	ntCreate NtCreateFile = (ntCreate)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateFile");
	connection->Send(L"msgbox", wstring(L"J'essaye d'ouvrir ") + ObjectAttributes->ObjectName->Buffer);

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