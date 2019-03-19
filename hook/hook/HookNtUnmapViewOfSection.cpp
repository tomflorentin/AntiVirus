#include "stdafx.h"
#include "global.h"
#include "Hook.h"

Hook *manageNtUnmapViewOfSection;

typedef NTSTATUS (__kernel_entry WINAPI *FuncNtUnmapViewOfSection)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress);

__kernel_entry NTSTATUS WINAPI HookNtUnmapViewOfSection(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress)
{
	manageNtUnmapViewOfSection->RemoveHook();

	FuncNtUnmapViewOfSection NtUnmapViewOfSection = (FuncNtUnmapViewOfSection) GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtUnmapViewOfSection");

	antiRunPE->LogAction(L"NtUnmapViewOfSection", (void*)ProcessHandle);

	NTSTATUS status = NtUnmapViewOfSection(ProcessHandle, BaseAddress);
	manageNtUnmapViewOfSection->PlaceHook();
	return status;
}

bool PlaceNtUnmapViewOfSectionHook()
{
	try {
		manageNtUnmapViewOfSection = new Hook(L"ntdll.dll", L"NtUnmapViewOfSection", HookNtUnmapViewOfSection);
		return true;
	}
	catch (std::exception const &ex)
	{
		return false;
	}
}