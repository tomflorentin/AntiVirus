#include "stdafx.h"
#include "Hook.h"
#include "func.h"
#include "Confirmation.h"

Hook *manageNtCreateKey;

const std::vector<std::wstring> blacklist = { 
	L"Software\\Microsoft\\Windows\\CurrentVersion\\RunServices",
	L"Software\\Microsoft\\Windows\\CurrentVersion\\RunServicesOnce",
	L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
	L"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
};


bool iequals(const wstring& a, const wstring& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

typedef __kernel_entry NTSTATUS(WINAPI *FuncNtCreateKey)(
	PHANDLE            KeyHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG              TitleIndex,
	PUNICODE_STRING    Class,
	ULONG              CreateOptions,
	PULONG             Disposition);

__kernel_entry NTSTATUS WINAPI HookNtCreateKey(
	PHANDLE            KeyHandle,
	ACCESS_MASK        DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG              TitleIndex,
	PUNICODE_STRING    Class,
	ULONG              CreateOptions,
	PULONG             Disposition)
{
	manageNtCreateKey->RemoveHook();

	std::wcout << ObjectAttributes->ObjectName->Buffer << std::endl;
	for (auto const &k : blacklist)
		if (iequals(k, ObjectAttributes->ObjectName->Buffer)) {
			FuncNtCreateKey NtCreateKey = (FuncNtCreateKey)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateKey");

			auto confirmation = Confirmation(wstring(L"Adding program to automatic startup"));
			wstring order = L"";
			
			if (confirmation.Wait()) {
				NTSTATUS status = NtCreateKey(KeyHandle, DesiredAccess, ObjectAttributes, TitleIndex, Class, CreateOptions, Disposition);
				manageNtCreateKey->PlaceHook();
				return status;
			} else {
				manageNtCreateKey->PlaceHook();
				return 0xC000000F; // No suck file NT_STATUS
			}
		}


	FuncNtCreateKey NtCreateKey = (FuncNtCreateKey)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateKey");
	NTSTATUS status = NtCreateKey(KeyHandle, DesiredAccess, ObjectAttributes, TitleIndex, Class, CreateOptions, Disposition);

	manageNtCreateKey->PlaceHook();
	return status;
}

bool PlaceNtCreateKeyHook()
{
	try {
		manageNtCreateKey = new Hook(L"ntdll.dll", L"NtCreateKey", HookNtCreateKey);
		return true;
	}
	catch (...) {
		return false;
	}
}
