#pragma once

#include "stdafx.h"
#include "Client.h"

class Server
{
public:
	Server();
	~Server();
	void Listen();
	void Refresh();
	int countConnections();
	int getPid(int index) const;
	wstring const &getPath(int index) const;
	wstring getOrder(int index);
	wstring getArgs(int index);
	char getRole(int index) const;
	void sendOrder(int index, wstring const &cmd, wstring const &arg);

private:
	void Accept();
	int port;
	SOCKET sock;
	vector<unique_ptr<Client>> clients;
};

