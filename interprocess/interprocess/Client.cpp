#include "stdafx.h"
#include "Client.h"


Client::Client(char role)
{
	sockaddr_in server = { 0 };

	sock = socket(PF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(5900);
	InetPton(AF_INET, L"127.0.0.1", &server.sin_addr.s_addr);

	if (sock == INVALID_SOCKET)
		std::exception("unable to open socket");
	int status = connect(sock, (struct sockaddr *)&server, sizeof(server));
	if (status)
		std::exception("connection failed");

	WCHAR buff[1024];
	GetModuleFileName(NULL, buff, 1023);
	this->self.filepath = buff;
	this->self.role = role;
	this->self.pid = GetCurrentProcessId();
	cout << "Connected ! " << endl;

	unsigned long block = 1;
	ioctlsocket(sock, FIONBIO, &block);
}

Client::Client(SOCKET _sock) : sock(_sock)
{
	WCHAR buff[1024];
	GetModuleFileName(NULL, buff, 1023);
	this->self.filepath = wstring(buff);
	this->self.role = SERVICE;
	this->self.pid = GetCurrentProcessId();

	unsigned long block = 1;
	ioctlsocket(sock, FIONBIO, &block);
}


Client::~Client()
{
	closesocket(sock);
}

void Client::Refresh(wstring const &cmd, wstring const &args)
{
	packet toSend = { 0 };
	packet toRecv = { 0 };


	toSend.role = self.role;
	wcscpy_s(toSend.filepath, self.filepath.c_str());
	wcsncpy_s(toSend.cmd, cmd.c_str(), 254);
	wcsncpy_s(toSend.argument, args.c_str(), 1023);
	toSend.pid = self.pid;

	Send(toSend);

	if (!Recv(toRecv))
		return;
	remote.filepath = toRecv.filepath;
	remote.role = toRecv.role;
	remote.pid = toRecv.pid;

	if (wcslen(toRecv.cmd))
		this->order = toRecv.cmd;
	if (wcslen(toRecv.argument))
		this->args = toRecv.argument;
}

bool Client::Send(packet const & data)
{
	int sendsize = send(sock, (char*)&data, sizeof(packet), 0);

	if (sendsize == -1)
		throw std::exception("client disconnected");
	if (sendsize != sizeof(packet))
		throw std::exception("client disconnected");
	return true;
}

bool Client::Recv(packet & data)
{
	int readsize = recv(sock, (char*)&data, sizeof(packet), 0);

	if (readsize == -1)
		return false;
	if (readsize != sizeof(packet))
		throw 1;
	return true;
}

wstring const & Client::getPath() const
{
	return path;
}

wstring Client::getOrder()
{
	wstring tmporder = order;
	
	order = L"";
	return tmporder;
}

wstring Client::getArgs()
{
	wstring tmparg = args;

	args = L"";
	return tmparg;
}

int Client::getPid() const
{
	return remote.pid;
}

char Client::getRole() const
{
	return remote.role;
}
