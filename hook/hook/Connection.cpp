#include "stdafx.h"
#include "Connection.h"

Connection::Connection()
{
	if (!importDll())
		throw exception("Unable to load DLL functions");

	if (ConnectClient())
		throw exception("Unable to connect to service");
}

Connection::~Connection()
{
	DisconnectClient();
	FreeLibrary(libHandle);
}

tuple<wstring, wstring> Connection::Read()
{
	WCHAR const *orderPtr = GetServiceOrder();
	WCHAR const *argPtr = GetServiceArg();
	auto tup = tuple<wstring, wstring>(wstring(orderPtr), wstring(argPtr));

	free((void*)orderPtr);
	free((void*)argPtr);
	return tup;
}

void Connection::Send(wstring const &cmd, wstring const &arg)
{
	SendServiceOrder((WCHAR *)cmd.c_str(), (WCHAR *)arg.c_str());
}

bool Connection::importDll()
{
	libHandle = LoadLibrary(L"C:\\Users\\tomfl\\Desktop\\AntiVirus\\interprocess\\interprocess\\Debug\\interprocess.dll");

	if (libHandle == INVALID_HANDLE_VALUE)
		return false;

	if ((StartServer = (_StartServer)GetProcAddress(libHandle, "StartServer")) == nullptr) return false;
	if ((StopServer = (_StopServer)GetProcAddress(libHandle, "StopServer")) == nullptr) return false;
	if ((ConnectClient = (_ConnectClient)GetProcAddress(libHandle, "ConnectClient")) == nullptr) return false;
	if ((DisconnectClient = (_DisconnectClient)GetProcAddress(libHandle, "DisconnectClient")) == nullptr) return false;
	if ((Refresh = (_Refresh)GetProcAddress(libHandle, "Refresh")) == nullptr) return false;
	if ((CountClients = (_CountClients)GetProcAddress(libHandle, "CountClients")) == nullptr) return false;
	if ((SendOrder = (_SendOrder)GetProcAddress(libHandle, "SendOrder")) == nullptr) return false;
	if ((GetClientPath = (_GetClientPath)GetProcAddress(libHandle, "GetClientPath")) == nullptr) return false;
	if ((GetClientPid = (_GetClientPid)GetProcAddress(libHandle, "GetClientPid")) == nullptr) return false;
	if ((GetClientRole = (_GetClientRole)GetProcAddress(libHandle, "GetClientRole")) == nullptr) return false;
	if ((GetClientOrder = (_GetClientOrder)GetProcAddress(libHandle, "GetClientOrder")) == nullptr) return false;
	if ((GetClientArg = (_GetClientArg)GetProcAddress(libHandle, "GetClientArg")) == nullptr) return false;
	if ((GetServiceOrder = (_GetServiceOrder)GetProcAddress(libHandle, "GetServiceOrder")) == nullptr) return false;
	if ((GetServiceArg = (_GetServiceArg)GetProcAddress(libHandle, "GetServiceArg")) == nullptr) return false;
	if ((SendServiceOrder = (_SendServiceOrder)GetProcAddress(libHandle, "SendServiceOrder")) == nullptr) return false;

	return true;
}
