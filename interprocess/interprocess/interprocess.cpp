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

DLLEXPORT WCHAR *GetClientPath(int index)
{
	WCHAR *ptr;

	try {
		if (!srv)
			throw exception("no server");
		auto const &tmp = srv->getPath(index);
		ptr = (WCHAR *)CoTaskMemAlloc(sizeof(WCHAR) * (1 + tmp.size()));
		wcscpy_s(ptr, tmp.size(), tmp.c_str());
		return ptr;
	}
	catch (...)
	{
		ptr = (WCHAR *)CoTaskMemAlloc(sizeof(WCHAR) * 2);
		wcscpy_s(ptr, 1, L"");
		return ptr;
	}
}

DLLEXPORT WCHAR *GetClientOrder(int index)
{
	WCHAR *ptr;

	try {
		if (!srv)
			throw exception("no server");
		auto const tmp = srv->getOrder(index);
		ptr = (WCHAR *)malloc(sizeof(WCHAR) * (1 + tmp.size()));
		wcscpy(ptr, tmp.c_str());
		return ptr;
	}
	catch (...)
	{
		ptr = (WCHAR *)malloc(sizeof(WCHAR) * 2);
		wcscpy(ptr, L"");
		return ptr;
	}
}

DLLEXPORT WCHAR *GetClientArg(int index)
{
	WCHAR *ptr;

	try {
		if (!srv)
			throw exception("no server");
		auto const &tmp = srv->getArgs(index);
		ptr = (WCHAR *)calloc(sizeof(WCHAR), (1 + tmp.size()));
		wcscpy(ptr, tmp.c_str());
		return ptr;
	}
	catch (...)
	{
		ptr = (WCHAR *)malloc(sizeof(WCHAR) * 2);
		wcscpy(ptr, L"");
		return ptr;
	}
}

#pragma endregion ClientGetters

DLLEXPORT WCHAR *GetServiceOrder()
{
	WCHAR *ptr;

	try {
		if (!cli)
			throw exception("no client");
		auto const &tmp = cli->getOrder();
		ptr = (WCHAR *)CoTaskMemAlloc(sizeof(WCHAR) * (1 + tmp.size()));
		wcscpy(ptr, tmp.c_str());
		return ptr;
	}
	catch (...)
	{
		ptr = (WCHAR *)CoTaskMemAlloc(sizeof(WCHAR) * 2);
		wcscpy(ptr, L"");
		return ptr;
	}
}

DLLEXPORT WCHAR *GetServiceArg()
{
	WCHAR *ptr;

	try {
		if (!cli)
			throw exception("no client");
		auto const &tmp = cli->getArgs();
		ptr = (WCHAR *)CoTaskMemAlloc(sizeof(WCHAR) * (1 + tmp.size()));
		wcscpy(ptr, tmp.c_str());
		return ptr;
	}
	catch (...)
	{
		ptr = (WCHAR *)CoTaskMemAlloc(sizeof(WCHAR) * 2);
		wcscpy(ptr, L"");
		return ptr;
	}
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

DLLEXPORT void FreePtr(void *ptr)
{
	if (ptr)
		free(ptr);
}