#pragma once
#include "stdafx.h"

typedef NTSTATUS(WINAPI *NTQUERYINFOPROC)(HANDLE ProcessHandle, PROCESSINFOCLASS  ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
using namespace std;

class MemoryScan
{
public:
	int scan();
	MemoryScan() = default;
	~MemoryScan() = default;
	int getAtElem(int i);
	int getSize();

private:
	NTQUERYINFOPROC NtQueryInfoProcess;
	DWORD NumOfThreats = 0;
	int ResNtFunc(HMODULE ntdll);
	int GetDebugPrivs();
	DWORD CmpNtFileHeaders(IMAGE_NT_HEADERS ProcHeader, IMAGE_NT_HEADERS ImageHeader);
	DWORD CmpNtOptHeaders(IMAGE_NT_HEADERS ProcHeader, IMAGE_NT_HEADERS ImageHeader);
	DWORD CmpSecHeaders(PIMAGE_SECTION_HEADER ProcessHeaders, PIMAGE_SECTION_HEADER ImageHeaders, int Sections);
	int CheckAllProcesses(DWORD *ProcId, DWORD TotalProcess);
	vector<int> vec;

};

