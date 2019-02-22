#pragma once

#include <string>
#include <Windows.h>

using namespace std;

class Hook
{
public:
	Hook(wstring const &dll, wstring const &function, void *hookFunc, bool hookNow = true);
	bool PlaceHook();
	bool RemoveHook();
	~Hook();
private:
	void *funcPtr;
	void *hookFunc;
	void *findFunction(wstring const &dll, wstring const &function);
	char overridedBytes[5];
};

