// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <algorithm>
#include <vector>

#define DLLEXPORT extern "C" __declspec(dllexport)

using namespace std;



// reference additional headers your program requires here
