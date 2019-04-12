#include "stdafx.h"
#include "MemoryScan.h"
#pragma comment(lib,"Shlwapi.lib")


int MemoryScan::scan()
{
	HMODULE ntdll = LoadLibrary(L"ntdll.dll");
	if (ntdll == NULL) {
		printf("Unable to Load Library");
		return 1;
	}
	if (ResNtFunc(ntdll)) {
		printf("Failed to resolve Functions");
		return 1;
	}
	if (GetDebugPrivs()) {
		printf("Unable to get Debug Privileges");
		return 1;
	}
	DWORD ProcId[2048], BytesFilled;
	int EnumStat = EnumProcesses(ProcId, sizeof(ProcId), &BytesFilled);
	if (EnumStat == 0) {
		printf("Failed to Enumerate Processes");
		return 1;
	}
	int TotalProcess = BytesFilled / sizeof(DWORD);
	int Status = CheckAllProcesses(ProcId, TotalProcess);
	if (Status) {
		printf("Failed! Unable to Check all the running processes");
		return 1;
	}
	FreeLibrary(ntdll);
	return 0;
}

int MemoryScan::getAtElem(int i)
{
	if (i > this->vec.size())
		return -1;
	return this->vec.at(i);
}

int MemoryScan::getSize()
{
	return this->vec.size();
}

#pragma region Private Method


int  MemoryScan::ResNtFunc(HMODULE ntdll)
{
	NtQueryInfoProcess = (NTQUERYINFOPROC)GetProcAddress(ntdll, "NtQueryInformationProcess");
	if (NtQueryInfoProcess == NULL)
		return 1;
	return 0;
}

int  MemoryScan::GetDebugPrivs()
{
	HANDLE token;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
		return 1;
	TOKEN_PRIVILEGES TokenPrivs;
	if (!LookupPrivilegeValue(NULL, L"SeDebugPrivilege", &(TokenPrivs.Privileges[0].Luid))) {
		CloseHandle(token);
		return 1;
	}
	TokenPrivs.PrivilegeCount = 1;
	TokenPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(token, FALSE, &TokenPrivs, NULL, NULL, NULL)) {
		CloseHandle(token);
		return 1;
	}
	CloseHandle(token);
	return 0;
}

DWORD  MemoryScan::CmpNtFileHeaders(IMAGE_NT_HEADERS ProcHeader, IMAGE_NT_HEADERS ImageHeader)
{
	if (ProcHeader.FileHeader.TimeDateStamp == ImageHeader.FileHeader.TimeDateStamp) {
		if (ProcHeader.FileHeader.SizeOfOptionalHeader == ImageHeader.FileHeader.SizeOfOptionalHeader) {
			if (ProcHeader.FileHeader.Characteristics == ImageHeader.FileHeader.Characteristics) {
				return 0;
			}
		}
	}
	return 1;
}

DWORD  MemoryScan::CmpNtOptHeaders(IMAGE_NT_HEADERS ProcHeader, IMAGE_NT_HEADERS ImageHeader)
{
	if (ProcHeader.OptionalHeader.CheckSum == ImageHeader.OptionalHeader.CheckSum) {
		if (ProcHeader.OptionalHeader.AddressOfEntryPoint == ImageHeader.OptionalHeader.AddressOfEntryPoint) {
			if (ProcHeader.OptionalHeader.BaseOfCode == ImageHeader.OptionalHeader.BaseOfCode) {
				if (ProcHeader.OptionalHeader.SizeOfInitializedData == ImageHeader.OptionalHeader.SizeOfInitializedData) {
					if (ProcHeader.OptionalHeader.SizeOfImage == ImageHeader.OptionalHeader.SizeOfImage) {
						return 0;
					}
				}
			}
		}
	}
	return 1;
}

DWORD  MemoryScan::CmpSecHeaders(PIMAGE_SECTION_HEADER ProcessHeaders, PIMAGE_SECTION_HEADER ImageHeaders, int Sections)
{
	int res;
	for (int num = 0; num < Sections; num++)
	{
		res = 1;
		if (memcmp(ProcessHeaders[num].Name, ImageHeaders[num].Name, 8) == 0) {
			if (ProcessHeaders[num].VirtualAddress == ImageHeaders[num].VirtualAddress) {
				if (ProcessHeaders[num].SizeOfRawData == ImageHeaders[num].SizeOfRawData) {
					if (ProcessHeaders[num].Characteristics == ImageHeaders[num].Characteristics) {
						res = 0;
					}
				}
			}
		}
		if (res)
			return 1;
	}
	return 0;
}

