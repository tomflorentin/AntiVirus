#pragma once

#include "Struct.h"

class Client
{
public:
	Client(char _role);
	Client(SOCKET sock);
	~Client();
	void Refresh(wstring const &cmd = L"", wstring const &args = L"");
	wstring const &getPath() const;
	wstring getOrder();
	wstring getArgs();
	int getPid() const;
	char getRole() const;

private:
	time_t lastPacket;
	bool Recv(packet &data);
	bool Send(packet const &data);
	wstring path = L"unknown";
	wstring order;
	wstring args;

	id self = { 0, 0, L"" };
	id remote = { 0, 0, L"" };
	SOCKET sock;
};

