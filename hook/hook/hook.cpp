 // hook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Hook.h"
#include <iostream>


Hook::Hook(wstring const &dll, wstring const &function, void *hook, bool hookNow)
{
	this->funcPtr = findFunction(dll, function); // Get function to hook address
	this->hookFunc = hook;

	if (hookNow)
		PlaceHook();
}


Hook::~Hook()
{
}

void *Hook::findFunction(wstring const & dll, wstring const & function)
{
	HMODULE hDll;
	HANDLE hFunc;
	char funcNameTmp[255];

	sprintf_s(funcNameTmp, "%ls", function.c_str());
	hDll = GetModuleHandle(dll.c_str());
	if (hDll == INVALID_HANDLE_VALUE)
		throw (runtime_error("Unable to load DLL"));
	hFunc = GetProcAddress(hDll, funcNameTmp);
	if (hFunc == nullptr) {
		throw (runtime_error("Unable to locate function"));
	}
	return hFunc;
}

bool Hook::PlaceHook()
{
	DWORD protection;

	VirtualProtect(funcPtr, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &protection); // Unprotect zone
	memcpy(this->overridedBytes, funcPtr, 5); // Save bytes before overriding
	memset(funcPtr, 0x90, 5); // Prepare zone with NOP asm operation

	DWORD relativeAddr = (DWORD)hookFunc - (DWORD)funcPtr - 5; // Get relative address for jmp

	((BYTE*)funcPtr)[0] = 0xE9; // ASM JMP
	memcpy((((BYTE*)funcPtr) + 1), &relativeAddr, 4); // Jump address

	DWORD tmp; // useless vairable to avoid crash
	VirtualProtect(funcPtr, sizeof(DWORD), protection, &tmp); // Reprotect zone
	std::wcout << L"Function hooked !" << std::endl;
	return true;
}

bool Hook::RemoveHook()
{
	DWORD protection;

	VirtualProtect(funcPtr, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &protection); // Unprotect zone

	memcpy(funcPtr, this->overridedBytes, 5); // Restore original bytes on API
	DWORD tmp; // useless vairable to avoid crash
	VirtualProtect(funcPtr, sizeof(DWORD), protection, &tmp); // Reprotect zone
	std::wcout << L"Function UnHooked !" << std::endl;
	return true;
}

