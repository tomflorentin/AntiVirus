#include "stdafx.h"

int getPid(std::wstring const &targetProcessName)
{
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W entry;
	int pid = 0;

	entry.dwSize = sizeof entry;
	if (!Process32FirstW(snap, &entry))
		return 0;
	do {
		if (std::wstring(entry.szExeFile) == targetProcessName) {
			pid = entry.th32ProcessID;
		}
	} while (Process32NextW(snap, &entry));
	return pid;
}