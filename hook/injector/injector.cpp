// injector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>

int main(int ac, char**av, char**env)
{
	// Starting child process

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(L"../Debug/target.exe",   // the path
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles. 

	LPCSTR DllPath = "../Debug/hook.dll";

	std::cout << pi.dwProcessId << std::endl;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);

	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath,
		strlen(DllPath) + 1, 0);

	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);


	WaitForSingleObject(hLoadThread, INFINITE);



	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);

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
