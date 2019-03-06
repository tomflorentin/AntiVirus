// interprocess.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Server.h"
#include "Client.h"

Server *srv = nullptr;
Client *cli = nullptr;

DLLEXPORT int StartServer()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	try {
		srv = new Server();
		srv->Listen();
		srv->Refresh();
	}
	catch (exception const &ex) {
		cerr << ex.what() << endl;
		return 1;
	}
	return 0;
}

DLLEXPORT void StopServer()
{
	delete srv;
	srv = 0;
	WSACleanup();
}

DLLEXPORT int ConnectClient(bool isGui)
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	char role = isGui ? GUI : APP;
	try {
		cli = new Client(role);
	}
	catch (...) {
		return 1;
	}
	return 0;
}

DLLEXPORT void DisconnectClient()
{
	delete cli;
	cli = nullptr;
	WSACleanup();
}

DLLEXPORT void Refresh()
{
	try
	{
		if (srv)
			srv->Refresh();
	}
	catch (exception const &ex)
	{
		cerr << ex.what() << endl;
		delete srv;
		srv = nullptr;
	}
	try
	{
		if (cli)
			cli->Refresh();
		delete cli;
		cli = nullptr;
	}
	catch (exception const &ex)
	{
		cerr << ex.what() << endl;
	}
}

#pragma region ServerGetters

DLLEXPORT int GetClientPid(int index)
{
	if (srv)
		return srv->getPid(index);
	return 0;
}

DLLEXPORT int GetClientRole(int index)
{
	if (srv)
		return srv->getRole(index);
	return 0;
}

DLLEXPORT int CountClients()
{
	if (!srv)
		return -1;
	return srv->countConnections();
}

DLLEXPORT WCHAR const *GetClientPath(int index)
{
	if (!srv)
		return nullptr;
	return srv->getPath(index).c_str();
}

DLLEXPORT WCHAR const *GetClientOrder(int index)
{
	if (!srv)
		return nullptr;
	return srv->getOrder(index).c_str();
}

DLLEXPORT WCHAR const *GetClientArg(int index)
{
	if (!srv)
		return nullptr;
	return srv->getArgs(index).c_str();
}

#pragma endregion ClientGetters

DLLEXPORT WCHAR const *GetServiceOrder()
{
	if (!cli)
		return nullptr;
	return cli->getOrder().c_str();
}

DLLEXPORT WCHAR const *GetServiceArg()
{
	if (!cli)
		return nullptr;
	return cli->getArgs().c_str();
}

DLLEXPORT void SendServiceOrder(WCHAR *order, WCHAR *args)
{
	if (cli)
		cli->Refresh(order, args);
}

DLLEXPORT void SendOrder(int index, WCHAR *order, WCHAR *args)
{
	if (cli)
		cli->Refresh(order, args);
}

#pragma region 