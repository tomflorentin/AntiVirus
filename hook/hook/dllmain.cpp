// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "Hook.h"
#include "Connection.h"
#include "AntiRunPE.h"

Connection *connection = nullptr;
AntiRunPE *antiRunPE = nullptr;

// APP_INIT path (to load DLL to each new process) :
// Computer\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Windows

#include "func.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		try {
			connection = new Connection();
		}
		catch (...) { std::cerr << "Error connecting server" << std::endl; };
		antiRunPE = new AntiRunPE();
		PlaceNtCreateKeyHook();
		PlaceNtUnmapViewOfSectionHook();
		PlaceNtWriteVirtualMemoryHook();
		PlaceGetProcAddressHook();
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		delete connection;
        break;
    }
    return TRUE;
}
