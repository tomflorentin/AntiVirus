// target.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>

typedef NTSTATUS(__kernel_entry WINAPI *FuncNtWriteVirtualMemory)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL);

typedef NTSTATUS(__kernel_entry WINAPI *FuncNtUnmapViewOfSection)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress);

int main()
{
	LoadLibrary(L"../Debug/hook.dll");
	Sleep(5000);

	//HKEY hkey;

	//LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey); //Creates a key       
	//LONG status = RegSetValueEx(hkey, L"MyApp", 0, REG_SZ, (BYTE*)L"blbl", (4 + 1) * sizeof(wchar_t));



	FuncNtUnmapViewOfSection NtUnmapViewOfSection = (FuncNtUnmapViewOfSection)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtUnmapViewOfSection");
	FuncNtWriteVirtualMemory NtWriteVirtualMemory = (FuncNtWriteVirtualMemory)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtWriteVirtualMemory");

	NtUnmapViewOfSection(NULL, NULL);
	NtWriteVirtualMemory((void*)0x01, NULL, NULL, 0, NULL);

	getchar();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
