#include "Server.h"
#include "Resource.h"

Server GlobalServer;

Server::Server() 
{
	m_isOnline = 0;

	WSAData wsadata; 
	sockaddr_in server; 

	WORD wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0) return;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons((u_short)m_serverPort);

	m_socketListen = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketListen == INVALID_SOCKET) return;

	err = bind(m_socketListen, (sockaddr*)&server, sizeof(server));
	if (err != 0) return;

	err = listen(m_socketListen, 5);
	if (err != 0) return;

	m_isOnline = 1;
	return;
}

bool Server::isOnline() 
{
	return m_isOnline;
}

void Server::listenClient()
{
	sockaddr_in hostClient;
	int client_len = sizeof(hostClient);
	m_socketClient = accept(m_socketListen, (sockaddr*)&hostClient, &client_len);

	Client* client = new Client;
	client->socket = m_socketClient;

	if (m_socketClient != INVALID_SOCKET)
	{
		SendMessage(m_hwnd, WM_COMMAND, ID_USER_CONNECT, 0);
		m_clientList.push_back(client);
	}

	AfxBeginThread(receiveMessageThread, (void*) m_socketClient);
}

int Server::sendMessage(Client* client, WCHAR* message, int len)
{
	int isResult = 0;
	isResult = send(client->socket, (char*)message, len * 2 + 2, 0);

	if (isResult == SOCKET_ERROR)
		m_clientList.remove(client);
	if (isResult == SOCKET_ERROR)
		return 1;
	return 0;
}

