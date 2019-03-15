#pragma once

#include "stdafx.h"

bool PlaceNtCreateFileHook();
bool PlaceNtOpenProcessHook();
bool PlaceNtCreateKeyHook();

int getPid(std::wstring const &targetProcessName);