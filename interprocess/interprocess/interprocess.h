#pragma once

#include "stdafx.h"

DLLEXPORT int StartServer(); // Start listening
DLLEXPORT void StopServer(); // Stop listening

DLLEXPORT int ConnectClient(bool isGui); // Connect to server
DLLEXPORT void DisconnectClient(); // Disconnect from server

DLLEXPORT void Refresh(); // Refresh Client / Server

DLLEXPORT int CountClients(); // Get client count
DLLEXPORT void SendOrder(int index, WCHAR *order, WCHAR *args); // Send order to client

DLLEXPORT WCHAR const *GetClientPath(int index); // Get client path
DLLEXPORT int GetClientPid(int index); // Get client PID
DLLEXPORT int GetClientRole(int index); // Get client PID
DLLEXPORT WCHAR const *GetClientOrder(int index); // Get client order (and set to null)
DLLEXPORT WCHAR const *GetClientArg(int index); // // Get arg from client (and set to null)

DLLEXPORT WCHAR const *GetServiceOrder(); // Get order from service (and set to null)
DLLEXPORT WCHAR const *GetServiceArg(); // // Get arg from service (and set to null)
DLLEXPORT void SendServiceOrder(WCHAR *order, WCHAR *args);  // Send an order to service