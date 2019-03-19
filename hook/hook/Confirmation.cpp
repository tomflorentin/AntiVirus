#include "stdafx.h"
#include "Confirmation.h"
#include "global.h"

Confirmation::Confirmation(wstring const &reason)
{
	connection->Send(L"confirm", reason);
}

bool Confirmation::Wait()
{
	while (order != L"allow" && order != L"block" && order != L"kill") {
		order = get<0>(connection->Read());
		Sleep(200);
	}

	if (order == L"allow") {
		return true;
	}

	if (order == L"block") {
		return false;
	}

	if (order == L"kill") {
		this->Kill();
	}
}


Confirmation::~Confirmation()
{
}

void Confirmation::Kill()
{
	exit(84); // Insert here killing method
}