int Server::receiveMessage(SOCKET recSocket)
{
	WCHAR temp[DEFAULT_BUFLEN];

	int isResult;
	int len;

	isResult = recv(recSocket, (char*)temp, DEFAULT_BUFLEN, 0);

	list<Client*>::iterator client;

	for (client = m_clientList.begin(); client != m_clientList.end(); client++)
		if ((*client)->socket == recSocket) break;

	if (isResult == SOCKET_ERROR)
	{
		m_clientList.remove(*client);
		SendMessage(m_hwnd, WM_COMMAND, ID_USER_LEAVE, 0);
		return 1;
	}

	WCHAR* message = temp;
	WCHAR send[DEFAULT_BUFLEN];
	switch (message[0])
	{
	case REGISTER:
	{
		/*
		* receive:	message = [FLAG | username | NULL | password | NULL]
		*send:		message = [FLAG | NULL]	
		*/
		WCHAR* username;
		WCHAR* password;

		username = message + 1;
		password = message + wcslen(message) + 1;

		User* user = new User;
		user->username = username;
		user->password = password;

		bool result = userRegister(user);
		if (result)
		{
			send[0] = REGISTER_SUCCESS;
			//SendMessage(m_hwnd, WM_COMMAND, ID_UPDATE, 0);
		}
		else send[0] = REGISTER_FAILURE;
		send[1] = NULL;
		sendMessage((*client), (WCHAR*)send, 1);
		break;
	}
	case LOGIN:
	{
		/*
		* receive:	message = [FLAG | username | NULL | password | NULL]
		* send:		message = [FLAG | NULL]
		*/
		WCHAR* username;
		WCHAR* password;

		username = message + 1;
		password = message + wcslen(message) + 1;

		User user;
		user.username = username;
		user.password = password;

		int result = userLogin(recSocket, user);
		if (result) send[0] = LOGIN_SUCCESS;
		else send[0] = LOGIN_FAILURE;
		send[1] = NULL;
		sendMessage((*client), (WCHAR*)send, 1);
		break;
	}
	case CHANGE_PASSWORD:
	{
		/*
		* receive:	message = [FLAG | username | NULL | old_password | NULL | new_password | NULL]
		* send:		message = [FLAG | NULL]
		*/
		WCHAR* username;
		WCHAR* old_password;
		WCHAR* new_password;

		username = message + 1;
		old_password = message + wcslen(message) + 1;
		new_password = message + wcslen(message) + 1 + wcslen(old_password) + 1;

		User user;
		user.username = username;
		user.password = old_password;

		bool result = userChangePassword(user, new_password);
		if (result) send[0] = CHANGE_PASSWORD_SUCCESS;
		else send[0] = CHANGE_PASSWORD_FAILURE;
		send[1] = NULL;
		sendMessage((*client), (WCHAR*)send, 1);
		break;
	}
	case CHECK_USER_FIND:
	{
		/*
		* receive:	message = [FLAG | username | NULL]
		* send:		message = [FLAG | NULL]
		*/
		WCHAR* username;

		username = message + 1;

		int isResult = isHaveUser(username);
		if (isResult) send[0] = USER_FIND_SUCCESS;
		else send[0] = USER_FIND_FAILURE;
		send[1] = NULL;
		sendMessage((*client), (WCHAR*)send, 1);
		break;
	}
	case CHECK_USER_ONLINE:
	{
		/*
		* receive:	message = [FLAG | username | NULL]
		* send:		message = [FLAG | 0 / 1 | NULL]		- Success
							  [FLAG | NULL]				- Fail
		*/
		WCHAR* username;

		username = message + 1;
		if (isHaveUser(username))
		{
			send[0] = USER_ONLINE_SUCCESS;
			int result = isOnlineUser(username);
			if (result) send[1] = 1;
			else send[1] = 0;
			len = 2;
			send[len] = NULL;
			sendMessage(*client, (WCHAR*)send, len);
		}
		else
		{
			send[0] = USER_ONLINE_FAILURE;
			send[1] = NULL;
			sendMessage((*client), (WCHAR*)send, 1);
		}
		break;
	}
	case CHECK_USER_SHOW_DATE:
	{
		/*
		* receive:	message = [FLAG | username | NULL]
		* send:		message = [FLAG | date     | NULL]	- Success
		*					  [FLAG | NULL]				- Fail
		*/
		WCHAR* username = message + 1;

		if (isHaveUser(username))
		{
			send[0] = USER_SHOW_DATE_SUCCESS;
			send[1] = NULL;
			for (auto userdata : m_user)
			{
				if (userdata->username == username)
				{
					std::wcscat(send, userdata->date.c_str());
					break;
				}
			}
			len = wcslen(send);
			sendMessage(*client, (WCHAR*)send, len);
		}
		else
		{
			send[0] = USER_SHOW_DATE_FAILURE;
			send[1] = NULL;
			sendMessage(*client, (WCHAR*)send, 1);
		}
		break;
	}
	case CHECK_USER_SHOW_FULLNAME:
	{
		/*
		* receive:	message = [FLAG | username | NULL]
		* send:		message = [FLAG | fullname | NULL]	- Success
		*					  [FLAG | NULL]				- Fail
		*/
		WCHAR* username = message + 1;

		if (isHaveUser(username))
		{
			send[0] = USER_SHOW_FULLNAME_SUCCESS;
			send[1] = NULL;
			for (auto userdata : m_user)
			{
				if (userdata->username == username)
				{
					std::wcscat(send, userdata->fullname.c_str());
					break;
				}
			}
			len = wcslen(send);
			sendMessage(*client, (WCHAR*)send, len);
		}
		else
		{
			send[0] = USER_SHOW_FULLNAME_FAILURE;
			send[1] = NULL;
			sendMessage(*client, (WCHAR*)send, 1);
		}
		break;
	}
	case CHECK_USER_SHOW_NOTE:
	{
		/*
		* receive:	message = [FLAG | username | NULL]
		* send:		message = [FLAG | note	   | NULL]	- Success
		*					  [FLAG]					- Fail
		*/
		WCHAR* username = message + 1;

		if (isHaveUser(username))
		{
			send[0] = USER_SHOW_NOTE_SUCCESS;
			send[1] = NULL;
			for (auto userdata : m_user)
			{
				if (userdata->username == username)
				{
					std::wcscat(send, userdata->note.c_str());
					break;
				}
			}
			len = wcslen(send);
			sendMessage(*client, (WCHAR*)send, len);
		}
		else
		{
			send[0] = USER_SHOW_NOTE_FAILURE;
			send[1] = NULL;
			sendMessage(*client, (WCHAR*)send, 1);
		}
		break;
	}
	case CHECK_USER_SHOW_ALL:
	{
		/*
		* receive:	message = [FLAG | username | NULL]
		* send:		message = [FLAG | 0 / 1 | fullname | NULL | date | NULL | note | NULL]	- Success
		*					  [FLAG | NULL]													- Fail
		*/
		WCHAR* username = message + 1;

		if (isHaveUser(username))
		{
			send[0] = USER_SHOW_ALL_SUCCESS;
			if (isOnlineUser(username))
				send[1] = USER_ONLINE_SUCCESS;
			else send[1] = USER_ONLINE_FAILURE;
			send[2] = NULL;
			WCHAR date[255];
			WCHAR note[255];
			WCHAR fullname[255];
			for (auto userdata : m_user)
			{
				if (userdata->username == username)
				{
					std::wcscpy(date, userdata->date.c_str());
					std::wcscpy(fullname, userdata->fullname.c_str());
					std::wcscpy(note, userdata->note.c_str());
					break;
				}
			}

			int i, j;

			std::wcscat(send, fullname);
			i = wcslen(send);
			send[i] = NULL;

			i++;
			for (j = 0; j < wcslen(date); j++)
				send[i + j] = date[j];
			i += j;
			send[i] = NULL;

			i++;
			for (j = 0; j < wcslen(note); j++)
				send[i + j] = note[j];
			i += j;
			send[i] = NULL;

			len = i;
			sendMessage(*client, (WCHAR*)send, len);
		}
		else
		{
			send[0] = USER_SHOW_ALL_FAILURE;
			send[1] = NULL;
			sendMessage(*client, (WCHAR*)send, 1);
		}
		break;
	}
	case SETUP_INFO_FULLNAME:
	{
		/*
		* receive:	message = [FLAG | username | NULL | fullname | NULL]
		* send:		message = [FLAG | NULL]															
		*/
		WCHAR* username;
		WCHAR* fullname;

		username = message + 1;
		fullname = message + wcslen(message) + 1;
		int result = 0;

		for (auto userdata : m_user)
		{
			if (userdata->username == username)
			{

				userdata->fullname = fullname;
				updateDataUser(userdata);
				//SendMessage(m_hwnd, WM_COMMAND, ID_UPDATE, 0);
				result = 1;
				break;
			}
		}

		if (result) send[0] = SETUP_INFO_FULLNAME_SUCCESS;
		else send[0] = SETUP_INFO_FULLNAME_FAILURE;
		send[1] = NULL;
		sendMessage((*client), (WCHAR*)send, 1);
		break;
	}
	case SETUP_INFO_DATE:
	{
		/*
		* receive:	message = [FLAG | username | NULL | date | NULL]
		* send:		message = [FLAG | NULL]														
		*/
		WCHAR* username;
		WCHAR* date;

		username = message + 1;
		date = message + wcslen(message) + 1;
		int result = 0;

		for (auto userdata : m_user)
		{
			if (userdata->username == username)
			{

				userdata->date = date;
				updateDataUser(userdata);
				//SendMessage(m_hwnd, WM_COMMAND, ID_UPDATE, 0);
				result = 1;
				break;
			}
		}

		if (result) send[0] = SETUP_INFO_DATE_SUCCESS;
		else send[0] = SETUP_INFO_DATE_FAILURE;
		send[1] = NULL;
		sendMessage((*client), (WCHAR*)send, 1);
		break;
	}
	case SETUP_INFO_NOTE:
	{
		/*
		* receive:	message = [FLAG | username | NULL | note | NULL]
		* send:		message = [FLAG | NULL]															
		*/
		WCHAR* username;
		WCHAR* note;

		username = message + 1;
		note = message + wcslen(message) + 1;
		int result = 0;

		for (auto userdata : m_user)
		{
			if (userdata->username == username)
			{

				userdata->note = note;
				updateDataUser(userdata);
				//SendMessage(m_hwnd, WM_COMMAND, ID_UPDATE, 0);
				result = 1;
				break;
			}
		}

		if (result) send[0] = SETUP_INFO_NOTE_SUCCESS;
		else send[0] = SETUP_INFO_NOTE_FAILURE;
		send[1] = NULL;
		sendMessage((*client), (WCHAR*)send, 1);
		break;
	}
	case CREATE_CHAT:
	{
		/*
		* receive:	message = [FLAG | partner | NULL]
		* send:		message = [FLAG | parter  | NULL]	- Succeed
		*					  [FLAG]					- Fail
		*/

		WCHAR* partner;
		partner = message + 1;
		bool result = isOnlineUser(partner);
		if (result)
		{

			send[0] = CREATE_CHAT_SUCCESS;
			send[1] = NULL;
			wcscat_s(send, partner);
			sendMessage((*client), (WCHAR*)send, wcslen(send));
		}
		else
		{
			send[0] = CREATE_CHAT_FAILURE;
			send[1] = NULL;
			sendMessage((*client), (WCHAR*)send, 1);
		}
		break;
	}
	case CHAT:
	{

		/*
		* receive:	message = [FLAG | sender | NULL | receiver | NULL | content | NULL]
		* receive:	message = [FLAG | sender | NULL | receiver | NULL | content | NULL]
		*/
		WCHAR* receiver;
		receiver = message + wcslen(message) + 1;

		for (auto client : m_clientList)
		{
			if (wcscmp(client->username.c_str(), receiver) == 0)
			{
				sendMessage(client, (WCHAR*)message, isResult / 2);
				break;
			}
		}
		break;
	}
	case END_CHAT:
	{
		/*
		* receive:	message = [FLAG | sender | NULL | receiver | NULL]
		* send:		message = [FLAG | sender | NULL | receiver | NULL]
		*/
		WCHAR* receiver = message + wcslen(message) + 1;

		for (auto client : m_clientList)
		{
			if (wcscmp(client->username.c_str(), receiver) == 0)
			{
				sendMessage(client, (WCHAR*)message, isResult / 2);
				break;
			}
		}
		break;
	}
	
	}
	return 0;
}

