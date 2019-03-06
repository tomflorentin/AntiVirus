// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "Hook.h"
#include "HookOpen.h"

Hook *msgHook;

int _stdcall HookMessageBoxW(
	HWND    hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT    uType
)
{
	std::wcout << "You sent a message box saying : " << lpText << std::endl;
	msgHook->RemoveHook();
	MessageBoxW(hWnd, lpText, lpCaption, uType);
	msgHook->PlaceHook();
	return 0;
}


// APP_INIT path (to load DLL to each new process) :
// Computer\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Windows

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//msgHook = new Hook(L"User32.dll", L"MessageBoxW", HookMessageBoxW);
		HookOpen();
		WCHAR buff[1024];
		GetModuleFileName(NULL, buff, 1024);
		MessageBox(NULL, buff, buff, NULL);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

