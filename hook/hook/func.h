#pragma once

#include "stdafx.h"

bool PlaceNtCreateFileHook();
bool PlaceNtOpenProcessHook();
bool PlaceNtCreateKeyHook();
bool PlaceNtWriteVirtualMemoryHook();
bool PlaceNtUnmapViewOfSectionHook();
bool PlaceGetProcAddressHook();

int getPid(std::wstring const &targetProcessName);