void Server::setHWND(HWND hwnd)
{
	m_hwnd = hwnd;
}

bool Server::userRegister(User* user)
{
	for (auto userdata : m_user)
		if (userdata->username == user->username) return 0;
	m_user.push_back(user);
	updateData(user);
	return 1;
}

bool Server::userLogin(SOCKET socket, User user)
{
	for (auto userdata : m_user)
	{
		if (userdata->username == user.username && userdata->password == user.password)
		{
			for (auto client : m_clientList)
				if (client->socket == socket)
				{
					client->username = user.username;
					return 1;
				}
		}
	}
		
	return 0;
}

bool Server::isHaveUser(wstring username)
{
	for (auto userdata: m_user)
		if (username == userdata->username) return 1;
	return 0;
}

bool Server::userChangePassword(User user, wstring new_password)
{
	for (auto userdata : m_user)
	{
		if (userdata->username == user.username && userdata->password == user.password)
		{
			(*userdata).password = new_password;
			updateDataUser(userdata);
			return 1;
		}
	}
	return 0;
}

bool Server::isOnlineUser(wstring username)
{
	for (auto client : m_clientList)
		if (username == client->username) return 1;
	return 0;
}

void Server::addUser(User* user)
{
	m_user.push_back(user);
}

list<User*>& Server::getUser()
{
	return m_user;
}

