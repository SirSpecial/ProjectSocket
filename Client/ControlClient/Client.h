#pragma once

#include "Flags.h"

class Client
{
public:
	Client();
	~Client();

	void init(string ipAddress, int portNumber);
	int sendMessage(WCHAR* message, int len);
	int receiveMessage();

	bool isOnline();
	void setHWND(HWND hwnd);
	void setUsername(wstring username);
	wstring& getUsername();
private:
	bool m_isOnline;
	string m_serverAddress;
	int m_serverPort;
	SOCKET m_socketConnect;
	HWND m_hwnd;
	wstring m_username;
};

extern Client GlobalClient;

string loadServerAddress();