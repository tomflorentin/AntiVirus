#include "stdafx.h"
#include "Hook.h"
#include "func.h"

Hook *manageNtOpenProcess;

const std::vector<std::wstring> blacklist = { L"service.exe" };

typedef __kernel_entry NTSTATUS (WINAPI *FuncNtOpenProcess)(
	OUT PHANDLE             ProcessHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN CLIENT_ID           *ClientId);


__kernel_entry NTSTATUS WINAPI HookNtOpenProcess(
	OUT PHANDLE             ProcessHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN CLIENT_ID           *ClientId)
{
	manageNtOpenProcess->RemoveHook();
	
	for (auto const &process : blacklist) {
		int pid = getPid(process);

		if (pid == (int)ClientId->UniqueProcess || pid == (int)ClientId->UniqueThread) // DENY ACTION
			exit(84);
	}

	FuncNtOpenProcess NtOpenProcess = (FuncNtOpenProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtOpenProcess");
	NTSTATUS status = NtOpenProcess(ProcessHandle, AccessMask, ObjectAttributes, ClientId);
	manageNtOpenProcess->PlaceHook();
	return 0;
}

bool PlaceNtOpenProcessHook()
{
	try {
		manageNtOpenProcess = new Hook(L"ntdll.dll", L"NtOpenProcess", HookNtOpenProcess);
		return true;
	}
	catch (...) {
		return false;
	}
}