int Server::getNumberUser()
{
	return m_user.size();
}

Server::~Server() //close m_socketListen, delete m_clientList
{
	closesocket(m_socketListen);
	WSACleanup();

	if (m_clientList.size() > 0)
	{
		for (auto client : m_clientList) delete client;
	}
}

void loadData()
{
	/*format userdata.ini:
	username 1
	username 2
	username n
	*/
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fin(L"User\\userdata.ini");
	fin.imbue(loc);

	WCHAR buffer[200];

	while (!fin.eof())
	{
		fin.getline(buffer, 200);
		auto user = new User;
		user->username = buffer;
		GlobalServer.addUser(user);
	}
	loadDataUser();
	fin.close();
}

void loadDataUser()
{
	/*format userdata.ini:
	username
	pasword
	fullname
	date
	note
   */
	list <User*> user;
	user = GlobalServer.getUser();
	for (auto userdata : user)
	{
		locale loc(locale(), new codecvt_utf8<wchar_t>);
		wstring path = L"User\\" + userdata->username + L"\\info.ini";
		wifstream fin(path);
		fin.imbue(loc);

		WCHAR buffer[255];

		fin.getline(buffer, 255); //username

		fin.getline(buffer, 255); //password
		userdata->password = buffer;

		fin.getline(buffer, 255); //fullname
		userdata->fullname = buffer;

		fin.getline(buffer, 255); //date
		userdata->date = buffer;

		fin.getline(buffer, 255); //date
		userdata->note = buffer;

		fin.close();
	}
}

void updateData(User* user)
{
	wofstream fos(L"User\\userdata.ini", ios::app);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	fos.imbue(loc);
	if (!fos.is_open()) return;
	fos.seekp(ios::end);
	fos << user->username << L"\n";

	wstring path = L"User\\" + user->username;
	_wmkdir(path.c_str());
	updateDataUser(user);
	fos.close();
}

void updateDataUser(User* user)
{
	wstring path = L"User\\" + user->username + L"\\info.ini";
	wofstream fos(path);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	fos.imbue(loc);
	if (!fos.is_open()) return;

	fos << user->username << L"\n";
	fos << user->password << L"\n";
	fos << user->fullname << L"\n";
	fos << user->date << L"\n";
	fos << user->note << L"\n";
	fos.imbue(loc);
}