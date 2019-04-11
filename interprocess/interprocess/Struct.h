#pragma once

#include "stdafx.h"

#define SERVICE 1
#define APP 2

struct packet
{
	char role;
	int pid;
	WCHAR filepath[1024];
	WCHAR cmd[255];
	WCHAR argument[1024];
};

struct id
{
	char role;
	unsigned pid;
	wstring filepath;
};