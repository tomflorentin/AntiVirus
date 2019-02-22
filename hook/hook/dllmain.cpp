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
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

