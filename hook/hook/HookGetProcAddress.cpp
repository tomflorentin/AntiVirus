#include "stdafx.h"
#include "global.h"
#include "Hook.h"

std::vector<std::tuple<std::string, std::string>> blacklist =
{
	{"kernel32.dll", "GetProcAddress"},
	{"ntdll.dll", "NtCreateFile"},
	{"ntdll.dll", "NtOpenKey"},
	{"ntdll.dll", "NtOpenProcess"},
	{"ntdll.dll", "NtUnmapViewOfSection"},
	{"ntdll.dll", "NtWriteVirtualMemory"}
};

Hook *manageGetProcAddress = nullptr;

typedef void* (WINAPI *FuncGetProcAddress)(
	HMODULE hModule,
	LPCSTR  lpProcName
);

void *WINAPI HookGetProcAddress(
	HMODULE hModule,
	LPCSTR  lpProcName
)
{
	manageGetProcAddress->RemoveHook();

	void *addr = nullptr;

	for (auto const &b : blacklist) {
		if (std::get<1>(b) == lpProcName)
			addr = GetProcAddress(GetModuleHandleA(std::get<0>(b).c_str()), std::get<1>(b).c_str());
	}
	if (addr == nullptr)
		addr = GetProcAddress(hModule, lpProcName);

	manageGetProcAddress->RemoveHook();
	return addr;
}


bool PlaceGetProcAddressHook()
{
	try {
		manageGetProcAddress = new Hook(L"kernel32.dll", L"GetProcAddress", HookGetProcAddress);
		return true;
	}
	catch (std::exception const &ex)
	{
		return false;
	}
}