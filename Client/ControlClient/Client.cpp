#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include "Resource.h"
Client GlobalClient;

Client::Client()
{
	m_isOnline = 0;
}

void Client::init(string ipAddress, int portNumber)
{
	m_serverAddress = ipAddress;
	m_serverPort = portNumber;

	int err;
	struct hostent* host;
	unsigned int addr;
	struct sockaddr_in server;

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) return;

	m_socketConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketConnect == INVALID_SOCKET) return;

	addr = inet_addr(m_serverAddress.c_str());
	host = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	if (host == NULL)
	{
		closesocket(m_socketConnect);
		return;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	server.sin_port = htons(m_serverPort);
	err = connect(m_socketConnect, (sockaddr*)&server, sizeof(server));
	if (err != 0)
	{
		closesocket(m_socketConnect);
		return;
	}

	m_isOnline = 1;
	return;
}

int Client::sendMessage(WCHAR* message, int len)
{
	int isResult = 0;
	isResult = send(m_socketConnect, (char*)message, len * 2 + 2, 0);
	if (isResult == INVALID_SOCKET) return 1;
	return 0;
}

int Client::receiveMessage()
{
	WCHAR message[DEFAULT_BUFLEN];

	int isResult = 0;
	isResult = recv(m_socketConnect, (char*)message, DEFAULT_BUFLEN, 0);
	if (isResult == INVALID_SOCKET) return 1;
	SendMessage(m_hwnd, WM_COMMAND, (WPARAM)IDC_RECEIVE, (LPARAM)message);

	return 0;
}

bool Client::isOnline()
{
	return m_isOnline;
}

void Client::setHWND(HWND hwnd)
{
	m_hwnd = hwnd;
}

void Client::setUsername(wstring username)
{
	m_username = username;
}

wstring& Client::getUsername()
{
	return m_username;
}

Client::~Client()
{
	if (m_isOnline)
	{
		closesocket(m_socketConnect);
	}
}

string loadServerAddress()
{
	ifstream fin;
	fin.open("Server//server.ini");
	if (!fin.is_open())
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(0, L"Unable to open server address file", L"ERROR", MB_OK | MB_ICONERROR);
		//SendMessage(m_hwnd, WM_COMMAND, (WPARAM)MB_IPFAIL, 0);
		return NULL;
	}
	string addr = "";
	getline(fin, addr);
	if (addr == "")
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(0, L"Unable to read server address file", L"ERROR", MB_OK | MB_ICONERROR);
		//SendMessage(m_hwnd, WM_COMMAND, (WPARAM)MB_IPFAIL, 0);
		return NULL;
	}
	return addr;
}