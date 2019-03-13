#include "stdafx.h"
#include "HookOpen.h"
#include "Hook.h"
#include "global.h"

Hook *hook;

__kernel_entry NTSTATUS WINAPI HookNtCreateFile(
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

	if (!wcsstr(ObjectAttributes->ObjectName->Buffer, L"virus")) {
		ntCreate NtCreateFile = (ntCreate)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateFile");
		NTSTATUS status = NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
		hook->PlaceHook();
		return status;
	}



	ntCreate NtCreateFile = (ntCreate)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateFile");
	connection->Send(L"confirm", wstring(L"Oppening suspicious file (") + ObjectAttributes->ObjectName->Buffer + wstring(L")"));

	wstring order = L"";
	while (order != L"allow" && order != L"block" && order != L"kill") {
		order = get<0>(connection->Read());
		Sleep(500);
	}

	if (order == L"allow") {
		NTSTATUS status = NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
		hook->PlaceHook();
		return status;
	}

	if (order == L"block") {
		hook->PlaceHook();
		return 0xC000000F; // No suck file NT_STATUS
	}

	if (order == L"kill") {
		exit(84); // Insert here killing method
	}
	return 0;
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