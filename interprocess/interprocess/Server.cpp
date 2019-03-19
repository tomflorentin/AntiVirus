#include "stdafx.h"
#include "Server.h"
#include "Client.h"

Server::Server()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
}

void Server::Listen()
{
	sockaddr_in server = { 0 };

	if (sock < 0)
		throw exception("Unable to create server socket");;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons(5900);
	InetPton(AF_INET, L"127.0.0.1", &server.sin_addr.s_addr);

	if (bind(sock, (struct sockaddr *) &server, sizeof server)) {
		cerr << GetLastError() << endl;
		closesocket(sock);
		throw exception("Unable to bind");
	}
	if (listen(sock, 500)) {
		closesocket(sock);
		throw exception("Unable to listen");
	}
	unsigned long block = 1;
	ioctlsocket(sock, FIONBIO, &block);
}

void Server::Refresh()
{
	Accept();

	for (int i = 0; i < clients.size(); i++) {
		try {
			clients[i]->Refresh();
		}
		catch (exception const &ex)
		{
			cerr << ex.what() << endl;
			clients.erase(clients.begin() + i);
		}
	}
}

int Server::countConnections()
{
	return (int)this->clients.size();
}

int Server::getPid(int index) const
{
	try {
		auto &cl = clients.at(index);
		return cl->getPid();
	}
	catch (...)
	{
		return -1;
	}
}

wstring const &Server::getPath(int index) const
{
	try {
		auto &cl = clients.at(index);
		return cl->getPath();
	}
	catch (...)
	{
		return wstring(L"");
	}
}

wstring Server::getOrder(int index)
{
	try {
		auto &cl = clients.at(index);
		return cl->getOrder();
	}
	catch (...)
	{
		return wstring(L"");
	}
}

char Server::getRole(int index) const
{
	try {
		auto &cl = clients.at(index);
		return cl->getRole();
	}
	catch (...)
	{
		return 0;
	}
}

wstring Server::getArgs(int index)
{
	try {
		auto &cl = clients.at(index);
		return cl->getArgs();
	}
	catch (...)
	{
		return wstring(L"");
	}
}

void Server::sendOrder(int index, wstring const & cmd, wstring const & arg)
{
	try {
		auto &cl = clients.at(index);
		cl->Refresh(cmd, arg);
	}
	catch (...)
	{
		return;
	}	
}

void Server::Accept()
{
	struct sockaddr_in server = { 0 };
	int size = sizeof server;

	SOCKET clientSock = accept(sock, (sockaddr *)&server, &size);

	if (clientSock == INVALID_SOCKET)
		return;
	try
	{
		this->clients.push_back(std::unique_ptr<Client>(new Client(clientSock)));
		wcout << L"New client ! on sock " << clientSock << endl;
	}
	catch (const exception& ex)
	{
		wcerr << L"Error accepting connection : " << ex.what() << endl;
	}
}

Server::~Server()
{
	closesocket(this->sock);
}