int  MemoryScan::CheckAllProcesses(DWORD *ProcId, DWORD TotalProcess)
{
	BOOL is32 = false;
	PEB RemotePEB;
	IMAGE_DOS_HEADER ProcessDosHeader;
	IMAGE_NT_HEADERS ProcessNtHeader;
	IMAGE_DOS_HEADER DiskImageDosHeader;
	IMAGE_NT_HEADERS DiskImageNtHeader;
	DWORD DosStatus;
	DWORD NtFileStatus;
	DWORD NtOptStatus;
	DWORD SecStatus;
	PIMAGE_SECTION_HEADER ProcessSecHeaders;
	int NumOfSecProcess;
	HANDLE Process = nullptr;
	PROCESS_BASIC_INFORMATION ProcInfo;
	NTSTATUS NtStat;
	LPVOID ProcessImageBase;
	LPWSTR filePath;
	HANDLE file;
	int NumOfSecImage;

	for (DWORD Num = 0; Num < TotalProcess; Num++)
	{
		if (ProcId[Num] == 0)
			continue;
		Process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, ProcId[Num]);
		if (Process == NULL)
			continue;
		IsWow64Process(Process, &is32);
		if (is32 == FALSE) {
			CloseHandle(Process);
			continue;
		}
		NtStat = NtQueryInfoProcess(Process, ProcessBasicInformation, &ProcInfo, sizeof(PROCESS_BASIC_INFORMATION), NULL);
		if (NtStat) {
			CloseHandle(Process);
			continue;
		}
		if (!ReadProcessMemory(Process, ProcInfo.PebBaseAddress, &RemotePEB, sizeof(PEB), 0)) {
			CloseHandle(Process);
			return 1;
		}
		ProcessImageBase = RemotePEB.Reserved3[1];
		if (!ReadProcessMemory(Process, ProcessImageBase, &ProcessDosHeader, sizeof(IMAGE_DOS_HEADER), 0)) {
			CloseHandle(Process);
			return 1;
		}
		if (!ReadProcessMemory(Process, LPVOID(ProcessDosHeader.e_lfanew + (DWORD)ProcessImageBase), &ProcessNtHeader, sizeof(IMAGE_NT_HEADERS), 0)) {
			CloseHandle(Process);
			return 1;
		}
		NumOfSecProcess = ProcessNtHeader.FileHeader.NumberOfSections;
		ProcessSecHeaders = (PIMAGE_SECTION_HEADER)malloc(NumOfSecProcess * sizeof(IMAGE_SECTION_HEADER));
		for (int num = 0; num < NumOfSecProcess; num++)
			ReadProcessMemory(Process, LPVOID((DWORD)ProcessImageBase + ProcessDosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS) + num * sizeof(IMAGE_SECTION_HEADER)), ProcessSecHeaders + num, sizeof(IMAGE_SECTION_HEADER), 0);
		filePath = (LPWSTR)malloc(1024 * sizeof(char));
		if (!GetModuleFileNameEx(Process, NULL, filePath, 1024)) {
			CloseHandle(Process);
			return 1;
		}
		file = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == NULL) {
			CloseHandle(Process);
			return 1;
		}
		if (!ReadFile(file, &DiskImageDosHeader, sizeof(IMAGE_DOS_HEADER), NULL, NULL)) {
			CloseHandle(Process);
			CloseHandle(file);
			return 1;
		}
		SetFilePointer(file, DiskImageDosHeader.e_lfanew, NULL, FILE_BEGIN);
		if (!ReadFile(file, &DiskImageNtHeader, sizeof(IMAGE_NT_HEADERS), NULL, NULL)) {
			CloseHandle(Process);
			CloseHandle(file);
			return 1;
		}
		SetFilePointer(file, DiskImageDosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS), NULL, FILE_BEGIN);
		NumOfSecImage = DiskImageNtHeader.FileHeader.NumberOfSections;
		PIMAGE_SECTION_HEADER DiskImageSecHeaders = (PIMAGE_SECTION_HEADER)malloc(
			NumOfSecImage * sizeof(IMAGE_SECTION_HEADER));
		for (int num = 0; num < NumOfSecImage; num++)
			ReadFile(file, DiskImageSecHeaders + num, sizeof(IMAGE_SECTION_HEADER), NULL, NULL);
		if (NumOfSecProcess != NumOfSecImage) {
			this->vec.push_back(ProcId[Num]);
			continue;
		}
		DosStatus = ProcessDosHeader.e_lfanew != DiskImageDosHeader.e_lfanew ? 1 : 0;
		NtFileStatus = CmpNtFileHeaders(ProcessNtHeader, DiskImageNtHeader);
		NtOptStatus = CmpNtOptHeaders(ProcessNtHeader, DiskImageNtHeader);
		SecStatus = CmpSecHeaders(ProcessSecHeaders, DiskImageSecHeaders, NumOfSecImage);
		if (DosStatus | NtFileStatus | NtOptStatus | SecStatus)
			this->vec.push_back(ProcId[Num]);
		CloseHandle(file);
		CloseHandle(Process);
	}
	return 0;
}

#pragma endregion