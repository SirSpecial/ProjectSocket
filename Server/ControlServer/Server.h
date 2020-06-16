#pragma once

#include "Flags.h"
#include "Client.h"
#include "User.h"
#include "Thread.h"

class Server
{
private:
	SOCKET m_socketListen;
	SOCKET m_socketClient;
	bool m_isOnline;
	int m_serverPort = 8080;
	HWND m_hwnd;
	list <Client*> m_clientList;
	list<User*> m_user;
public :
	Server();
	
	bool isOnline();
	void listenClient();
	int sendMessage(Client* client, WCHAR* message, int len);
	int receiveMessage(SOCKET sevSocket);
	void setHWND(HWND hwnd);

	bool userRegister(User* user);
	bool userLogin(SOCKET socket, User user);

	bool userChangePassword(User user, wstring new_password);

	bool isHaveUser(wstring username);
	bool isOnlineUser(wstring user);

	int getNumberUser();
	void addUser(User* user);
	list<User*>& getUser();

	~Server();
};

extern Server GlobalServer;

void loadData();
void loadDataUser();

void updateData(User* user);
void updateDataUser(User* user);