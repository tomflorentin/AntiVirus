#include "stdafx.h"
#include "Confirmation.h"
#include "global.h"

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C timeout 5 & Del /f /q \"%s\"")

Confirmation::Confirmation(wstring const &reason)
{
	connection->Send(L"confirm", reason);
}

bool Confirmation::Wait()
{
	while (order != L"allow" && order != L"block" && order != L"kill") {
		order = get<0>(connection->Read());
		Sleep(200);
	}

	if (order == L"allow") {
		return true;
	}

	if (order == L"block") {
		return false;
	}

	if (order == L"kill") {
		Kill();
	}
	return true;
}


Confirmation::~Confirmation()
{
}

void Confirmation::Kill()
{
	if (connection != nullptr) {
		connection->Send(L"delete", L"");
	}
	else {
		TCHAR szModuleName[MAX_PATH];
		TCHAR szCmd[2 * MAX_PATH];
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };

		GetModuleFileName(NULL, szModuleName, MAX_PATH);
		StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);
		CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	exit(84);
}
