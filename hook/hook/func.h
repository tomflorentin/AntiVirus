#pragma once

#include "stdafx.h"

bool PlaceNtCreateFileHook();
bool PlaceNtOpenProcessHook();
bool PlaceNtCreateKeyHook();
bool PlaceNtWriteVirtualMemoryHook();
bool PlaceNtUnmapViewOfSectionHook();

int getPid(std::wstring const &targetProcessName);