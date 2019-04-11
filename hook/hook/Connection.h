#pragma once

class Connection
{
public:
	Connection();
	~Connection();

	tuple<wstring, wstring> Read();
	void Send(wstring const &cmd, wstring const &arg);

private:
	bool importDll();


	HMODULE libHandle;
	// Dll imports

	typedef int(_cdecl *_StartServer)(); // Start listening
	_StartServer StartServer;
	typedef void(_cdecl *_StopServer)(); // Stop listening
	_StopServer StopServer;

	typedef int(_cdecl *_ConnectClient)(); // Connect to server
	_ConnectClient ConnectClient;
	typedef void(_cdecl *_DisconnectClient)(); // Disconnect from server
	_DisconnectClient DisconnectClient;

	typedef void(_cdecl *_Refresh)(); // Refresh Client / Server
	_Refresh Refresh;

	typedef int(_cdecl *_CountClients)(); // Get client count
	_CountClients CountClients;
	typedef void(_cdecl *_SendOrder)(int index, WCHAR *order, WCHAR *args); // Send order to client
	_SendOrder SendOrder;

	typedef WCHAR const * (_cdecl *_GetClientPath)(int index); // Get client path
	_GetClientPath GetClientPath;
	typedef int(_cdecl *_GetClientPid)(int index); // Get client PID
	_GetClientPid GetClientPid;
	typedef int(_cdecl *_GetClientRole)(int index); // Get client PID
	_GetClientRole GetClientRole;
	typedef WCHAR const * (_cdecl *_GetClientOrder)(int index); // Get client order (and set to null)
	_GetClientOrder GetClientOrder;
	typedef WCHAR const * (_cdecl *_GetClientArg)(int index); // // Get arg from client (and set to null)
	_GetClientArg GetClientArg;

	typedef WCHAR const * (_cdecl *_GetServiceOrder)(); // Get order from service (and set to null)
	_GetServiceOrder GetServiceOrder;
	typedef WCHAR const * (_cdecl *_GetServiceArg)(); // // Get arg from service (and set to null)
	_GetServiceArg GetServiceArg;
	typedef void(_cdecl *_SendServiceOrder)(WCHAR *order, WCHAR *args);  // Send an order to service
	_SendServiceOrder SendServiceOrder;
};

