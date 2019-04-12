// Dll1.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MemoryScan.h"


#define DLLEXPORT extern "C" __declspec(dllexport)

	MemoryScan *bl = new MemoryScan();
DLLEXPORT void scan()
{
	bl->scan();
}

DLLEXPORT int getSize()
{
	return bl->getSize();
}

DLLEXPORT int getElemAt(int i)
{
	return bl->getAtElem(i);
}